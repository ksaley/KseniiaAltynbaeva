#include <iostream>
#include <vector>	
#include <fstream>

using std::vector;
using std::cin;
using std::cout;

long long mergeSort(vector<int>& a) {
	int n = a.size();
	long long ans = 0;
	if (n > 1) {
		vector <int> first;
		vector <int> second;
		int mid = n / 2;
		for (int i = 0; i < mid; ++i) {
			first.push_back(a[i]);
		}
		for (int i = mid; i < n; ++i) {
			second.push_back(a[i]);
		}
		ans = mergeSort(first) + mergeSort(second);
		int elemfirst = 0, elemsecond = 0;
		for (int i = 0; i < n; ++i) {
			if ((elemsecond >= second.size()) || elemfirst < first.size() && first[elemfirst] <= second[elemsecond]) {
				a[i] = (first[elemfirst]);
				elemfirst++;
			}
			else if ((elemfirst >= first.size()) || elemsecond < second.size() && first[elemfirst] >= second[elemsecond]) {
				a[i] = (second[elemsecond]);
				elemsecond++;
				ans += first.size() - elemfirst;
			}
		}
	}
	return ans;
}
int main() {
	std::ifstream in("inverse.in");
	int n;
	in >> n;
	vector <int> input;
	for (int i = 0; i < n; ++i) {
		int y;
		in >> y;
		input.push_back(y);
	}
	std::ofstream out("inverse.out");
	out << mergeSort(input);
	return 0;
}	