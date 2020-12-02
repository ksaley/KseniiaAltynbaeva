#include <iostream>
#include <vector>

int PrefixSum(std::vector <int>& a, int position) { // сумма с 0 до k-той позиции
	int result = 0;
	while (position >= 0) {
		result += a[position];
		position = ((position + 1) & position) - 1;
	}
	return result;
}

void ModifyElement(std::vector <int>& a, int position, int newvalue) {
	int current = PrefixSum(a,position) - PrefixSum(a, position- 1);
	while (position < a.size()) {
		a[position] += newvalue - current;
		position = position | (position + 1);
	}
}

void reading(std::vector <int>& even, std::vector <int>& odd, int sizeOfarray) {
	for (int i = 1; i <= sizeOfarray; ++i) {
		int current;
		std::cin >> current;
		if (i % 2) {
			ModifyElement(odd, i, current);
		}
		else {
			ModifyElement(even, i, current);
		}
	}
}
int getSum(std::vector <int>& even, std::vector <int>& odd) {
	int left, right;
	std::cin >> left >> right;
	int flag = left % 2;
	if (flag) { // если вычитание начинается с нечетных
		return PrefixSum(odd, right) - PrefixSum(odd, left - 1) + PrefixSum(even, left - 1) - PrefixSum(even, right);
	}
	else { // иначе
		return  PrefixSum(odd, left - 1) - PrefixSum(odd, right) - PrefixSum(even, left - 1) + PrefixSum(even, right);
	}
}
void change(std::vector <int>& even, std::vector <int>& odd) {
	int position, newvalue;
	std::cin >> position >> newvalue;
	if (position % 2) { // меняем нечетные в своем массиве
		ModifyElement(odd, position, newvalue);
	}
	else { // и четные в своем
		ModifyElement(even, position, newvalue);
	}
}

int main() {
	int sizeOfarray;
	std::cin >> sizeOfarray;
	std::vector <int> even(sizeOfarray + 1, 0); // храним элементы с четным индексом
	std::vector <int> odd(sizeOfarray + 1, 0); // храним элементы с нечетным индексом 
	reading(even, odd, sizeOfarray); //считывание
	int amountOfrequests;
	std::cin >> amountOfrequests;
	for (int i = 0; i < amountOfrequests; ++i) {
		int operation;
		std::cin >> operation;
		if (operation) { // если 1, то ищем сумму
			std::cout << getSum(even, odd) << std::endl;
		}
		else { // иначе меняем элемент
			change(even, odd); 
		}
	}
	//system("pause");
	return 0;
}
