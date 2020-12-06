#include <iostream>
#include <vector>

int partition(std::vector <long long>& input, int begin, int end) {
	int result = begin - 1;
	long long x = rand() % (end - begin) + begin;
	for (int i = begin; i < end; ++i) {
		if (input[i] < input[x]) {
			++result;
			if (result == x) {
				x = i;
			}
			std::swap(input[result], input[i]);
		}
	}
	std::swap(input[result + 1], input[x]);
	return result + 1;
}

int kth(std::vector <long long>& input, int k) {
	int left = 0, right = input.size();
	while (true) {
		int mid = partition(input, left, right);
		if (mid == k) {
			return input[mid];
		}
		else if (k < mid) {
			right = mid;
		}
		else {
			left = mid + 1;
		}
	}
}

int main() {
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	int k, n;
	std::cin >> n >> k;
	std::vector <long long> input;
	for (int i = 0; i < n; ++i) {
		long long x;
		std::cin >> x;
		input.push_back(x);
	}
	std::cout << kth(input, k);
	return 0;
}	
