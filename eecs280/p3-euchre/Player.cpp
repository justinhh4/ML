#include <cassert>
#include <iostream>
#include "Player.hpp"
#include <vector>
#include <algorithm>

using namespace std;

class SimplePlayer : public Player
{
private:
	string name;
	vector<Card> hand;

public:
	SimplePlayer(string name) : name(name)
	{
	}

	// EFFECTS returns player's name
	const std::string &get_name() const override
	{
		return name;
	}

	// REQUIRES player has less than MAX_HAND_SIZE cards
	// EFFECTS  adds Card c to Player's hand
	void add_card(const Card &c) override
	{
		assert(hand.size() < MAX_HAND_SIZE);
		hand.push_back(c);
	}

	// REQUIRES round is 1 or 2
	// MODIFIES order_up_suit
	// EFFECTS If Player wishes to order up a trump suit then return true and
	//   change order_up_suit to desired suit.  If Player wishes to pass, then do
	//   not modify order_up_suit and return false.
	bool make_trump(const Card &upcard, bool is_dealer,
									int round, Suit &order_up_suit) const override
	{
		assert(round == 1 || round == 2);
		if (round == 1)
		{
			int num = 0;
			for (int i = 0; i < MAX_HAND_SIZE; i++)
			{
				if ((hand[i].is_face_or_ace() && hand[i].get_suit() == upcard.get_suit()) ||
						hand[i].is_left_bower(upcard.get_suit()))
				{
					num++;
				}
			}
			if (num >= 2)
			{
				order_up_suit = upcard.get_suit();
				return true;
			}
			else
				return false;
		}
		else if (is_dealer)
		{
			order_up_suit = Suit_next(upcard.get_suit());
			return true;
		}
		else
		{
			for (int i = 0; i < MAX_HAND_SIZE; i++)
			{
				if (hand[i].is_face_or_ace() &&
						(Suit_next(hand[i].get_suit()) == upcard.get_suit() ||
							hand[i].is_left_bower(upcard.get_suit())))
				{
					order_up_suit = Suit_next(upcard.get_suit());
					return true;
				}
			}
			return false;
		}
	}

	// REQUIRES Player has at least one card
	// EFFECTS  Player adds one card to hand and removes one card from hand.
	void add_and_discard(const Card &upcard) override
	{
		int minIndex = 0;
		Card minCard = hand[0];
		for (int i = 0; i < hand.size(); i++)
		{
			if (Card_less(hand[i], minCard, upcard.get_suit()))
			{
				minCard = hand[i];
				minIndex = i;
			}
		}
		hand.erase(hand.begin() + minIndex);
		hand.push_back(upcard);
	}

	// REQUIRES Player has at least one card
	// EFFECTS  Leads one Card from Player's hand according to their strategy
	//   "Lead" means to play the first Card in a trick.  The card
	//   is removed the player's hand.
	Card lead_card(Suit trump) override
	{
		int numTrump = 0;
		for (int i = 0; i < hand.size(); i++)
		{
			if (hand[i].get_suit(trump) == trump)
			{
				numTrump++;
			}
		}
		int maxIndex = 0;
		Card maxCard = hand[0];
		if (numTrump == hand.size())
		{
			for (int i = 0; i < hand.size(); i++)
			{
				if (Card_less(maxCard, hand[i], trump))
				{
					maxCard = hand[i];
					maxIndex = i;
				}
			}
		}
		else
		{
			for (int i = 0; i < hand.size(); i++)
			{
				if (hand[i].get_suit(trump) != trump)
				{
					maxIndex = i;
					maxCard = hand[i];
					break;
				}
			}
			for (int i = 0; i < hand.size(); i++)
			{
				if ((hand[i].get_suit(trump) != trump) && Card_less(maxCard, hand[i], trump))
				{
					maxCard = hand[i];
					maxIndex = i;
				}
			}
		}
		hand.erase(hand.begin() + maxIndex);
		return maxCard;
	}

