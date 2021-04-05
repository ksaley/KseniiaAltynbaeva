#include <iostream>
#include <vector>
#include <string>
#include <cmath>
 
struct node {
    int variable_name;
    int value = 0;
    node* prev = nullptr;
    node(int s, int value, node* prev):variable_name(s),value(value), prev(prev) {}
    int get_value() {
        return  value;
    }
};
 
struct list{
    node* last = nullptr;
 
    void change_value(node* current, int prev, int value) {
        if(!current) {
            node *next = new node(prev, value, last);
            last = next;
        }
        else {
            current->value = value;
        }
    }
 
    node* find(int variable_name) {
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
    const size_t hash_table_size;
    std::vector<list> buckets;
 
public:
    Hash_table(int n):hash_table_size(n), buckets(hash_table_size) {}
 
    int hash_function(int current) {
        const int a = 19;
        const int b = 997;
        const int prime = 65610001;
        return (a * current + b) % prime % hash_table_size;
    }
 
    int swap(int x, int y)  {
        int hash_first = hash_function(x);
        int hash_second = hash_function(y);
        node* first = buckets[hash_first].find(x);
        node* second = buckets[hash_second].find(y);
        int value_first = first?first->get_value():x;
        int value_second = second?second->get_value():y;
        int ans = std::abs(value_first - value_second);
        buckets[hash_first].change_value(first, value_first, value_second);
        buckets[hash_second].change_value(second,value_second, value_first);
        return ans;
    }
};
 
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    Hash_table hashTable(n + 1);
    for (int i = 0; i < n; ++i) {
        int first, second;
        std::cin >> first >> second;
        std::cout << hashTable.swap(first,second) << std::endl;
    }
    return 0;
}
