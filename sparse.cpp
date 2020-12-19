#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
 
 void build_sparse_table(std::vector <std::vector<std::pair <int, int>>>& f, int n) {
     for (int k = 0; (1 << (k + 1)) <= n; ++k) {
		for (int i = 0; i < n; ++i) {
			f[k + 1][i].second = std::min(std::max(f[k][i].first, f[k][i + (1 << k)].first), std::min(f[k][i].second, f[k][i + (1 << k)].second));
			f[k + 1][i].first = std::min(f[k][i].first, f[k][i + (1 << k)].first);
		}
	}
 }
 
 void build_degree(std::vector <int>& degree, int n) {
     	for (int i = 2; i <= n; ++i) {
		degree[i] = degree[i >> 1] + 1;
	}
 }
 
int main() {
	const int max = 1000000000;
	int size_of_array, amount_of_requests;
	std::cin >> size_of_array >> amount_of_requests;
	std::vector <std::vector<std::pair <int, int>>> array(std::ceil(log2l(size_of_array)), std::vector<std::pair<int, int>>(size_of_array + std::ceil(log2l(size_of_array))));
	for (int i = 0; i < size_of_array; ++i) {
		std::cin >> array[0][i].first;
		array[0][i].second = max;
	}
	
	build_sparse_table(array, size_of_array);
	std::vector <int> degree(size_of_array + 1);
    build_degree(degree, size_of_array);
    
	for (int i = 0; i < amount_of_requests; ++i) {
		int left, right;
		std::cin >> left >> right;
		--left;
		--right;
		int k = degree[right - left + 1];
			if (array[k][left].first != array[k][right - (1 << k) + 1].first) {
				std::cout << std::min(std::max(array[k][left].first, array[k][right - (1 << k) + 1].first), std::min(array[k][left].second, array[k][right - (1 << k) + 1].second));
				std::cout << std::endl;
			}
			else {
				std::cout << std::min(array[k][left].second, array[k][right - (1 << k) + 1].second) << std::endl;
			}
	}
	
	return 0;
}
