#include <iostream>
#include <string>
class Tree {
private:
    struct Node{
        long long value;
        int balance;
        Node* leftChild;
        Node* rightChild;
        Node(long long x): value(x), balance(1), leftChild(nullptr),rightChild(nullptr) {}
    };
    Node* root = nullptr;
public:
    int balance(Node* current) {
        return current?current->balance:0;
    }
    int difference(Node* current) {
        return balance(current->rightChild) - balance(current->leftChild);
    }
    void fixBalance(Node* current) {
        int left = balance(current->leftChild);
        int right = balance(current->rightChild);
       current->balance = (left>right?left:right) + 1;
        //current->balance = !(left + right)*2;
        //current->balance = abs(left - right) + 1;
    }
    void insert(long long x) {
        if (exists(x)) return;
        root = insert(x, root);
    }
    Node* rotateRight (Node* current) {
        Node* daughter = current->leftChild;
        current->leftChild = daughter->rightChild;
        daughter->rightChild = current;
        fixBalance(current);
        fixBalance(daughter);
        return daughter;
    }
    Node* rotateLeft(Node* current) {
        Node* daughter = current->rightChild;
        current->rightChild = daughter->leftChild;
        daughter->leftChild = current;
        fixBalance(current);
        fixBalance(daughter);
        return daughter;
    }
    Node* balancing(Node* current)
    {
        fixBalance(current);
        if( difference(current) == 2){
            if( difference(current->rightChild) < 0 )
                current->rightChild = rotateRight(current->rightChild);
            return rotateLeft(current);
        }
        if( difference(current) == -2 ) {
            if( difference(current->leftChild) > 0  )
                current->leftChild = rotateLeft(current->leftChild);
            return rotateRight(current);
        }
        return current;
    }
    Node* insert(long long x, Node* current) {
        if (!current) {
            return new Node(x);
        }
        if (x < current->value) {
            current->leftChild = insert(x, current->leftChild);
        }
        if ( x > current-> value) {
            current->rightChild = insert(x, current->rightChild);
        }
        return balancing(current);
    }
    void remove(long long x) {
        root = remove(x,root);
    }
    Node* remove(long long x, Node* current) {
        if (!current) {
            return nullptr;
        }
        if (x < current->value) {
            current->leftChild = remove(x, current->leftChild);
        }
        else if ( x > current-> value) {
            current->rightChild = remove(x, current->rightChild);
        }
        else {
            Node* rightDaughter = current->rightChild;
            Node* leftDaughter = current->leftChild;
            delete current;
            if(!leftDaughter) return rightDaughter;
            Node* replacement = leftDaughter;
            while(replacement->rightChild) {
                replacement = replacement->rightChild;
            }
            replacement->leftChild = removeMax(leftDaughter);
            replacement->rightChild = rightDaughter;
            return balancing(replacement);
        }
        return balancing(current);
    }
    Node* removeMax(Node* current) {
        if(!current->rightChild) {
            return current->leftChild;
        }
        current->rightChild = removeMax(current->rightChild);
        return balancing(current);
    }
    Node* exists(long long x) {
        if (root) {
            return exists(x,root);
        }
        return nullptr;
    }
    Node* exists (long long x, Node* current) {
        if (x == current->value) {
            return current;
        }
        if (x < current->value && current->leftChild) {
            return exists(x, current->leftChild);
        }
        if (x > current->value && current->rightChild) {
            return exists(x, current->rightChild);
        }
        return nullptr;
    }
    long long next(long long x) {
        Node* current = root;
        Node* mother = nullptr;
        while (current) {
            if (current->value > x) {
                mother = current;
                current = current->leftChild;
            }
            else {
                current  = current->rightChild;
            }
        }
        return mother?mother->value:x;
    }
    long long prev(long long x) {
        Node* current = root;
        Node* mother = nullptr;
        while (current) {
            if (current->value < x) {
                mother = current;
                current = current->rightChild;
            }
            else {
                current  = current->leftChild;
            }
        }
        return mother?mother->value:x;
    }
};
int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    Tree AVL;
    std::string request;
    while(std::cin >>request) {
        long long x;
        std::cin >> x;
        if (request == "insert") {
            AVL.insert(x);
        }
        if (request == "exists") {
            if (AVL.exists(x)) {
                std::cout << "true" << '\n';
            }
            else {
                std::cout << "false" << '\n';
            }
        }
        if (request == "delete") {
            AVL.remove(x);
        }
        if (request == "next") {
            long long current = AVL.next(x);
            if (current == x) {
                std::cout << "none" << '\n';
            } else {
                std::cout << current << '\n';
            }
        }
        if (request == "prev") {
            long long current = AVL.prev(x);
            if (current == x) {
                std::cout << "none" << '\n';
            } else {
                std::cout << current << '\n';
            }
        }
    }
    return 0;
}