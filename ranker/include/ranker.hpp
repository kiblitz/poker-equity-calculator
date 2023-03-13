#include <immintrin.h>

#include <array>
#include <string>

namespace ranker {
  typedef unsigned ranking_t;

  enum Card {
    two = 0u,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
  };

  enum Suit {
    spade = 0u, heart, club, diamond
  };

  typedef struct card {
    Card value;
    Suit suit;
  } card_t;

  namespace hand {
    constexpr unsigned SIZE = 5;
  }

  typedef std::array<card_t, hand::SIZE> hand_t;

  ranking_t l1_rank(hand_t hand);
  void simd_l1_rank(ranking_t *rankings, hand_t *hand);

  namespace hand {
    inline std::string to_string(hand_t hand) {
      std::string s;
      for (unsigned i = 0u; i < SIZE; ++i) {
        const auto &value = hand[i].value;
        const auto &suit = hand[i].suit;

        const auto value_char = value == Card::ace ? 'a' : value == Card::king ? 'k' : value == Card::queen ? 'q' : value == Card::jack ? 'j' : (static_cast<char>(value) + '2');
        const auto suit_str = suit == Suit::spade ? "s" : suit == Suit::heart ? "h" : suit == Suit::club ? "c" : suit == Suit::diamond ? "d" : "";
        s += value_char;
        s += suit_str;
        if (i != SIZE - 1) {
          s += ' ';
        }
      }
      return s;
    }
  }
}
