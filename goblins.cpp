#include <iostream>
#include <deque>

using std::cout;
using std::cin;
using std::deque;
void adding(int number, deque <int>& qend) {
	qend.push_back(number);
}
void priveleged(int number, deque<int>& qbegin, deque<int>& qend) {
	while (qbegin.size() < qend.size()) {
		qbegin.push_back(qend.front());
		qend.pop_front();
	}
	qbegin.push_back(number);
	while (qbegin.size() > qend.size() + 1) {
		qend.push_front(qbegin.back());
		qbegin.pop_back();
	}
}
void toshamans(deque<int>& qbegin, deque<int>& qend) {
	if (qbegin.empty()) {
		cout << qend.front() << std::endl;
		qend.pop_front();
	}
	else {
		cout << qbegin.front() << std::endl;
		qbegin.pop_front();
	}
}
int main() {
	std::ios_base::sync_with_stdio(0);
	cin.tie(0);
	int amountOfrequests;
	cin >> amountOfrequests;
	deque <int> qend;
	deque <int> qbegin;
	for (int j = 0; j < amountOfrequests; ++j) {
		char request;
		int number;
		cin >> request;
		if (request == '+') {
			cin >> number;
			adding(number, qend);
		}
		else if (request == '-') {
			toshamans(qbegin, qend);
		}
		else {
			cin >> number;
			priveleged(number, qbegin, qend);
		}
	}
	return 0;
}