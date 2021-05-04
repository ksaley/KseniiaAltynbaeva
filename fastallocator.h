#include <vector>

template <size_t chunkSize>
class FixedAllocator {
    const int block_size = 100;
    std::vector<char*> blocks;
    char* alloc_ptr = nullptr;
    std::vector<char*> empty_chunks;
public:
    FixedAllocator();
    ~FixedAllocator();
    char* allocate();
    static char* getInstance(char* prt = nullptr);
    void deallocate(char* ptr);
};

template<size_t chunkSize>
char* FixedAllocator<chunkSize>::allocate() {
    if (!this->empty_chunks.empty()) {
        auto copy = this->empty_chunks.back();
        this->empty_chunks.pop_back();
        return copy;
    }
    char* copy;
    if (!this->alloc_ptr) {
        char* block  = reinterpret_cast<char*> (::operator new[](block_size * chunkSize));
        this->alloc_ptr = reinterpret_cast<char*> (block);// А зачем второй раз кастовать тот же самый блок?
        this->blocks.push_back(block);
        copy = this->alloc_ptr;
        this->alloc_ptr += chunkSize;
    }
    else {
        if (this->alloc_ptr !=  this->blocks.back() + block_size * chunkSize) {
            copy = this->alloc_ptr;
            this->alloc_ptr += chunkSize;
        }
        else {
            char* block = reinterpret_cast<char*> (::operator new[](block_size * chunkSize));
            this->alloc_ptr = block;
            this->blocks.push_back(block);
            copy = this->alloc_ptr;
            this->alloc_ptr += chunkSize;
        }
    }
    return copy;
}

// Такую функцию по хорошему следует делать так же static
template<size_t chunkSize>
char* FixedAllocator<chunkSize>::getInstance(char* ptr) {
//    if (single == nullptr) {
//            single = new FixedAllocator<chunkSize>;
//            //single = std::make_shared<FixedAllocator<chunkSize>>;
//        }
    static FixedAllocator<chunkSize> single;
    if (!ptr) {
        return single.allocate();
    }
    else {
        single.deallocate(ptr);
        return nullptr;
    }
}

template<size_t chunkSize>
FixedAllocator<chunkSize>::FixedAllocator() {}

template<size_t chunkSize>
void FixedAllocator<chunkSize>::deallocate(char* ptr) {
    (this->empty_chunks).push_back(ptr);
}

template<size_t chunkSize>
FixedAllocator<chunkSize>::~FixedAllocator() {
    //std::cerr << "destructor";
    for (size_t i = 0; i < this->blocks.size(); ++i) {
        delete[] this->blocks[i];
    }
}


template <typename T>
class FastAllocator {
public:
    FastAllocator() = default;
    template <typename U>
    FastAllocator(const FastAllocator<U>&) {}
    template <typename U>
    FastAllocator& operator =(const FastAllocator<U>& alloc) {}
    template <typename U>
    bool operator ==(const FastAllocator<U>&) {
        return true;
    }
    template <typename U>
    bool operator !=(const FastAllocator<U>&) {
        return false;
    }
    using value_type = T;
    T* allocate(size_t n) {
        if ((sizeof(T) % 8 == 0 || sizeof(T) % 4 == 0) && sizeof(T) < 25 && n == 1) {
            return reinterpret_cast<T*> (FixedAllocator<sizeof(T)>::getInstance(nullptr));
        }
        return reinterpret_cast<T*> (::operator new(n * sizeof(T)));
    }
    void deallocate(T* ptr,size_t n) {
        if ((!(sizeof(T) % 8) || !(sizeof(T) % 4)) && sizeof(T) < 25 && n == 1) {
            FixedAllocator<sizeof(T)>::getInstance(reinterpret_cast<char*> (ptr));
            return;
        }
        ::operator delete(ptr);
    }
    template <typename... Args>// В таком виде не обязательно их объявлять, они будут сгенерированы автоматически через TypeTraits
    void construct(T* ptr, const Args&... args) {
        new(ptr) T(args...);
    }
    void destroy(T* ptr) {
        ptr->~T();
    }
};


