#include <include/ranker.hpp>

#include <array>
#include <cassert>

namespace ranker {
  ranking_t l1_rank(hand_t hand) {
    const unsigned short hand_set = 1u << hand[0].value | 1u << hand[1].value | 1u << hand[2].value | 1u << hand[3].value | 1u << hand[4].value;

    std::array<unsigned char, 13> card_count{};
    unsigned char classification{};
    card_count[hand[0].value] = 1u;

    for (unsigned i = 1u; i < 5; ++i) {
      classification += card_count[hand[i].value]++;
    }

    unsigned char straight_flush_classification = (hand_set == 0x1f | hand_set == 0x3e | hand_set == 0x7c | hand_set == 0xf8 | hand_set == 0x1f0 | hand_set == 0x3e0 | hand_set == 0x7c0 | hand_set == 0xf80 | hand_set == 0x1f00 | hand_set == 0x100f);
 
    straight_flush_classification += ((hand[0].suit == hand[1].suit) & (hand[0].suit == hand[2].suit) & (hand[0].suit == hand[3].suit) & (hand[0].suit == hand[4].suit)) * 3;

    straight_flush_classification += ((!!straight_flush_classification) * 4);
    classification += straight_flush_classification;

    assert(classification <= 8u);
    constexpr unsigned classification_permutation[9] = {0u, 1u, 2u, 3u, 6u, 4u, 7u, 5u, 8u};
    classification = classification_permutation[classification];

    return classification;
  }

  void simd_l1_rank(ranking_t *rankings, hand_t *hand) {
    for (unsigned i = 0u; i < 8; ++i) {
      rankings[i] = l1_rank(hand[i]);
    }
  }
}
