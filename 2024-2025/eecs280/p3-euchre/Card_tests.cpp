#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

TEST(test_card_ctor)
{
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());

    Card c1(JACK, HEARTS);
    ASSERT_EQUAL(DIAMONDS, c1.get_suit(DIAMONDS));
    Card c2(ACE, HEARTS);
    ASSERT_EQUAL(HEARTS, c2.get_suit(DIAMONDS));

    Card c3;
    ASSERT_EQUAL(SPADES, c3.get_suit());
    ASSERT_EQUAL(TWO, c3.get_rank());
}

TEST(test_card_suit_and_rank)
{
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), TWO);
    ASSERT_EQUAL(two_spades.get_suit(), SPADES);

    Card three_spades = Card(THREE, SPADES);
    ASSERT_EQUAL(three_spades.get_rank(), THREE);
    ASSERT_EQUAL(three_spades.get_suit(), SPADES);
    ASSERT_EQUAL(three_spades.get_suit(CLUBS), SPADES);

    Card jack_clubs = Card(JACK, CLUBS);
    ASSERT_EQUAL(jack_clubs.get_suit(SPADES), SPADES);
}

TEST(test_card_type)
{
    Card three_spades = Card(THREE, SPADES);
    Card jack_clubs = Card(JACK, CLUBS);
    ASSERT_FALSE(three_spades.is_face_or_ace());
    ASSERT_FALSE(three_spades.is_right_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_left_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_trump(CLUBS));
    ASSERT_TRUE(jack_clubs.is_left_bower(SPADES));
    ASSERT_FALSE(jack_clubs.is_left_bower(CLUBS));
    ASSERT_TRUE(jack_clubs.is_right_bower(CLUBS));
}

TEST(test_card_self_comparison)
{
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades < three_spades);
    ASSERT_TRUE(three_spades <= three_spades);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(three_spades >= three_spades);
    ASSERT_TRUE(three_spades == three_spades);
    ASSERT_FALSE(three_spades != three_spades);
    ASSERT_TRUE(Card(ACE, SPADES) < Card(ACE, DIAMONDS));
    ASSERT_TRUE(Card(JACK, CLUBS) < Card(KING, DIAMONDS));
    ASSERT_TRUE(Card(TEN, DIAMONDS) < Card(KING, SPADES));
    ASSERT_TRUE(Card(JACK, DIAMONDS) > Card(TEN, DIAMONDS));
    ASSERT_TRUE(Card(JACK, DIAMONDS) >= Card(JACK, DIAMONDS));
    ASSERT_TRUE(Card(JACK, DIAMONDS) <= Card(QUEEN, SPADES));
}

TEST(test_Suit_next)
{
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(test_Card_less)
{
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(Card_less(three_spades, three_spades, CLUBS));
    ASSERT_FALSE(Card_less(three_spades, three_spades, three_spades,
                           CLUBS));

    Card ace_spades = Card(ACE, SPADES);
    Card jack_spades = Card(JACK, SPADES);
    Card jack_clubs = Card(JACK, CLUBS);
    Card nine_hearts = Card(NINE, HEARTS);
    Card ten_diamonds = Card(TEN, DIAMONDS);

    ASSERT_TRUE(Card_less(ace_spades, jack_clubs, SPADES));
    ASSERT_TRUE(Card_less(ace_spades, nine_hearts, HEARTS));
    ASSERT_FALSE(Card_less(Card(JACK, HEARTS), Card(ACE, HEARTS), HEARTS));
    ASSERT_FALSE(Card_less(ace_spades, nine_hearts, nine_hearts, SPADES));
    ASSERT_TRUE(Card_less(ace_spades, nine_hearts, nine_hearts, DIAMONDS));
    ASSERT_FALSE(Card_less(jack_spades, nine_hearts, nine_hearts, CLUBS));
    ASSERT_FALSE(Card_less(ace_spades, nine_hearts, ten_diamonds, CLUBS));
    ASSERT_TRUE(Card_less(Card(ACE, SPADES), Card(JACK, SPADES), SPADES));
    ASSERT_TRUE(Card_less(Card(TEN, HEARTS), Card(TEN, SPADES), Card(NINE, DIAMONDS), SPADES));
    ASSERT_TRUE(Card_less(Card(NINE, CLUBS), Card(QUEEN, DIAMONDS), Card(JACK, CLUBS), SPADES));
}

TEST(test_card_insertion)
{
    Card three_spades = Card(THREE, SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}

TEST(test_card_extraction)
{
    istringstream iss("Three of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(THREE, SPADES));
}

// Add more test cases here

TEST_MAIN()
