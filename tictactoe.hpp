// Copyright Andrew Bernal 2023
#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
class Tic {
 public:
    Tic() {
        for (int i = 0; i < 9; i++) {
            board[i] = '.';
        }
        whiteTurn = true;
    }

    Tic(const Tic& obj) {
        for (int i = 0; i < 9; i++) {
            board[i] = obj.board[i];
        }
        whiteTurn = obj.whiteTurn;
    }

    friend std::ostream& operator<<(std::ostream& out, const Tic& obj);
    friend std::istream& operator>>(std::istream& in, Tic& obj);

    void placeX(int x, int y) {
        board[3 * x + y] = 'X';
        whiteTurn = !whiteTurn;
    }
    void placeO(int x, int y) {
        board[3 * x + y] = 'O';
        whiteTurn = !whiteTurn;
    }
    void playMove(int x, int y) {
        board[3 * x + y] = whiteTurn ? 'X' : 'O';
        whiteTurn = !whiteTurn;
    }
    void playMove(std::pair<int, int> loc) {
        board[3 * loc.first + loc.second] = whiteTurn ? 'X' : 'O';
        whiteTurn = !whiteTurn;
    }
    // 1 is an 'X' win, false is no result, 0 is a draw, -1 is an 'O' win
    std::pair<int, bool> detectWin();

    std::vector<Tic> getLegalBoards();

    friend bool operator==(const Tic& lhs, const Tic& rhs);

    bool isLegalMove(int row, int col) const;

    char* getBoard() {
        return board;
    }

 private:
    char board[9];
    bool whiteTurn;
};
