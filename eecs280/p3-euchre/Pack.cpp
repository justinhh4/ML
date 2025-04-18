#include <cassert>
#include <iostream>
#include <sstream>
#include <array>
#include "Pack.hpp"

using namespace std;

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on.
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack() : next(0)
{
    // what to do with next card?
    int i = 0;
    for (int s = SPADES; s <= DIAMONDS; s++)
    {
        for (int r = NINE; r <= ACE; r++)
        {
            Card card = Card(static_cast<Rank>(r), static_cast<Suit>(s));
            cards[i] = card;
            i++;
        }
    }
}

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack(std::istream &pack_input) : next(0)
{
    for (int i = 0; i < 24; i++)
    {
        string card;
        getline(pack_input, card);
        istringstream sentence(card);
        sentence >> cards[i];
    }
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one()
{
    Card card = cards[next];
    next++;
    return card;
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset()
{
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle()
{
    for (int i = 0; i < 7; i++)
    {
        Pack temp = *this;
        for (int j = 0; j < 12; j++)
        {
            cards[2 * j + 1] = temp.cards[j];
        }
        for (int j = 12; j < 24; j++)
        {
            cards[(j - 12) * 2] = temp.cards[j];
        }
    }
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const
{
    if (next == 24)
    {
        return true;
    }
    else
        return false;
}
