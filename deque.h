#include <iostream>
#include <vector>
#include <iterator>

template <typename T>
class Deque{
private:
      size_t first = 0;
      std::vector<std::vector<T>*> buckets;
      size_t sz = 0;
      static const size_t buf_size = 10;
      bool isEnough() {
          return (first + sz < buf_size * buckets.size());
      }
      void increase() {
          if (!buckets.size()) {
              auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
              buckets.push_back(current);
              first = buf_size / 2;
              return;
          }
          size_t prev_size = buckets.size();
          for (size_t i = 0; i < prev_size; ++i) {
              auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
              buckets.push_back(current);
          }
          for (size_t i = buckets.size()/2 ; i > 0; --i) {
              std::swap(buckets[i - 1], buckets[i + buckets.size()/4 - 1]);
          }
          first += buf_size*(buckets.size()/4);
      }
public:
    ~Deque();
    Deque();
    explicit Deque(size_t n);
    Deque(size_t n, const T&);
    Deque(const Deque& arr);
    Deque& operator=(const Deque& arr);
    T& operator[](size_t i);
    const T& operator[](size_t i) const;
    T& at(size_t i) ;
    const T& at(size_t i) const;
    size_t size() const;
    void push_back(const T& value);
    void pop_back();
    void push_front(const T& value);
    void pop_front();

    template <bool IsConst>
    struct common_iterator {
        std::conditional_t<IsConst, T*, const T*> ptr;
        std::conditional_t<IsConst, std::vector<std::vector<T>*> *, const std::vector<std::vector<T>*> *> main_ptr;
        size_t place;
        common_iterator(T* ptr, size_t place,
                        std::vector<std::vector<T>*>* main_ptr): ptr(ptr), main_ptr(main_ptr) ,place(place) {};
        common_iterator(const T* ptr, size_t place,
                        const std::vector<std::vector<T>*>* main_ptr): ptr(ptr), main_ptr(main_ptr), place(place) {};

        std::conditional_t<IsConst, T&, const T&> operator*() const{
            return *ptr;
        }
        common_iterator& operator++() {
            ++place;
                auto k = (*main_ptr)[place / buf_size];
                ptr = &((*k)[place % buf_size]);

            return *this;
        }
        common_iterator& operator--() {
            --place;
                auto k = (*main_ptr)[place / buf_size];
                ptr = &((*k)[place % buf_size]);

            return *this;
        }
        common_iterator& operator+(int n) {
                place += n;
                auto k = (*main_ptr)[place / buf_size];
                ptr = &((*k)[place % buf_size]);

            return *this;
        }
        common_iterator& operator-(int n) {
            place -= n;

                auto k = (*main_ptr)[place / buf_size];
                ptr = &((*k)[place % buf_size]);
            return *this;
        }
        common_iterator& operator-(common_iterator current) {
            auto it = new common_iterator(ptr, place, main_ptr);
            it->place -= current.place;
            auto k = (*it->main_ptr)[it->place / buf_size];
            it->ptr = &((*k)[it->place % buf_size]);

            return *it;
        }
        std::conditional_t<IsConst, T*, const T*> operator ->() const {
            return ptr;
        }
        operator int() const {
            return place;
        }
        bool operator==(common_iterator current) const {
            return ptr == current.ptr;
        }
        bool operator!=(common_iterator current) const {
            return !(current == *this);
        }
        bool operator<(common_iterator current) const {
            return place < current.place && current.ptr != ptr;
        }
        bool operator>(common_iterator current) const  {
            return  current < *this && current.ptr != ptr;
        }
        bool operator<=(common_iterator current) const {
            return !(*this > current);
        }
        bool operator>=(common_iterator current) const {
            return !(*this < current);
        }

    };

    using iterator = common_iterator<true>;
    using const_iterator = common_iterator<false>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

     iterator begin() {
            return iterator(&(*this)[0], first, &(this->buckets));
    };
    iterator end() {
        return iterator(&(*this)[sz],first + sz, &(this->buckets));
    };
    const_iterator begin() const {
        const T* al = &((*this)[0]);
        return const_iterator(al, first, &(this->buckets));
    };
    const_iterator end() const {
        return const_iterator(&(*this)[sz],first + sz, &(this->buckets));
    };
    const_iterator cbegin() const{
        return const_iterator(&(*this)[0], first, &(this->buckets));
    };
    const_iterator cend()  const {
        return const_iterator(&(*this)[sz], first + sz, &(this->buckets));
    };
    reverse_iterator rbegin() {
        return reverse_iterator(&(*this)[sz], first + sz, &(this->buckets));
    };
    reverse_iterator rend() {
        return reverse_iterator(&(*this)[0], first, &(this->buckets));
    };
    const_reverse_iterator crbegin() const{
        return const_reverse_iterator(&(*this)[sz], first + sz, &(this->buckets));
    };
    const_reverse_iterator crend() const{
        return const_reverse_iterator(&(*this)[0], first, &(this->buckets));
    };
    void insert(iterator it, const T&);
    void erase(iterator it);

};
// Constructors
template<typename T>
Deque<T>::Deque() = default;

