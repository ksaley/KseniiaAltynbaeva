#include <iostream>
#include <cstdlib>
#include <string>

struct node {
    long long x;
    long long y;
    int size;
    node* left;
    node* right;
    
    node(long long val) {
        x = val;
        y = rand();
        size = 1;
        left = nullptr;
        right = nullptr;
    }
    ~node() {
        if (left) delete left;
        if (right) delete right;
    } 
};

int get_size(node* root) {
    if (root) return root->size;
    else return 0;
}
void update(node* root) {
    if (!root) return;
    root->size = 1 +get_size(root->left) + get_size(root->right);
}
bool exists (node* root, long long key) {
    if (root == nullptr)
        return false;
    if (key == root->x)
        return true;
    if (key > root-> x)
        return exists(root->right, key);
    if (key < root->x)
        return exists(root->left, key);
}
std::pair<node*, node*> split (node* root, long long val) {
    if (root == nullptr)
        return {nullptr, nullptr};
    if (root-> x <= val) {
        auto res = split(root->right, val);
        root->right = res.first;
        update(root);
        return {root, res.second};
    }
    else {
        auto res = split (root->left, val);
        root->left = res.second;
        update(root);
        return {res.first, root};
    }
}
node* merge(node* root1, node* root2){
    if (root1 == nullptr) return root2;
    if (root2 == nullptr) return root1;
    if (root1->y <= root2->y) {
        root1->right = merge(root1->right, root2);
        update(root1);
        return root1;
    }
    else {
        root2->left = merge(root1, root2->left);
        update(root2);
        return root2;
    }
}
node* insert(node* root, long long val) {
    if (exists(root,val)) return root;
    auto res = split(root, val);
    node* newnode = new node(val);
    return merge(merge(res.first,newnode), res.second);
}
node* erase(node* root, long long val) {
    if (!exists(root, val))
        return root;
    auto res1 = split(root, val);
    auto res2 = split(res1.first, val - 1);
    delete res2.second;
    return merge(res2.first, res1.second);
}
 
long long order(node* root, long long k) {
    if (k<=get_size(root->left))
        return order(root->left,k);
    if (k == get_size(root->left) + 1)
        return root->x;
    return order(root->right, k-get_size(root->left) - 1);
}
int main() {
    int n;
    std::cin >> n;
    node *root = nullptr;
    std::string request;
    long long x;
    for (int i = 0; i < n; i++) {
        std::cin >> request >> x;
        if (request == "-1") {
            root = erase(root, x);
        }
        else if (request == "0") {
            std::cout << order(root, root->size - x + 1) <<std::endl;
        }
        else {
            root = insert(root, x);
        }
    }
    return 0;
}
