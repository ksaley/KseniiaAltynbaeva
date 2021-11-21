#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#define int long long
 
void sort(int k, std::vector<int>& suffmas, std::vector<int>& rang) {
    int n = suffmas.size();
    std::vector<int> temp = suffmas;
    int  maxi = std::max(256ll, n);
    std::vector<int> count(maxi, 0);
    for (int i = 0; i < n; ++i) {
        if (i + k < n) {
            ++count[rang[i + k]];
        }
        else ++count[0];
    }
    int sum = 0;
    for (int i = 0; i < maxi; ++i) {
        int t = count[i];
        count[i] = sum;
        sum += t;
    }
 
    for (int i = 0; i < n; i++) {
        if (suffmas[i] + k < n) {
            temp[count[rang[suffmas[i] + k]]++] = suffmas[i];
        }
        else {
            temp[count[0]++] = suffmas[i];
        }
    }
    suffmas = temp;
}
 
void build(std::string text, std::vector<int> &suffmas) {
    std::iota(suffmas.begin(), suffmas.end(), 0);
    int n = text.size();
    std::vector<int> rang(n), temp(n);
    for (int i = 0; i < n; ++i) {
        rang[i] = text[i];
    }
    for (int k = 1; k < n; k <<= 1) {
        sort(k, suffmas, rang);
        sort(0, suffmas, rang);
        int r = temp[suffmas[0]] = 0;
        for (int i = 1; i < n; ++i) {
            if (rang[suffmas[i]] == rang[suffmas[i - 1]] &&
                rang[suffmas[i] + k] == rang[suffmas[i - 1] + k]) {
                temp[suffmas[i]] = r;
            }
            else {
                temp[suffmas[i]] = ++r;
            }
        }
        rang = temp;
    }
}
 
int32_t main() {
    std::string text;
    std::cin >> text;
    text.push_back('#');
    std::vector<int> suffmas(text.size());
    build(text, suffmas);
    for (int i = 1; i < text.size(); ++i)
        std::cout << suffmas[i] + 1 << " ";
    return 0;
}