template <typename T, typename Allocator = std::allocator<T>>
class List {
private:
    size_t sz = 0;
    struct Node {
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;
        Node() : value(T()){}
        Node(const T& value):value(value){}
    };
    Node* fake;
    Node* head;
    typename std::allocator_traits<Allocator>::template rebind_alloc<Node> alloc;
    using AllocTraits = std::allocator_traits<typename std::allocator_traits<Allocator>::template rebind_alloc<Node>>;
public:
    List& operator=(const List& another);
    List(const List& another);
    explicit List(const Allocator& alloc = Allocator());
    explicit List(size_t count, const Allocator&  alloc = Allocator());
    explicit List(size_t count,  const T& value, const Allocator&  alloc = Allocator());
    Allocator get_allocator();
    ~List();
    size_t size() const;
    void push_back(const T& elem);
    void pop_back();
    void pop_front();
    void push_front(const T& elem);

    template <bool IsConst>
    struct common_iterator {

        using difference_type = std::ptrdiff_t;
        using value_type = std::conditional_t<IsConst,const T, T>;
        using pointer = std::conditional_t<IsConst,const T*, T*>;
        using reference = std::conditional_t<IsConst,const T&, T&>;
        using iterator_category = std::bidirectional_iterator_tag;
        Node* ptr;
        common_iterator(Node* ptr):ptr(ptr){}
        common_iterator(const Node* ptr):ptr(ptr){}

        common_iterator(const common_iterator<false>& it) {
            ptr = it.ptr;
        }

        common_iterator() {}
        std::conditional_t<IsConst,const T&, T&> operator*() {// reference
            return ptr->value;
        }
        //common_iterator(const common_iterator& it) {
        //ptr = it.ptr;
        //}
        common_iterator operator=(const common_iterator& it) {
            ptr = it.ptr;
            return ptr;
        }
        common_iterator& operator++()  {
            ptr = ptr->next;
            return *this;
        }
        common_iterator operator++(int) {
            common_iterator copy;
            copy.ptr = ptr;
            ptr = ptr->next;
            return copy;
        }
        common_iterator operator--(int) {
            common_iterator copy;
            copy.ptr = ptr;
            ptr = ptr->prev;
            return copy;
        }
        common_iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }

        std::conditional_t<IsConst,const T*, T*> operator ->() const {
            return ptr;
        }

        bool operator==(common_iterator current) const {
            return ptr == current.ptr;
        }
        bool operator!=(common_iterator current) const {
            return !(current == *this);
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    iterator begin() {
        return iterator(head);
    }
    iterator end() {
        return iterator(fake);
    }
    const_iterator begin() const {
        return const_iterator(head);
    }

    const_iterator end() const {
        return const_iterator(fake);
    }
    const_iterator cbegin() const{
        return const_iterator(head);
    }
    const_iterator cend()  const {
        return const_iterator(fake);
    }
    reverse_iterator rbegin() const {
        return reverse_iterator(fake);
    }
    reverse_iterator rend() const {
        return reverse_iterator(head);
    }
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(fake);
    }
    const_reverse_iterator crend() const{
        return const_reverse_iterator(head);
    }

    void insert(const_iterator it, const T& value);
    void erase(const_iterator it);
};

template<typename T, typename Allocator>
List<T, Allocator>::List(const Allocator &alloc):alloc(alloc) {
    fake = reinterpret_cast<Node*> (operator new(sizeof(Node)));// У тебя для этого есть аллокатор. -5%
    fake->next = fake;
    fake->prev = fake;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const T& value, const Allocator& alloc):List(alloc) {
    sz = count;
    head = AllocTraits::allocate(this->alloc, 1);
    AllocTraits::construct(this->alloc, head, value);
    fake->prev = head;
    fake->next = head;
    head->prev = fake;
    head->next = fake;
    Node* prev = head;
    for ( size_t i = 0; i < count - 1; ++i) {
        Node* cur = AllocTraits::allocate(this->alloc, 1);
        AllocTraits::construct(this->alloc, cur, value);
        cur->prev = prev;
        prev->next = cur;
        std::swap(cur, prev);
    }
    prev->next = fake;
    fake->prev = prev;
}

template<typename T, typename Allocator>
Allocator List<T, Allocator>::get_allocator() {
    return alloc;
}

template<typename T, typename Allocator>
List<T, Allocator>::~List() {
    Node* current = fake;
    current = current->next;
    while (current->next!=fake) {
        auto copy = current->next;
        AllocTraits::destroy(alloc, current);
        AllocTraits::deallocate(alloc, current, 1);
        current = copy;
    }
    if (current!= fake) {
        AllocTraits::destroy(alloc, current);
        AllocTraits::deallocate(alloc, current, 1);
    }
    operator delete(fake);
}