	// REQUIRES Player has at least one card
	// EFFECTS  Plays one Card from Player's hand according to their strategy.
	//   The card is removed from the player's hand.
	Card play_card(const Card &led_card, Suit trump) override
	{
		bool hasLedCard = false;
		for (int i = 0; i < hand.size(); i++)
		{
			if (hand[i].get_suit(trump) == led_card.get_suit(trump))
			{
				hasLedCard = true;
				break;
			}
		}
		int maxIndex = 0;
		Card maxCard = hand[0];
		if (hasLedCard)
		{
			for (int i = 0; i < hand.size(); i++)
			{
				if ((hand[i].get_suit(trump) == led_card.get_suit(trump)))
				{
					maxIndex = i;
					maxCard = hand[i];
					break;
				}
			}
			for (int i = 0; i < hand.size(); i++)
			{
				if (Card_less(maxCard, hand[i], led_card, trump) && 
					(hand[i].get_suit(trump) == led_card.get_suit(trump)))
				{
					maxIndex = i;
					maxCard = hand[i];
				}
			}
		}
		else
		{
			for (int i = 0; i < hand.size(); i++)
			{
				// we are not actually using max we are doing min
				if (Card_less(hand[i], maxCard, led_card, trump))
				{
					maxIndex = i;
					maxCard = hand[i];
				}
			}
		}
		hand.erase(hand.begin() + maxIndex);
		return maxCard;
	}
};

//////////// HUMAN PLAYER //////////////

class HumanPlayer : public Player
{
private:
	string name;
	vector<Card> hand;

public:
	HumanPlayer(string name) : name(name)
	{
	}

	// EFFECTS returns player's name
	const std::string &get_name() const override
	{
		return name;
	}

	// REQUIRES player has less than MAX_HAND_SIZE cards
	// EFFECTS  adds Card c to Player's hand
	void print_hand() const
	{
		for (size_t i = 0; i < hand.size(); ++i)
			cout << "Human player " << name << "'s hand: "
					 << "[" << i << "] " << hand[i] << "\n";
	}

	void add_card(const Card &c) override
	{
		hand.push_back(c);
		sort(hand.begin(), hand.end());
	}

	// REQUIRES round is 1 or 2
	// MODIFIES order_up_suit
	// EFFECTS If Player wishes to order up a trump suit then return true and
	//   change order_up_suit to desired suit.  If Player wishes to pass, then do
	//   not modify order_up_suit and return false.
	bool make_trump(const Card &upcard, bool is_dealer,
									int round, Suit &order_up_suit) const override
	{
		print_hand();
		cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
		string decision;
		cin >> decision;

		if (decision != "pass")
		{
			order_up_suit = string_to_suit(decision);
			return true;
		}
		else
			return false;
	}

	// REQUIRES Player has at least one card
	// EFFECTS  Player adds one card to hand and removes one card from hand.
	void add_and_discard(const Card &upcard) override
	{
		print_hand();
		cout << "Discard upcard: [-1]\n";
		cout << "Human player " << name << ", please select a card to discard:\n";
		int index;
		cin >> index;
		if (index == -1)
		{
			return;
		}
		else
		{
			hand.erase(hand.begin() + index);
			add_card(upcard);
		}
	}

	// REQUIRES Player has at least one card
	// EFFECTS  Leads one Card from Player's hand according to their strategy
	//   "Lead" means to play the first Card in a trick.  The card
	//   is removed the player's hand.
	Card lead_card(Suit trump) override
	{
		print_hand();
		cout << "Human player " << name << ", please select a card:\n";
		int i;
		cin >> i;
		Card card = hand[i];
		hand.erase(hand.begin() + i);
		return card;
	}

	// REQUIRES Player has at least one card
	// EFFECTS  Plays one Card from Player's hand according to their strategy.
	//   The card is removed from the player's hand.
	Card play_card(const Card &led_card, Suit trump) override
	{
		print_hand();
		cout << "Human player " << name << ", please select a card:\n";
		int i;
		cin >> i;
		Card card = hand[i];
		hand.erase(hand.begin() + i);
		return card;
	}
};

// EFFECTS: Returns a pointer to a player with the given name and strategy
// To create an object that won't go out of scope when the function returns,
// use "return new Simple(name)" or "return new Human(name)"
// Don't forget to call "delete" on each Player* after the game is over
Player *Player_factory(const std::string &name, const std::string &strategy)
{
	if (strategy == "Simple")
	{
		return new SimplePlayer(name);
	}
	else if (strategy == "Human")
	{
		return new HumanPlayer(name);
	}
	assert(false);
}

// EFFECTS: Prints player's name to os
std::ostream &operator<<(std::ostream &os, const Player &p)
{
	os << p.get_name();
	return os;
}
