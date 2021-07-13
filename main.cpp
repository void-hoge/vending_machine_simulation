#include <iostream>
#include <array>
#include <random>
#include <cmath>
#include <vector>

using coin_t = int;
const coin_t coin_1000 = 0;
const coin_t coin_500 = 1;
const coin_t coin_100 = 2;
const coin_t coin_50 = 3;
const coin_t coin_10 = 4;
// const coin_t coin_5 = 5;
// const coin_t coin_1 = 6;

const std::array<int, 5> coin_value = {1000, 500, 100, 50, 10};
const std::vector<int> products = {
	190, 140, 130, 160, 160, 140, 140, 150, 140, 130,
	110, 110, 130, 130, 130, 130, 130, 130, 130, 210,
	120, 130, 130, 130, 130, 130, 130, 130, 130, 140
};
// http://www.hachiyoh.co.jp/products/suntory.html

static std::mt19937
// rnd(time(NULL));
rnd(1);
static std::uniform_real_distribution<double> real_rand(0.0, 1.0);

class customer {
private:
	std::array<double, 4> exchange_prob;
public:
	int value;
	std::array<coin_t, 5> coin_set;
	customer (std::array<double, 4> ecp, int min, int max)  {
		exchange_prob = ecp;
		value = std::abs((int)rnd()%(max-min))+min;
		value -= value%coin_value.at(coin_10);
		min_coin();
		exchange();
	}
	void min_coin() {
		int tmp = value;
		for (coin_t i = coin_1000; i <= coin_10; i++) {
			coin_set.at(i) = tmp/coin_value.at(i);
			tmp %= coin_value.at(i);
		}
	}
	void exchange() {
		for (coin_t i = coin_1000; i < coin_10; i++) {
			if (coin_set.at(i) == 0) {
				continue;
			}
			if (real_rand(rnd) < exchange_prob.at(i)) {
				coin_set.at(i)--;
				coin_set.at(i+1) += coin_value.at(i)/coin_value.at(i+1);
			}
		}
	}
	std::array<coin_t, 5> pay(const int v) {
		std::cerr << value << ": ";
		for (auto a: coin_set) std::cerr << a << " "; std::cerr << '\n';
		int tmp = value-v;
		std::array<coin_t, 5> result = coin_set;
		// 財布の中身全部出して釣りをもらう戦略
		for (coin_t i = coin_1000; i <= coin_10; i++) {
			int t = tmp/coin_value.at(i);
			result.at(i) -= t;
			tmp -= t*coin_value.at(i);
			coin_set.at(i) = t;
		}
		recalculate_sum();
		if (value < 0) {
			throw std::logic_error("Payment must be less than or equal to the total of the wallet.");
		}
		std::cerr << v << ": ";
		for (auto a: result) std::cerr << a << " "; std::cerr << '\n';
		return result;
	}
	void dump() {
		std::cout << value << '\n';
		std::cout << 1000 << ": " << coin_set.at(coin_1000) << '\n';
		std::cout << 500 << ": " << coin_set.at(coin_500) << '\n';
		std::cout << 100 << ": " << coin_set.at(coin_100) << '\n';
		std::cout << 50 << ": " << coin_set.at(coin_50) << '\n';
		std::cout << 10 << ": " << coin_set.at(coin_10) << '\n';
	}
	void recalculate_sum() {
		value = 0;
		for (coin_t i = coin_1000; i <= coin_10; i++) {
			value += coin_set.at(i)*coin_value.at(i);
		}
	}
};

class cashier {
public:
	std::array<coin_t, 5> used_coins;
	cashier() {
		for (auto& a: used_coins) {
			a = 0;
		}
	}
	void add(const std::array<coin_t, 5> p) {
		for (coin_t i = coin_1000; i <= coin_10; i++) {
			used_coins.at(i) += p.at(i);
		}
	}
	void dump() {
		for (coin_t i = coin_1000; i < coin_10; i++) {
			std::cout << used_coins.at(i) << ", ";
		}
		std::cout << used_coins.at(coin_10) << '\n';
	}
};

int main() {
	cashier hoge;
	for (int i = 0; i < 1000; i++) {
		customer tmp((std::array<double, 4>){0,0,0,0}, 1000, 3000);
		hoge.add(tmp.pay(products.at(rnd()%products.size())));
		hoge.dump();
	}
	return 0;
}