template<typename T, typename Allocator>
size_t List<T, Allocator>::size() const {
    return sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_back(const T &elem) {
    if (sz == 0) {
        Node* pushed = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, pushed, elem);
        head = pushed;
        head->prev = fake;
        head->next = fake;
        fake->next = head;
        fake->prev = head;
    }
    else {
        auto current = fake->prev;
        Node *pushed = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, pushed, elem);
        current->next = pushed;
        pushed->prev = current;
        pushed->next = fake;
        fake->prev = pushed;
    }
    ++sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
    if (sz == 1) {
        AllocTraits::destroy(alloc, head);
        AllocTraits::deallocate(alloc, head, 1);
    }
    else {
        auto new_last = fake->prev->prev;
        AllocTraits::destroy(alloc, fake->prev);
        AllocTraits::deallocate(alloc, fake->prev, 1);
        new_last->next = fake;
        fake->prev = new_last;
    }
    --sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
    if (sz == 1) {
        AllocTraits::destroy(alloc, head);
        AllocTraits::deallocate(alloc, head, 1);
        fake->next = fake;
        fake->prev =fake;
    } else {
        auto new_head = head->next;
        AllocTraits::destroy(alloc, head);
        AllocTraits::deallocate(alloc, head, 1);
        new_head->prev = fake;
        fake->next = new_head;
        head = new_head;
    }
    --sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_front(const T &elem) {
    if (sz == 0) {
        Node* pushed = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, pushed, elem);
        head = pushed;
        head->prev = fake;
        head->next = fake;
        fake->next = head;
        fake->prev = head;
    }
    Node* pushed = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, pushed, elem);
    head->prev = pushed;
    pushed->next = head;
    fake->next = pushed;
    pushed->prev = fake;
    std::swap(head, pushed);
    ++sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::insert(List::const_iterator it, const T & value) {
    auto inserted = AllocTraits::allocate(alloc, 1);
    AllocTraits ::construct(alloc, inserted, value);
    auto next = it.ptr;
    auto prev = next->prev;
    next->prev = inserted;
    inserted->next = next;
    inserted->prev = prev;
    prev->next = inserted;
    ++sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::erase(List::const_iterator it) {
    auto erased = it.ptr;
    auto prev = erased->prev;
    auto next = erased->next;
    prev->next = next;
    next->prev = prev;
    if (head == erased) {
        head = head->next;
    }
    AllocTraits::destroy(alloc, erased);
    AllocTraits ::deallocate(alloc, erased, 1);
    --sz;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List& another) {
    fake = reinterpret_cast<Node*> (operator new(sizeof(Node)));// Аналогично, для этого есть аллокатор
    fake->next = fake;
    fake->prev = fake;
    alloc = AllocTraits::select_on_container_copy_construction(another.alloc);
    head = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, head,another.head->value);
    fake->next = head;
    head->prev = fake;
    auto current = another.head->next;
    Node* prev = head;
    while (current!=another.fake) {
        Node* cur = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, cur, current->value);
        cur->prev = prev;
        prev->next = cur;
        std::swap(cur, prev);
        current = current->next;
    }
    prev->next = fake;
    fake->prev = prev;
    sz = another.sz;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const Allocator &alloc): List(alloc) {
    sz = count;
    head = AllocTraits::allocate(this->alloc, 1);
    AllocTraits::construct(this->alloc, head);
    fake->prev = head;
    fake->next = head;
    head->prev = fake;
    head->next = fake;
    Node* prev = head;
    for ( size_t i = 0; i < count - 1; ++i) {
        Node* cur = AllocTraits::allocate(this->alloc, 1);
        AllocTraits::construct(this->alloc, cur);
        cur->prev = prev;
        prev->next = cur;
        prev = cur;
    }
    prev->next = fake;
    fake->prev = prev;
}

template<typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List& another) {
    if (this == &another) return *this;
    while(sz > 0) {
        this->pop_back();
    }
    auto it = another.begin();
    if (AllocTraits::propagate_on_container_copy_assignment::value
        && alloc != another.alloc) {
        alloc = another.alloc;
    }
    while(sz < another.sz) {
        this->push_back(*it);
        it++;
    }
    return *this;
}
