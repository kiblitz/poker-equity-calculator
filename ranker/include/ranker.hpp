#include <array>

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

  typedef std::array<card_t, 5> hand_t;

  ranking_t rank(hand_t hand);
}