template<typename T>
Deque<T>::Deque(size_t n): sz(n) {
    first = 0;
    size_t capacity = n / buf_size + 1;
    for (size_t  i = 0; i < capacity; ++i) {
        auto *current = new std::vector<T>(buf_size);
        buckets.push_back(current);
    }
}

template<typename T>
Deque<T>::Deque(size_t n, const T& fill): sz(n) {
    first = 0;
    size_t copy = n;
    size_t capacity = n / buf_size + 1;
    for (size_t  i = 0; i < capacity; ++i) {
        auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
        buckets.push_back(current);
    }
    size_t i = first;
    for(; i < first + copy; ++i) {
        (*this)[i] = T(fill);
    }
    auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
    buckets.push_back(current);
}

template<typename T>
Deque<T>::Deque(const Deque& arr) {
    //std:: cerr << "here!copy";
    sz = arr.sz;
    first = arr.first;
    for (size_t i = 0; i < arr.buckets.size(); ++i) {
        auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
        buckets.push_back(current);
    }
        size_t i = arr.first;
        for(; i < first + arr.sz; ++i) {
            (*this)[i]  =  arr[i];
        }
    auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
    buckets.push_back(current);
}

template<typename T>
T& Deque<T>::operator[](size_t i) {
    auto& cur  = *buckets[(first + i ) / buf_size];
    return cur[(first + i) % buf_size];
}

template<typename T>
const T& Deque<T>::operator[](size_t i) const {
    auto& cur  = *buckets[(first + i)/ buf_size];
    return cur[(first + i) % buf_size];
}

template<typename T>
T& Deque<T>::at(size_t i) {
    if (i >= sz) throw std::out_of_range("..."); //attention!!!
    auto& cur  = *buckets[i / buf_size];
    return cur[i % buf_size];
}

template<typename T>
const T& Deque<T>::at(size_t i) const {
    if (i >= sz) throw std::out_of_range("...");//attention!!!
    auto& cur  = *buckets[i / buf_size];
    return cur[i % buf_size];
}

template<typename T>
size_t Deque<T>::size() const {
    return sz;
}

template<typename T>
void Deque<T>::push_back(const T& value) {
    if (!isEnough()) {
        increase();
    }
    ++sz;
    (*this)[sz-1] = value;
}

template<typename T>
void Deque<T>::pop_back() {
    (*buckets[(first + sz - 1)/buf_size]).pop_back();
    --sz;
}

template<typename T>
void Deque<T>::push_front(const T &value) {
    while (!first) increase();
    auto& cur  = *buckets[(first-1) / buf_size];
    cur[(first -1) % buf_size] = value;
    --first;
    ++sz;
}

template<typename T>
void Deque<T>::pop_front() {
    ++first;
    --sz;
}

template<typename T>
void Deque<T>::insert(Deque::iterator it, const T& value) {
    size_t place = it.place;
    auto it2 = this->end();
    this->push_back(value);
    while(  it2.place != (place)) {
        std::swap((*this)[it2.place - first], (*this)[it2.place - first - 1]);
        --it2;
    }
}

template<typename T>
void Deque<T>::erase(Deque::iterator it) {
    while(it.place != (first + sz - 1)) {
        std::swap((*this)[it.place - first], (*this)[it.place - first + 1]);
        ++it;
    }
    this->pop_back();
}

template<typename T>
Deque<T>::~Deque() {
    for ( size_t i = 0; i < buckets.size(); ++i) {
            //for (size_t  j = 0; j < buf_size; ++j) {
                //&((*buckets[i])[j])->~T();
            //}
            delete buckets[i];
    }
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque &arr) {
    sz = arr.sz;
    first = arr.first;
    for (size_t i = 0; i < arr.buckets.size(); ++i) {
        auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
        buckets.push_back(current);
    }
    size_t i = arr.first;
    for(; i < first + arr.sz; ++i) {
        (*this)[i]  =  arr[i];
    }
    auto current  = reinterpret_cast <std::vector<T>*> (new std::vector<int8_t[sizeof(T)]>(buf_size));
    buckets.push_back(current);
    return *this;
}




