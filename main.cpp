#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <fstream>
#include <functional>

// Player class. This contains the deque of values to create the bets, the bet winner/losser function and the player balance. 
class Player {
private:
    std::deque<int> bets;
    std::deque<int> starting_bets;
    std::function<bool(int)> winFunc;
    int balance = 0;

public:
    // Constructor
    Player(std::function<bool(int)> winFunc, const std::deque<int>& bets) : bets(bets), starting_bets(bets), winFunc(winFunc) {}

    // Methods to add, remove or reset bets. 
    void addBet(int bet) {
        bets.push_back(bet);
    }

    void removeBet() {
        if (bets.size() == 1) {
            bets.pop_front();
        } else {
            bets.pop_front();
            bets.pop_back();
        }
    }

    void startingBets() {
        bets = starting_bets;
    }

    // Get the next bet value. Special case for the empty bets deque, as 0 is not a valid bet value
    int getBet() const {
        if (bets.empty()) {
            return 0;
        } else if (bets.size() == 1) {
            return bets.front();
        } else {
            return bets.front() + bets.back();
        }
    }

    // Update the player balance given the bet
    void updateBalance(int bet) {
        balance += bet;
    }

    // Get the player balance
    int getBalance() const {
        return balance;
    }

    // Check if player won given the roulette number
    bool isWinner(int number) const {
        return winFunc(number);
    }
};

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