#include <iostream>
#include <deque>

using std::cout;
using std::cin;
using std::deque;

int main() {
	std::ios_base::sync_with_stdio(0);
	cin.tie(0);
	int n;
	cin >> n;
	deque <int> qend;
	deque <int> qbegin;
	for (int j = 0; j < n; ++j) {
		char k;
		int i;
		cin >> k;
		if (k == '+') {
			cin >> i;
			qend.push_back(i);
		}
		else if (k == '-') {
			if (qbegin.empty()) {
				cout << qend.front() << std::endl;
				qend.pop_front();
			}
			else {
				cout << qbegin.front() << std::endl;
				qbegin.pop_front();
			}
		}
		else {
			cin >> i;
			while (qbegin.size() < qend.size()) {
				qbegin.push_back(qend.front());
				qend.pop_front();
			}
			qbegin.push_back(i);
			while (qbegin.size() > qend.size() + 1) {
				qend.push_front(qbegin.back());
				qbegin.pop_back();
			}
		}
	}
	return 0;
}