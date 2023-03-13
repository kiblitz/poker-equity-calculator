#include <include/ranker.hpp>

#include <chrono> 
#include <iostream>
#include <string>
#include <vector>

template <typename T> struct Duration;

template <> struct Duration<std::chrono::milliseconds> {
	static const char* unit() {
			return "ms";
	}
};

template <> struct Duration<std::chrono::seconds> {
	static const char* unit() {
			return "s";
	}
};

std::chrono::milliseconds bench_l1_rank() {
  std::vector<ranker::card_t> cards{};
  cards.reserve(52);
  for (unsigned value = 0u; value < 13; ++value) {
    for (unsigned suit = 0u; suit < 4; ++suit) {
      cards.push_back({static_cast<ranker::Card>(value), static_cast<ranker::Suit>(suit)});
    }
  }

  std::vector<ranker::hand_t> hands{};
  hands.reserve(2598960);
  for (unsigned i = 0u; i < 52; ++i) {
    for (unsigned j = i + 1; j < 52; ++j) {
      for (unsigned k = j + 1; k < 52; ++k) {
        for (unsigned l = k + 1; l < 52; ++l) {
          for (unsigned m = l + 1; m < 52; ++m) {
            hands.push_back({cards[i], cards[j], cards[k], cards[l], cards[m]});
          }
        }
      }
    }
  }

  auto start = std::chrono::high_resolution_clock::now();
  for (const auto &hand : hands) {
    ranker::l1_rank(hand);
  }
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

template<typename T>
void run_bench(std::string bench_name, T (*bench)()) {
  std::cout << "BENCHING " << bench_name << std::endl;
  auto elapsed = bench();
  std::cout << "TIME " << elapsed.count() << (Duration<T>::unit()) << std::endl;
}

int main() {
  run_bench("l1_rank", bench_l1_rank);
  return 0;
}
