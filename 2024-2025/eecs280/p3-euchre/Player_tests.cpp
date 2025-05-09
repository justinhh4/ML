#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name)
{
    Player *alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_insertion)
{
    // Create a Human player
    Player *human = Player_factory("NotRobot", "Human");

    // Print the player using the stream insertion operator
    ostringstream oss1;
    oss1 << *human;

    // Verify that the output is the player's name
    ASSERT_EQUAL(oss1.str(), "NotRobot");

    // Create a Simple player
    Player *alice = Player_factory("Alice", "Simple");

    // Print the player using the stream insertion operator
    ostringstream oss2;
    oss2 << *alice;
    ASSERT_EQUAL(oss2.str(), "Alice");

    // Clean up players that were created using Player_factory()
    delete human;
    delete alice;
}

TEST(test_simple_player_make_trump)
{
    // Bob's hand
    Player *bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, SPADES));
    bob->add_card(Card(TEN, SPADES));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, SPADES));
    bob->add_card(Card(ACE, SPADES));

    // Bob makes tump
    Card nine_spades(NINE, SPADES);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_spades, // Upcard
        true,        // Bob is also the dealer
        1,           // First round
        trump        // Suit ordered up (if any)
    );

    // Verify Bob's order up and trump suit
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, SPADES);

    bool orderup2 = bob->make_trump(Card(NINE, HEARTS), false, 1, trump);
    ASSERT_FALSE(orderup2);
    ASSERT_EQUAL(trump, SPADES);

    bob->add_and_discard(Card(KING, DIAMONDS));
    bool orderup3 = bob->make_trump(Card(NINE, HEARTS), true, 2, trump);
    ASSERT_TRUE(orderup3);
    ASSERT_EQUAL(trump, DIAMONDS);

    bob->add_and_discard(Card(JACK, HEARTS));
    bool orderup4 = bob->make_trump(Card(NINE, DIAMONDS), false, 1, trump);
    ASSERT_TRUE(orderup4);
    ASSERT_EQUAL(trump, DIAMONDS);
    // QUEEN KING ACE SPADES, KING DIAMONDS, JACK HEARTS

    bob->add_and_discard(Card(JACK, DIAMONDS));
    bool orderup5 = bob->make_trump(Card(NINE, DIAMONDS), true, 1, trump);
    ASSERT_TRUE(orderup5);
    ASSERT_EQUAL(trump, DIAMONDS);

    // KING ACE SPADES, KING DIAMONDS, JACK HEARTS, JACK DIAMONDS

    bob->add_and_discard(Card(NINE, DIAMONDS));
    bool orderup6 = bob->make_trump(Card(NINE, SPADES), true, 1, trump);
    ASSERT_FALSE(orderup6);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete bob;
}
TEST(make_trump_2)
{
    Suit trump;
    Player *bob2 = Player_factory("Bob", "Simple");
    bob2->add_card(Card(QUEEN, HEARTS));
    bob2->add_card(Card(TEN, SPADES));
    bob2->add_card(Card(QUEEN, SPADES));
    bob2->add_card(Card(JACK, HEARTS));
    bob2->add_card(Card(ACE, DIAMONDS));

    bool orderup7 = bob2->make_trump(Card(NINE, SPADES), false, 1, trump);
    ASSERT_FALSE(orderup7);
    bool orderup8 = bob2->make_trump(Card(NINE, HEARTS), false, 1, trump);
    ASSERT_TRUE(orderup8);
    bool orderup9 = bob2->make_trump(Card(KING, DIAMONDS), true, 1, trump);
    ASSERT_TRUE(orderup9);
    bool orderup10 = bob2->make_trump(Card(KING, CLUBS), false, 1, trump);
    ASSERT_FALSE(orderup10);
    bool orderup11 = bob2->make_trump(Card(NINE, CLUBS), false, 2, trump);
    ASSERT_TRUE(orderup11);
    ASSERT_EQUAL(trump, SPADES);
    bob2->add_and_discard(Card(ACE, HEARTS));
    bob2->add_and_discard(Card(KING, HEARTS));
    bool orderup12 = bob2->make_trump(Card(NINE, SPADES), true, 2, trump);
    ASSERT_TRUE(orderup12);
    ASSERT_EQUAL(trump, CLUBS);

    /*
    bool orderup12 = bob2->make_trump(Card(JACK, DIAMONDS), true, 2, trump);
    ASSERT_TRUE(orderup12);
    ASSERT_EQUAL(trump, DIAMONDS);
    bool orderup13 = bob2->make_trump(Card(KING, DIAMONDS), false, 2, trump);
    ASSERT_TRUE(orderup13);
    ASSERT_EQUAL(trump, SPADES);
    bool orderup14 = bob2->make_trump(Card(KING, CLUBS), true, 2, trump);
    ASSERT_FALSE(orderup14);
    ASSERT_EQUAL(trump, SPADES);
    */
    delete bob2;
}

