#include <iostream>
#include <vector>
#include <string>
#include <cmath>

void stableSort(std::vector <long long>& input, int rank) {
	std::vector <int> count(10, 0);
	for (int i = 0; i < input.size(); ++i) {
		count[input[i] / (long long)pow(10, rank) % 10]++;
	}
	for (int i = 1; i < count.size(); ++i) {
		count[i] += count[i - 1];
	}
	std::vector <long long> ans(input);
	for (int i = input.size() - 1; i >= 0; --i) {
		input[--count[ans[i] / (long long)pow(10, rank) % 10]] = ans[i];
	}
}
void LSD(std::vector <long long>& input, long long maxsize) {
	int  n = 0;
	while (maxsize > 0) {
		maxsize /= 10;
		++n;
	}
	for (int i = 0; i < n; ++i) {
		stableSort(input, i);
	}
}
int main() {
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	int n;
	std::cin >> n;
	std::vector <long long> input;
	long long maxsize = -1;
	for (int i = 0; i < n; ++i) {
		long long number;
		std::cin >> number;
		if (number > maxsize) {
			maxsize = number;
		}
		input.push_back(number);
	}
	LSD(input, maxsize);
	for (int i = 0; i < n; ++i) {
		std::cout << input[i] << " ";
	}
	return 0;
}
