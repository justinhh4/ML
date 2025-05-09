#include <iostream>
#include <fstream>
#include "Pack.hpp"
#include "Player.hpp"
#include <vector>
#include <cassert>
using namespace std;

class Game
{
public:
  Game(vector<Player *> players, Pack pack, int pointsToWin, bool shuffle)
      : players(players), pack(pack), pointsToWin(pointsToWin), shuffle(shuffle)
  {
  }

  void play()
  {
    int team1Points = 0;
    int team2Points = 0;
    int round = 0;
    while (team1Points < pointsToWin && team2Points < pointsToWin)
    {
      pack.reset();
      playHand(round, team1Points, team2Points);
      round++;
    }
    if (team1Points > team2Points)
    {
      cout << endl
           << players[0]->get_name() << " and " << players[2]->get_name()
           << " win!" << endl;
    }
    else
    {
      cout << endl
           << players[1]->get_name() << " and " << players[3]->get_name()
           << " win!" << endl;
    }
  }

  void playHand(int round, int &team1Points, int &team2Points)
  {
    if (shuffle)
      pack.shuffle();
    cout << endl
         << "Hand " << round << endl;
    const int dealerNumber = round % 4;
    const int eldestHand = (dealerNumber + 1) % 4;
    cout << players[dealerNumber]->get_name() << " deals" << endl;
    deal(dealerNumber);

    Card upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;

    // MAKE TRUMP
    Suit trump;
    int orderUpPlayer = make_trump_round(trump, upcard, dealerNumber);

    // PLAY TRICKS
    int orderUpWinCount = 0;
    int leadPlayer = eldestHand;
    for (int i = 0; i < 5; i++)
    {
      leadPlayer = play_trick(leadPlayer, trump, orderUpWinCount, orderUpPlayer);
    }

    // POINT ASSIGNMENT

    point_assign(orderUpPlayer, orderUpWinCount, team1Points, team2Points); 

    cout << players[0]->get_name() << " and " << players[2]->get_name()
         << " have " << team1Points << " points" << endl;
    cout << players[1]->get_name() << " and " << players[3]->get_name()
         << " have " << team2Points << " points" << endl;
  }

  void point_assign(int orderUpPlayer, int orderUpWinCount, 
                    int &team1Points, int &team2Points){
    int orderUpTeam = orderUpPlayer % 2;
    int notOrderUpTeam = (orderUpPlayer + 1) % 2;
    if (orderUpWinCount == 3 || orderUpWinCount == 4)
    {
      cout << players[orderUpTeam]->get_name() << " and "
           << players[orderUpTeam + 2]->get_name()
           << " win the hand" << endl;
      if (orderUpTeam == 0)
        team1Points++;
      else
        team2Points++;
    }
    else if (orderUpWinCount == 5)
    {
      cout << players[orderUpTeam]->get_name() << " and "
           << players[orderUpTeam + 2]->get_name()
           << " win the hand" << endl;
      cout << "march!" << endl;
      if (orderUpTeam == 0)
        team1Points += 2;
      else
        team2Points += 2;
    }
    else
    {
      cout << players[notOrderUpTeam]->get_name() << " and "
           << players[notOrderUpTeam + 2]->get_name()
           << " win the hand" << endl;
      cout << "euchred!" << endl;
      if (orderUpTeam == 0)
        team2Points += 2;
      else
        team1Points += 2;
    }
  }

  void deal(int dealerNumber)
  {

    for (int i = 0; i < 4; i++)
    {
      int player = (dealerNumber + 1 + i) % 4;
      if (i % 2 == 0)
      {
        players[player]->add_card(pack.deal_one());
        players[player]->add_card(pack.deal_one());
        players[player]->add_card(pack.deal_one());
      }
      else
      {
        players[player]->add_card(pack.deal_one());
        players[player]->add_card(pack.deal_one());
      }
    }

    for (int i = 0; i < 4; i++)
    {
      int player = (dealerNumber + 1 + i) % 4;
      if (i % 2 == 1)
      {
        players[player]->add_card(pack.deal_one());
        players[player]->add_card(pack.deal_one());
        players[player]->add_card(pack.deal_one());
      }
      else
      {
        players[player]->add_card(pack.deal_one());
        players[player]->add_card(pack.deal_one());
      }
    }
  }