TEST(test_simple_player_lead_card)
{
    // Bob's hand
    Player *bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, SPADES));
    bob->add_card(Card(TEN, SPADES));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, SPADES));
    bob->add_card(Card(ACE, SPADES));

    // Bob adds a card to his hand and discards one card
    bob->add_and_discard(
        Card(NINE, HEARTS) // upcard
    );

    // Bob leads
    Card card_led = bob->lead_card(HEARTS);

    // Verify the card Bob selected to lead
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    Card nine_hearts(NINE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card jack_clubs(JACK, CLUBS);

    ASSERT_EQUAL(card_led, ace_spades); // check led card
    Card card_led2 = bob->lead_card(SPADES);
    ASSERT_EQUAL(card_led2, nine_hearts);
    Card card_led3 = bob->lead_card(CLUBS);
    ASSERT_EQUAL(card_led3, king_spades);

    bob->add_card(Card(JACK, CLUBS));
    bob->add_card(Card(JACK, DIAMONDS));

    Card card_led4 = bob->lead_card(SPADES);
    ASSERT_EQUAL(card_led4, jack_diamonds);
    Card card_led5 = bob->lead_card(SPADES);
    ASSERT_EQUAL(card_led5, jack_clubs);

    delete bob;
}

TEST(test_simple_player_play_card_and_make_trump)
{
    // Bob's hand
    Player *bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, SPADES));
    bob->add_card(Card(TEN, SPADES));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, SPADES));
    bob->add_card(Card(ACE, SPADES));

    // Bob plays a card
    Card nine_diamonds(NINE, DIAMONDS);
    Card card_played = bob->play_card(
        nine_diamonds, // Nine of Diamonds is led
        HEARTS         // Trump suit
    );

    // Verify the card Bob played
    ASSERT_EQUAL(card_played, Card(NINE, SPADES));
    Card card_played2 = bob->play_card(Card(QUEEN, SPADES), SPADES);
    ASSERT_EQUAL(card_played2, Card(ACE, SPADES));
    Card card_played3 = bob->play_card(Card(JACK, CLUBS), SPADES);
    ASSERT_EQUAL(card_played3, Card(KING, SPADES));

    bob->add_card(Card(JACK, HEARTS));
    bob->add_card(Card(QUEEN, DIAMONDS));
    bob->add_card(Card(NINE, DIAMONDS));
    Card card_played4 = bob->play_card(Card(NINE, CLUBS), CLUBS);
    ASSERT_EQUAL(card_played4, Card(NINE, DIAMONDS));
    // TEN and QUEEN SPADES, JACK HEARTS, QUEEN DIAMONDS
    bob->add_card(Card(JACK, DIAMONDS));
    Card card_played5 = bob->play_card(Card(ACE, DIAMONDS), DIAMONDS);
    ASSERT_EQUAL(card_played5, Card(JACK, DIAMONDS));
    Card card_played6 = bob->play_card(Card(KING, DIAMONDS), DIAMONDS);
    ASSERT_EQUAL(card_played6, Card(JACK, HEARTS));

    bob->add_card(Card(TEN, HEARTS));
    bob->add_card(Card(TEN, CLUBS));
    Card card_played7 = bob->play_card(Card(KING, DIAMONDS), SPADES);
    ASSERT_EQUAL(card_played7, Card(QUEEN, DIAMONDS));
    Card card_played8 = bob->play_card(Card(KING, DIAMONDS), DIAMONDS);
    ASSERT_EQUAL(card_played8, Card(TEN, SPADES));

    delete bob;
}

TEST_MAIN()
