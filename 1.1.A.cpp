#include <iostream>
#include <string>


struct Node{
	int value;
	Node* previous;
	Node() : value(0), previous(nullptr) {}
};

class Stack {
private:
	size_t size;
	Node* last;
public:
	Stack() : size(0), last(nullptr) {}
	
	size_t length() {
		return size;
	}
	
	void push(int x) {
		Node* next = new Node();
		next->value = x;
		next->previous = last;
		++size;
		last = next;
	}
	
	int back() {
		return last->value;
	}
	
	int pop() {
		int result = last->value;
		Node* copy = last;
		last = last->previous;
		delete copy;
		--size;
		return result;
	}
	
	void clear() {
		while (size > 0) {
			pop();
		}
	}
};
int main() {
	std::string operation;
	std::cin >> operation;
	Stack stack;
	while (operation != "exit") {
		if (operation == "push") {
			int n;
			std::cin >> n;
			stack.push(n);
			std::cout << "ok" << '\n';
		}
		if (operation == "pop") {
			if (stack.length() > 0) std::cout << stack.pop() << '\n';
			else std::cout << "error" << '\n';
		}
		if (operation == "back") {
			if (stack.length() > 0) std::cout << stack.back() << '\n';
			else  std::cout << "error" << '\n';

		}
		if (operation == "size") {
			std::cout << stack.length() << '\n';
		}
		if (operation == "clear") {
			stack.clear();
			std::cout << "ok" << '\n';
		}
	std::cin >> operation;
 	}
	std::cout << "bye" << '\n';
	return 0;
}
