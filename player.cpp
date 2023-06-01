#include <deque>
#include <functional>
#include <algorithm>
#include <fstream>

#include "player.h"
// Player class implementation. This contains the deque of values to create the bets, the bet winner/losser function and the player balance. 
// Constructor
Player::Player(std::function<bool(int)> winFunc, const std::deque<int>& bets) : bets(bets), starting_bets(bets), winFunc(winFunc) {}

// Methods to add, remove or reset bets. 
void Player::addBet(int bet) {
    bets.push_back(bet);
}

void Player::removeBet() {
    if (bets.size() == 1) {
        bets.pop_front();
    } else {
        bets.pop_front();
        bets.pop_back();
    }
}

void Player::startingBets() {
    bets = starting_bets;
}

// Get the next bet value. Special case for the empty bets deque, as 0 is not a valid bet value
int Player::getBet() const {
    if (bets.empty()) {
        return 0;
    } else if (bets.size() == 1) {
        return bets.front();
    } else {
        return bets.front() + bets.back();
    }
}

// Update the player balance given the bet
void Player::updateBalance(int bet) {
    balance += bet;
}

// Get the player balance
int Player::getBalance() const {
    return balance;
}

// Check if player won given the roulette number
bool Player::isWinner(int number) const {
    return winFunc(number);
}

// Bool functions to pass to the Player constructor. This defines if they win the bet or not.
// The functions can be changed for lambda expressions if they will not be reused.
bool isRed(int number) {
    std::vector<int> redNumbers = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    return std::find(redNumbers.begin(), redNumbers.end(), number) != redNumbers.end();
}

bool isBlack(int number) {
    return !isRed(number) && number != 0;
}

bool isMinor(int number) {
    return number <= 18 && number != 0;
}

bool isMajor(int number) {
    return number >= 19;
}

bool isEven(int number) {
    return number % 2 == 0 && number != 0;
}

bool isOdd(int number) {
    return number % 2 != 0;
}
