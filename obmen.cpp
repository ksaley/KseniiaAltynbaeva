#include <iostream>
#include <vector>
#include <string>
#include <cmath>
 
struct node {
    int variable_name;
    int value = 0;
    node* prev = nullptr;
    node(int s, int value, node* prev):variable_name(s),value(value), prev(prev) {}
    int get_value(int x) {
        return  this? this->value:x; // какая-то странная конструкция, если указатель на текущий объект = 0, то уже при вызове его метода get_value будет исключение
    }
};
 
struct list{
    node* last = nullptr;
 
    void change_value(node* current, int prev, int value) {
        if(!current) {
            node *next = new node(prev, value, last); // не освобождается память
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
 
    void swap(int x, int y)  {
        int hash_first = hash_function(x);
        int hash_second = hash_function(y);
        node* first = buckets[hash_first].find(x);
        node* second = buckets[hash_second].find(y);
        int value_first = first->get_value(x);
        int value_second = second->get_value(y);
        std::cout << std::abs(value_first - value_second) << std::endl;
        buckets[hash_first].change_value(first, value_first, value_second);
        buckets[hash_second].change_value(second,value_second, value_first);
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
        hashTable.swap(first,second);
    }
    return 0;
}
