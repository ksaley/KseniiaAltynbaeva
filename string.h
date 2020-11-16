#include <iostream>
#include <cstring>

class String {
private:
    size_t sz = 0;
    size_t capacity = 0;
    char *str = nullptr;

    void change_capacity_up() {
        if (capacity == 0) capacity = 1;
        else capacity *= 2;
        char* copy = new char[capacity];
        memcpy(copy,str, sz);
        std::swap(copy,str);
        delete[] copy;
    }

    void change_capacity_down() {
        capacity /= 2;
        char* copy = new char[capacity];
        memcpy(copy,str, sz);
        std::swap(copy, str);
        delete[] copy;
    }

public:
    String() = default;

    String(size_t sz, char c = '\0') : sz(sz), capacity(sz), str(new char[sz]) {
        memset(str, c, sz);
    }

    String(const char* s): sz(strlen(s)),capacity(sz), str(new char[sz]) {
        strcpy(str, s);
    }

    ~String() {
        delete[] str;
    }

    String(const String &s) : String(s.sz,'\0') {
        memcpy(str, s.str, sz);
    }

    String &operator=( String s) {
        swap(s);
        return *this;
    }

    void swap(String &s) {
        std::swap(sz, s.sz);
        std::swap(capacity, s.capacity);
        std::swap(str, s.str);
    }


    void push_back(char back) {
        if (sz + 1 > capacity) {
            change_capacity_up();
        }
        str[sz] = back;
        ++sz;
    }
    void pop_back() {
        if (sz - 1 <= capacity/4) {
            change_capacity_down();
        }
        --sz;
    }
    size_t length() const {
        return sz;
    }
    bool empty() const{
        if (sz == 0) return true;
        return false;
    }
    char& operator[](size_t i) {
        return str[i];
    }
    char operator[](size_t i) const {
        return str[i];
    }
    bool operator==(const String& s) {
        bool flag = true;
        size_t index = 0;
        if (sz != s.sz) return false;
        while (flag && index < sz) {
            flag = (str[index] == s.str[index]);
            ++index;
        }
        return flag;
    }

    char& front() {
        return str[0];
    }
    char front() const {
        return str[0];
    }
    char& back() {
        return str[sz - 1];
    }
    char back() const {
        return str[sz - 1];
    }
    String substr(size_t start, size_t count) const {
        String newstring(count + 1);
        memcpy(newstring.str, str + start,count);
        --newstring.sz;
        return newstring;
    }
    void clear() {
        while (sz> 0) {
            pop_back();
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, const String& s) {
        for (size_t i = 0; i < s.sz; ++i) {
            stream << s.str[i];
        }
        return stream;
    }
    friend std::istream& operator>>(std::istream& stream, String& s) {
        char buf[2048];
        stream >> buf;
        s = buf;
        return stream;
    }
    String& operator+=(const String& s) {
        size_t current = sz + s.sz;
        char* copy = new char[current];
        memcpy(copy,str, sz);
        memcpy(copy + sz, s.str, s.sz);
        std::swap(copy,str);
        delete[] copy;
        sz+= s.sz;
        return *this;
    }
    String& operator+=(const char s) {
        push_back(s);
        return *this;
    }
    size_t find(const String& substring) const{
        for (size_t i = 0; i < sz - substring.sz;++i) {
            bool flag = true;
            for (size_t j = i; j < substring.sz + i; ++j) {
                if (str[j] != substring.str[j - i]) flag = false;
            }
            if (flag) return i;
        }
        return sz;
    }
    size_t rfind(const String& substring) const     {
        String reversesub(substring.sz);
        String reverse(sz);
        for (size_t i = substring.sz; i > 0; --i) {
            reversesub[substring.sz - i] = substring[i - 1];
        }
        for (size_t i = sz; i > 0; --i) {
            reverse[sz - i] = str[i - 1];
        }
        return sz - reverse.find(reversesub) - substring.sz;
    }
};
String operator+(const String& a, const String& b) {
    String copy = a;
    copy += b;
    return copy;
}
String operator+(const String& a, const char b) {
    String copy = a;
    copy += b;
    return copy;
}
String operator+(const char b, const String& a) {
    String copy;
    copy += b;
    copy += a;
    return copy;
}