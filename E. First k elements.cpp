#include <iostream>
#include <vector>
void siftUp(std::vector <long long>& heap, long long i) {
	while (i > 1 && heap[i] > heap[i / 2]) {
		std::swap(heap[i], heap[i / 2]);
		i /= 2;
	}
}
void siftDown(std::vector<long long>& heap) {
	long long i = 1;
	while (2 * i < heap.size()) {
		long long j = 2 * i;
		if (2 * i + 1 < heap.size() && heap[2 * i + 1] > heap[j]) {
			j = 2 * i + 1;
		}
		if (heap[j] <= heap[i]) {
			return;
		}
		std::swap(heap[i], heap[j]);
		i = j;
	}
}

int main() {
	int n, k;
	std::cin >> n >> k;
	std::vector <long long> heap(1);
	heap[0] = 0;
	for (int i = 0; i < k; ++i) {
		long long elem;
		std::cin >> elem;
		heap.push_back(elem);
		siftUp(heap, heap.size() - 1);
	}
	for (int i = 0; i < n - k; ++i) {
		long long elem;
		std::cin >> elem;
		if (elem < heap[1]) {
			heap[1] = elem;
			siftDown(heap);
		}
	}
	std::vector <long long> ans(k);
	for (int i = k - 1; i >= 0; --i) {
		ans[i] = heap[1];
		heap[1] = heap[heap.size() - 1];
		heap.pop_back();
		siftDown(heap);
	}
	for (int i = 0; i < k; ++i) {
		std::cout << ans[i] << " ";
	}
	return 0;
}