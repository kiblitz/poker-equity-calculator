#include <include/ranker.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

ranker::ranking_t l1_rank_of(std::string raw) {
  ranker::hand_t hand{};
  std::istringstream stream(raw);
  unsigned card_on = 0u;
  unsigned rng_suit_on = 5u;
  std::string s;
  while (getline(stream, s, ' ')) {
    if (card_on > 4) {
      throw std::invalid_argument("more than 5 cards provided");
    }
    ranker::Suit suit{};
    if (s.length() == 1 || (s.length() == 2 && s[0] == '1' && s[1] == '0')) {
      suit = static_cast<ranker::Suit>(rng_suit_on++);
    } else if (s.length() == 2 || (s.length() == 3 && s[0] == '1' && s[1] == '0')) {
      switch (s[s.length() - 1]) {
        case 's':
          suit = ranker::Suit::spade;
          break;
        case 'h':
          suit = ranker::Suit::heart;
          break;
        case 'c':
          suit = ranker::Suit::club;
          break;
        case 'd':
          suit = ranker::Suit::diamond;
          break;
        default:
          throw std::invalid_argument("unknown suit provided");
      }
    } else {
      throw std::invalid_argument("each card should be represented by at most 2 characters");
    }
    auto value = (s[0] == 'a' ? 14 : s[0] == 'k' ? 13 : s[0] == 'q' ? 12 : s[0] == 'j' ? 11 : s.length() >= 2 && s[0] == '1' && s[1] == '0' ? 10 : s[0] - '0') - 2;
    if (value > 13) {
      throw std::invalid_argument("card value out of bounds");
    }
    hand[card_on++] = {static_cast<ranker::Card>(value), suit};
  }
  if (card_on != 5) {
    throw std::invalid_argument("less than 5 cards provided");
  }
  return ranker::l1_rank(hand);
} 

void test_l1_rank() {
  const ranker::ranking_t l1_rankings[] = {
    // high card
    l1_rank_of("q k a 2 3"),
    l1_rank_of("3h 4h 6h 7h 8d"),

    // pair
    l1_rank_of("jh jc 2 3 4"),
    l1_rank_of("a a k q j"),

    // two pair
    l1_rank_of("a a 5 5 7"),
    l1_rank_of("7c 7s 8d 8h a"),

    // trips
    l1_rank_of("a 2 7 7 7"),
    l1_rank_of("3h 5h 3s qd 3c"),

    // straight
    l1_rank_of("8 9 10 j q"),
    l1_rank_of("as 2c 3d 4d 5h"),
    l1_rank_of("10h jh qh kd ah"),

    // flush
    l1_rank_of("3h 4h 6h 7h 8h"),
    l1_rank_of("as 2s 4s 5s 6s"),
    l1_rank_of("5d 2d 6d jd ad"),

    // boat
    l1_rank_of("2 2 7 7 7"),
    l1_rank_of("3h 5h 3s 5d 3c"),

    // quads
    l1_rank_of("2 2 2 2 5"),
    l1_rank_of("kh ah as ac ad"),

    // straight flush
    l1_rank_of("as 2s 3s 4s 5s"),
    l1_rank_of("ah kh qh jh 10h")
  };
  assert(std::is_sorted(std::begin(l1_rankings), std::end(l1_rankings)));
}

void run_test(std::string test_name, void (*test)()) {
  std::cout << "TESTING " << test_name << std::endl;
  test();
  std::cout << "PASSED " << test_name << std::endl;
}

int main() {
  run_test("l1_rank", test_l1_rank);
  return 0;
}
