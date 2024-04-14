#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maximum_capital(vector<pair<int, int>>& price_gain, int N, int C) {
    vector<tuple<int, int, int>> laptops;
    for (const auto& p_g : price_gain) {
        int price = p_g.first;
        int gains = p_g.second;
        int profit_margin = gains - price;
        laptops.emplace_back(profit_margin, price, gains);
    }
    sort(laptops.begin(), laptops.end(), [](const tuple<int, int, int>& a, const tuple<int, int, int>& b) {
        return get<0>(a) > get<0>(b);
    });

    int total_capital = C;
    int max_profit = 0;

    for (const auto& laptop : laptops) {
        int profit_margin = get<0>(laptop);
        int laptop_price = get<1>(laptop);
        int laptop_gains = get<2>(laptop);

        if (total_capital >= laptop_price) {
            total_capital += laptop_gains - laptop_price;
            max_profit += laptop_gains;
            N--;

            if (N == 0) {
                break;
            }
        }
    }

    return total_capital;
}

int main() {
    int k, capital, n;
    cin >> k >> capital >> n;

    vector<pair<int, int>> price_gain(k);
    for (auto& i : price_gain) {
        cin >> i.first >> i.second;
    }

    int final_capital = maximum_capital(price_gain, n, capital);
    cout << final_capital;

    return 0;
}
