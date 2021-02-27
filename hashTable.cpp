#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#pragma GCC optimize("Ofast")
 
struct node {
    std::string key;
    std::string value;
    node* prev;
    node(const std::string& s, const std::string& value, node* prev):key(s),value(value), prev(prev) {}
};
 
struct list{
    node* last = nullptr;
    void push(const std::string& key, const std::string& value) {
        node* current = find(key);
        if(!current) {
            node *next = new node(key, value, last); // не освобождается память для всего что не удалилось, как и в предыдущих задачах
            last = next;
        }
        else {
            current->value = value;
        }
 
    }
    void extract(const std::string& key) {
        node* current = last;
        node* prev = nullptr;
        while(current != nullptr && key != current->key) {
            prev = current;
            current = current->prev;
        }
        if(current) {
           if (prev) prev->prev = current->prev;
           else last = current->prev;
           delete current;
        }
    }
 
    void get(const std::string& key) {
        node* current = find(key);
        if(current) std::cout << current->value << std::endl; // результат выводится не из main
        else std::cout << "none" << std::endl;
    }
 
    node* find(const std::string& key) {
        node* current = last;
        while(current != nullptr && key != current->key) {
            current = current->prev;
        }
        return current;
    }
};
 
class Hash_table {
private:
    static const size_t hash_table_size = 60000001ull;
    std::vector<list> buckets;
    const unsigned long long prime =  1e8 + 7;//65610001ull;
    unsigned long long a, b;
 
    unsigned long long hash_function(const std::string& variable_name) {
        return ((a * string_to_int(variable_name) + b) % prime % hash_table_size);
    }
 
    unsigned long long string_to_int(const std::string& variable_name) {
        unsigned long long sum = 0;
        unsigned long long x = 1;
        const unsigned long long deg = 997;
        for(size_t i = 0; i < variable_name.size(); ++i) {
            sum = (sum + variable_name[i] * x);
            x *= deg;
            x %= prime;
        }
        return sum % prime;
    }
 
public:
    Hash_table(): buckets(hash_table_size){
        std::mt19937 engine;
        engine.seed(std::time(nullptr));
        a = engine() % hash_table_size;
        b = engine() % hash_table_size;
        //std::cout << a << " "<< b;
    }
    void insert(const std::string& variable_name, const std::string& value) {
        buckets[hash_function(variable_name)].push(variable_name,value);
    }
    void extract(const std::string& variable_name) {
        buckets[hash_function(variable_name)].extract(variable_name);
    }
    void get(const std::string& variable_name) {
        buckets[hash_function(variable_name)].get(variable_name);
    }
};
 
int main() {
    freopen("map.in", "r", stdin);
    freopen("map.out", "w", stdout);
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    Hash_table hashTable;
    std::string s;
    while(std::cin >> s) {
        std::string x;
        std::cin >> x;
        if (s == "put") {
            std::string y;
            std::cin >> y;
            hashTable.insert(x,y);
        }
        if (s == "delete") { // лучше сделать else if, чтобы лишний раз не сравнивать строки
            hashTable.extract(x);
        }
        if ( s == "get") {
            hashTable.get(x);
        }
    }
    return 0;
}
