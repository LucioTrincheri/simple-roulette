#ifndef PLAYER_H
#define PLAYER_H

#include <deque>
#include <functional>
#include <algorithm>
#include <fstream>

// Player class. This contains the deque of values to create the bets, the bet winner/losser function and the player balance. 
class Player {
private:
    std::deque<int> bets;
    std::deque<int> starting_bets;
    std::function<bool(int)> winFunc;
    int balance = 0;

public:
    Player(std::function<bool(int)> winFunc, const std::deque<int>& bets);

// Methods to interact with the bets, the balance and if the number given is a winner. 
    void addBet(int bet);
    void removeBet();
    void startingBets();
    int getBet() const;
    void updateBalance(int bet);
    int getBalance() const;
    bool isWinner(int number) const;
};

// Functions given to the player constructor
bool isRed(int number);
bool isBlack(int number);
bool isMinor(int number);
bool isMajor(int number);
bool isEven(int number);
bool isOdd(int number);

#endif