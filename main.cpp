#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <fstream>
#include <functional>

#include "player.h"
int main() {
    // Simulation variables 
    const int NUM_PLAYERS = 6;
    const int NUM_SPINS = 10000;
    const int MIN_BET = 5;
    const int MAX_BET = 4000;

    srand(time(nullptr));

    //Players names are in this order (ex. Player A = isRed)
    std::deque<int> genericBets = {1, 2, 3, 4};
    std::vector<Player> players{
        Player(isRed, genericBets),
        Player(isBlack, genericBets),
        Player(isMajor, genericBets),
        Player(isMinor, genericBets),
        Player(isEven, genericBets),
        Player(isOdd, genericBets)
    };

    // Open file to store the logs. If the file fails to open, end the program with error code
    std::ofstream file;
    file.open ("resultados.txt");
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // For each spin, generate a winning number. Then for each player we get the bet, check if is valid, find if the player won or not and then update the balance and log the result
    // The code to check if the number is a winner and to add or remove balance could be run on a different thread for 
    // each player, but the time complexity of the code is too small to justify the overhead when creating the threads
    for (int spin = 0; spin < NUM_SPINS; spin++) {
        int winnerNumber = rand() % 37;
        file << "Spin " << spin + 1 << ": " << winnerNumber << std::endl;

        for (int j = 0; j < NUM_PLAYERS; j++) {
            int bet = players[j].getBet();

            if (bet > MAX_BET || bet < MIN_BET) {
                players[j].startingBets();
                bet = players[j].getBet();
            }
            
            if (players[j].isWinner(winnerNumber)) {
                players[j].addBet(bet);
                players[j].updateBalance(bet);
                file << "Player " << (char)('A' + j) << " WON: " << bet << std::endl;
            } else {
                players[j].removeBet();
                players[j].updateBalance(-bet);
                file << "Player " << (char)('A' + j) << " LOST: " << bet << std::endl;
            }
        }
    }
    
    // At the end of the file log each player balance
    file << std::endl << "Players balances at the end of the game:" << std::endl;
    for (int j = 0; j < NUM_PLAYERS; j++) {
        file << "Player " << (char)('A' + j) << " balance: " << players[j].getBalance() << std::endl;
    }
    file.close();

    std::cout << "Results stored in 'resultados.txt'." << std::endl;

    return 0;
}