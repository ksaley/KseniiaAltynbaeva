#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#pragma GCC optimize("Ofast")
 
struct node {
    std::pair<int, int> coordinates;
    node* prev;
    node(std::pair <int,int> input, node* prev):coordinates(input), prev(prev) {}
}; 
 
struct list{
    node* last = nullptr;
    bool push(std::pair <int,int> input) {
        node* current = find(input);
        if(!current) {
            node *next = new node(input, last);
            last = next;
            return true;
        }
        return false;
    }
    node* find(std::pair <int,int> input) {
        node* current = last;
        while(current != nullptr && input != current->coordinates) {
            current = current->prev;
        }
        return current;
    }
};
 
class Hash_table {
private:
    static const size_t hash_table_size = 40000003;
    std::vector<list> buckets;
    const unsigned long long prime =  99990001;//65610001ull;
    unsigned long long a, b;
 
    unsigned long long hash_function(std::pair <int, int> current) {
        return ((a * string_to_int(current) + b) % prime % hash_table_size);
    }
 
    unsigned long long string_to_int(std::pair <int, int> current) {
        const unsigned long long deg = 10e8 + 3;
        return (current.first + current.second * deg) % prime;
    }
 
public:
    size_t current_quantity = 0;
    Hash_table(): buckets(hash_table_size){
        std::mt19937 engine;
        engine.seed(std::time(nullptr));
        a = engine() % hash_table_size;
        b = engine() % hash_table_size;
        //std::cout << a << " "<< b;
    }
    void insert(std::pair<int, int> current) {
        current_quantity += buckets[hash_function(current)].push(current);
    }
};
std::vector <std::pair<int,int>> blot_coordinates(int x, int y, int w, int h) {
    std::vector <std::pair<int,int>>  coord(5);
    coord[0] = {x,y};
    coord[1] = {x-1, y};
    coord[2] = {x,y-1};
    coord[3] = {x+1,y};
    coord[4] = {x, y+1};
    return coord;
}
 
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned long long width, height, n;
    int sum = 0;
    std::cin >> width >> height >> n;
    if ( width * height > 5 * n) {
        std::cout << "No";
        return 0;
    }
    std::vector<std::vector<int>> hashTable(width + 1,std::vector<int>(height + 1, 0));
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        for ( const auto& j : blot_coordinates(x,y, width, height)) {
            if (j.first > 0 && j.first <= width && j.second > 0 && j.second <= height)
                if (!hashTable[j.first][j.second]) {
                    hashTable[j.first][j.second] = 1;
                    sum++;
                }
        }
    }
    if(sum >= height * width) std::cout << "Yes";
    else std::cout << "No";
    return 0;
}