  // Returns index of player who ordered up
  int make_trump_round(Suit &trump, Card upcard, int dealerNumber)
  {
    const int eldestHand = (dealerNumber + 1) % 4;
    // ROUND 1
    for (int i = 0; i < 4; i++)
    {
      int player = (eldestHand + i) % 4;
      if (players[player]->make_trump(upcard, false, 1, trump))
      {
        cout << players[player]->get_name() << " orders up " << trump << endl;
        players[dealerNumber]->add_and_discard(upcard);
        cout << endl;
        return player;
      }
      else
        cout << players[player]->get_name() << " passes" << endl;
    }
    // ROUND 2
    for (int i = 0; i < 3; i++)
    {
      int player = (eldestHand + i) % 4;
      if (players[player]->make_trump(upcard, false, 2, trump))
      {
        // players[dealerNumber]->add_and_discard(upcard);
        cout << players[player]->get_name() << " orders up " << trump << endl
             << endl;
        return player;
      }
      else
        cout << players[player]->get_name() << " passes" << endl;
    }

    // players[dealerNumber]->add_and_discard(upcard);
    cout << players[dealerNumber]->get_name() << " orders up " << trump << endl
         << endl;
    return dealerNumber;
  }

  int play_trick(int leadPlayer, Suit trump, int &orderUpWinCount, int orderUpPlayer)
  {
    Card led = players[leadPlayer]->lead_card(trump);
    Card max = led;
    int maxPlayer = leadPlayer;
    cout << led << " led by " << players[leadPlayer]->get_name() << endl;
    for (int i = 1; i < 4; i++)
    {
      int player = (leadPlayer + i) % 4;
      Card played = players[player]->play_card(led, trump);
      cout << played << " played by " << players[player]->get_name() << endl;
      if (Card_less(max, played, led, trump))
      {
        max = played;
        maxPlayer = player;
      }
    }
    cout << players[maxPlayer]->get_name() << " takes the trick" << endl
         << endl;
    if (maxPlayer % 2 == orderUpPlayer % 2)
      orderUpWinCount++;
    return maxPlayer;
  }

private:
  vector<Player *> players;
  Pack pack;
  int pointsToWin;
  bool shuffle;
};

int main(int argc, char *argv[])
{
  if (argc != 12){
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
         << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
    return 1;
  }
  ifstream packIn(argv[1]);
  string shuffle = argv[2];
  int pointsToWin = stoi(argv[3]);
  string p1N = argv[4];
  string p2N = argv[6];
  string p3N = argv[8];
  string p4N = argv[10];
  string p1T = argv[5];
  string p2T = argv[7];
  string p3T = argv[9];
  string p4T = argv[11];
  if (pointsToWin < 1 || pointsToWin > 100 ||
      (shuffle != "shuffle" && shuffle != "noshuffle") ||
      (p1T != "Simple" && p1T != "Human") || (p2T != "Simple" && p2T != "Human") ||
      (p3T != "Simple" && p3T != "Human") || (p4T != "Simple" && p4T != "Human")){
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
         << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
    return 1;
  }
  if (!packIn.is_open()){
    cout << "Error opening " << argv[1] << endl;
    return 1;
  }
  bool shuffled;
  if (shuffle == "shuffle")
    shuffled = true;
  else
    shuffled = false;
    /*
  Player *p1 = Player_factory(p1N, p1T);
  Player *p2 = Player_factory(p2N, p2T);
  Player *p3 = Player_factory(p3N, p3T);
  Player *p4 = Player_factory(p4N, p4T);
  */
  vector<Player *> players;
  players.push_back(Player_factory(p1N, p1T));
  players.push_back(Player_factory(p2N, p2T));
  players.push_back(Player_factory(p3N, p3T));
  players.push_back(Player_factory(p4N, p4T));

  Pack pack(packIn);
  Game game(players, pack, pointsToWin, shuffled);
  for (int i = 0; i < argc; i++)
  {
    cout << argv[i] << " ";
  }
  game.play();

  for (size_t i = 0; i < players.size(); ++i)
  {
    delete players[i];
  }
}
