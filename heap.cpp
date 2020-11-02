#include <iostream>
#include <string>
#include <vector>
void siftUp(std::pair <long long, int>* heap, int i, int* pointer) {
	while (i > 1 && heap[i] < heap[(unsigned)i / 2]) {
		std::swap(pointer[heap[i].second], pointer[heap[(unsigned)i / 2].second]);
		std::swap(heap[i], heap[(unsigned)i / 2]);
		i /= 2;
	}
}
void  siftDown(std::pair <long long, int>* heap, int* pointer, int size) {
	long long i = 1;
	while (2 * i < size + 1) {
		long long j = 2 * i;
		if (2 * i + 1 < size + 1 && heap[2 * i + 1] < heap[j]) {
			j = 2 * i + 1;
		}
		if (heap[j] >= heap[i]) {
			return;
		}
		std::swap(pointer[heap[i].second], pointer[heap[j].second]);
		std::swap(heap[i], heap[j]);
		i = j;
	}
}
int main() {
	int q;
	scanf("%d", &q);
	std::pair<long long, int> heap[1000001];
	int size = 0;
	//std::vector <std::pair  < long long, int>> heap(1);
	int pointer[1000001];
	pointer[0] = 0;
	heap[0] = { 0, 0 };
	for (int i = 0; i < q; ++i) {
		pointer[i + 1] = 0;
		char request[15];
		scanf("%s", request);
		if (request[0] == 'i') {
			size++;
			long long x;
			scanf("%lld", &x);
			heap[size].first = x;
			heap[size].second = i + 1;
			pointer[i + 1] = size;
			siftUp(heap, pointer[i + 1], pointer);
		}
		else if (request[0] == 'g') {
			printf("%lld \n ", heap[1].first);
		}
		else if (request[0] == 'e') {
			if (size == 1) {
				size--;
			}
			else {
				heap[1] = heap[size];
				pointer[heap[size].second] = 1;
				heap[size].first = 0;
				heap[size].second = 0;
				size--;
				siftDown(heap, pointer, size);
			}
		}
		else {
			long long number;
			long long diff;
			scanf("%lld" "%lld", &number, &diff);
			heap[pointer[number]].first -= diff;
			siftUp(heap, pointer[number], pointer);
		}
	}
	return 0;
}