#include <iostream>
#include <map>
#include <set>
#include <string>

// Зачем превращать глаголы в существительные. Можно было бы так и назвать add_element; delete_element; clear_set
void addition(std::map <int, std::set <unsigned long long>>& listofsets, 
std::map <unsigned long long, std::set <int>>& gdelezhitE) {// gdelezitE :)))))
	unsigned long long e;
	int s;
	std::cin >> e >> s;
	listofsets[s].insert(e);
	gdelezhitE[e].insert(s);
}

void deletion(std::map <int, std::set <unsigned long long>>& listofsets,
std::map <unsigned long long, std::set <int>>& gdelezhitE) {
	unsigned long long e;
	int s;
	std::cin >> e >> s;
	listofsets[s].extract(e);
	gdelezhitE[e].extract(s);
} 

void purification(std::map <int, std::set <unsigned long long>>& listofsets,
std::map <unsigned long long, std::set <int>>& gdelezhitE) {
	int s;
	std::cin >> s;
	for (auto now : listofsets[s]) {
		gdelezhitE[now].extract(s);
	}
	listofsets[s].clear();
}

void listset(std::map <int, std::set <unsigned long long>>& listofsets) {
	int s;
	std::cin >> s;
	if (!listofsets[s].empty())
		for (auto now : listofsets[s]) {
			std::cout << now << ' ';
		}
	else {
		std::cout << -1;
	}
	std::cout << std::endl;
}

void listsetof(std::map <unsigned long long, std::set <int>>& gdelezhitE) {
	unsigned long long e;
	std::cin >> e;
	if (!gdelezhitE[e].empty()) {
		for (auto now : gdelezhitE[e]) {
			std::cout << now << ' ';
		}
	}
	else {
		std::cout << -1;
	}
	std::cout << std::endl;
}

int main() {
	unsigned long long n;
	int m, k;
	std::cin >> n >> m >> k;
	std::map <int, std::set <unsigned long long>> listofsets;// использовать эту структуру в рамках задачи довольно избыточно
	//так как там явно указано количество множеств и можно было бы использовать обычный массив. А для защиты от создания ненужных множеств можно хранить ссылки на элементы
	std::map <unsigned long long, std::set <int>> gdelezhitE;
	for (int i = 0; i < k; ++i) {
		std::string request;
		std::cin >> request;
		if (request == "ADD") {
			addition(listofsets, gdelezhitE);
		}
		if (request == "DELETE") {
			deletion(listofsets, gdelezhitE);
		}
		if (request == "CLEAR") {
			purification(listofsets, gdelezhitE);
		}
		if (request == "LISTSET") {
			listset(listofsets);
		}
		if (request == "LISTSETSOF") {
			listsetof(gdelezhitE);
		}
	}
		return 0;
}
