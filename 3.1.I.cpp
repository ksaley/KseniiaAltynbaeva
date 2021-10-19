#include <iostream>
#include <string>
#include <vector>

std::vector<int> z_function(const std::string &string) {
    int size = string.size();
    std::vector<int> z(size, 0);
    int left = -1, right = -1;
    for (int i = 1; i < size; ++i) {
        if (left <= i && i <= right) {
            z[i] = std::min(z[i - left], right - i + 1);
        }
        while (i + z[i] < size && string[z[i]] == string[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }
    return z;
}

int main() {
    std::string input;
    std::cin >> input;
    auto z = z_function(input);
    for (auto i: z) {
        std::cout << i << " ";
    }
}
