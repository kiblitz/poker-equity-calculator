#include <include/ranker.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

std::unique_ptr<ranker::hand_t> to_hand(std::string raw) {
  std::unique_ptr<ranker::hand_t> hand(new ranker::hand_t);
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
    (*hand)[card_on++] = {static_cast<ranker::Card>(value), suit};
  }
  if (card_on != 5) {
    throw std::invalid_argument("less than 5 cards provided");
  }
  return hand;
} 

void test_l1_rank() {
  const std::vector<std::vector<ranker::ranking_t>> l1_rankings = {
    // high card
    {
      ranker::l1_rank(*to_hand("q k a 2 3")),
      ranker::l1_rank(*to_hand("3h 4h 6h 7h 8d"))
    },

    // pair
    {
      ranker::l1_rank(*to_hand("jh jc 2 3 4")),
      ranker::l1_rank(*to_hand("a a k q j"))
    },

    // two pair
    {
      ranker::l1_rank(*to_hand("a a 5 5 7")),
      ranker::l1_rank(*to_hand("7c 7s 8d 8h a"))
    },

    // trips
    {
      ranker::l1_rank(*to_hand("a 2 7 7 7")),
      ranker::l1_rank(*to_hand("3h 5h 3s qd 3c"))
    },

    // straight
    {
      ranker::l1_rank(*to_hand("8 9 10 j q")),
      ranker::l1_rank(*to_hand("as 2c 3d 4d 5h")),
      ranker::l1_rank(*to_hand("10h jh qh kd ah"))
    },

    // flush
    {
      ranker::l1_rank(*to_hand("3h 4h 6h 7h 8h")),
      ranker::l1_rank(*to_hand("as 2s 4s 5s 6s")),
      ranker::l1_rank(*to_hand("5d 2d 6d jd ad"))
    },

    // boat
    {
      ranker::l1_rank(*to_hand("2 2 7 7 7")),
      ranker::l1_rank(*to_hand("3h 5h 3s 5d 3c"))
    },

    // quads
    {
      ranker::l1_rank(*to_hand("2 2 2 2 5")),
      ranker::l1_rank(*to_hand("kh ah as ac ad"))
    },

    // straight flush
    {
      ranker::l1_rank(*to_hand("as 2s 3s 4s 5s")),
      ranker::l1_rank(*to_hand("ah kh qh jh 10h"))
    }
  };

  // high card
  assert(std::equal(l1_rankings[0].begin(), l1_rankings[0].end(), l1_rankings[0].begin()));

  // pair
  assert(*l1_rankings[0].begin() < *l1_rankings[1].begin());
  assert(std::equal(l1_rankings[1].begin(), l1_rankings[1].end(), l1_rankings[1].begin()));

  // two pair
  assert(*l1_rankings[1].begin() < *l1_rankings[2].begin());
  assert(std::equal(l1_rankings[2].begin(), l1_rankings[2].end(), l1_rankings[2].begin()));

  // trips
  assert(*l1_rankings[2].begin() < *l1_rankings[3].begin());
  assert(std::equal(l1_rankings[3].begin(), l1_rankings[3].end(), l1_rankings[3].begin()));

  // straight
  assert(*l1_rankings[3].begin() < *l1_rankings[4].begin());
  assert(std::equal(l1_rankings[4].begin(), l1_rankings[4].end(), l1_rankings[4].begin()));

  // flush
  assert(*l1_rankings[4].begin() < *l1_rankings[5].begin());
  assert(std::equal(l1_rankings[5].begin(), l1_rankings[5].end(), l1_rankings[5].begin()));

  // boat
  assert(*l1_rankings[5].begin() < *l1_rankings[6].begin());
  assert(std::equal(l1_rankings[6].begin(), l1_rankings[6].end(), l1_rankings[6].begin()));

  // quads
  assert(*l1_rankings[6].begin() < *l1_rankings[7].begin());
  assert(std::equal(l1_rankings[7].begin(), l1_rankings[7].end(), l1_rankings[7].begin()));

  // straight flush
  assert(*l1_rankings[7].begin() < *l1_rankings[8].begin());
  assert(std::equal(l1_rankings[8].begin(), l1_rankings[8].end(), l1_rankings[8].begin()));
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
