#include <include/ranker.hpp>

#include <array>

namespace ranker {
  ranking_t rank(hand_t hand) {
    const unsigned short hand_set = 1u << hand[0].value | 1u << hand[1].value | 1u << hand[2].value | 1u << hand[3].value | 1u << hand[4].value;

    std::array<unsigned char, 13> hand_multiset{};
    ++hand_multiset[hand[0].value];
    for (unsigned i = 1u; i < 5; ++i) {
      hand_multiset[hand[i].value] <<= 1;
      ++hand_multiset[hand[i].value];
    }

    unsigned long long hand_multiset_num = 0u;
    hand_multiset_num |= hand_multiset[0];
    for (unsigned i = 1u; i < 13; ++i) {
      hand_multiset_num <<= 4;
      hand_multiset_num |= hand_multiset[i];
    }

    unsigned classification = hand_multiset_num % 0xf;
    --classification;

    classification -= (hand_set == 0x1f | hand_set == 0x3e | hand_set == 0x7c | hand_set == 0xf8 | hand_set == 0x1f0 | hand_set == 0x3e0 | hand_set == 0x7c0 | hand_set == 0xf80 | hand_set == 0x1f00 | hand_set == 0x100f) << 1;

    classification -= (hand[0].suit == hand[1].suit) & (hand[0].suit == hand[2].suit) & (hand[0].suit == hand[3].suit) & (hand[0].suit == hand[4].suit);

    return classification;
  }
}
