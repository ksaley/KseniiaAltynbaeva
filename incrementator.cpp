#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#pragma GCC optimize("Ofast")

struct node {
    std::string variable_name;
    int value;
    node* prev;
    node(const std::string& s, int value, node* prev):variable_name(s),value(value), prev(prev) {}
};

struct list{
    node* last = nullptr;
    int push(const std::string& variable_name, int add) {
        node* current = find(variable_name);
        if(!current) {
            node *next = new node(variable_name, add, last);
            last = next;
            return next->value;
        }
        else {
            current->value += add;
            return current->value;
        }
    }
    node* find(const std::string& variable_name) {
        node* current = last;
        while(current != nullptr && variable_name != current->variable_name) {
            current = current->prev;
        }
        return current;
    }
    ~list() {
        while (last) {
            node* copy = last;
            last = last->prev;
            delete copy;
        }
    }
};

class Hash_table {
private:
    static const size_t hash_table_size = 60000001ull;
    std::vector<list> buckets;
    const unsigned long long prime =  1e8 + 7;
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
    }
    int insert(const std::string& variable_name, int add) {
        return buckets[hash_function(variable_name)].push(variable_name,add);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    Hash_table hashTable;
    std::string s;
    while(std::cin >> s) {
        int n;
        std::cin >> n;
        std::cout << hashTable.insert(s,n) << std::endl;
    }
    return 0;
}
