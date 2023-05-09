// Copyright Andrew Bernal 2023
#include "tictactoe.hpp"

std::vector<Tic> Tic::getLegalBoards() {
    std::vector<std::pair<int, int>> emptySpaces;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '.') {
            int row = i / 3;
            int col = i % 3;
            emptySpaces.push_back(std::make_pair(row, col));
        }
    }
    std::vector<Tic> validBoards;
    for (auto& e : emptySpaces) {
        // copy the current object.
        // Push it to the back of a vector and play a move on it.
        validBoards.push_back(Tic(*this));
        validBoards.back().playMove(e);
    }
    return validBoards;
}

bool Tic::isLegalMove(int row, int col) const {
    std::vector<std::pair<int, int>> emptySpaces;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '.') {
            int row = i / 3;
            int col = i % 3;
            emptySpaces.push_back(std::make_pair(row, col));
        }
    }
    for (auto& e : emptySpaces) {
        if (e.first == row && e.second == col) {
            return true;
        }
    }
    return false;
}

bool operator==(const Tic& lhs, const Tic& rhs) {
    for (int i = 0; i < 9; i++) {
        if (lhs.board[i] != rhs.board[i]) {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Tic& obj) {
    for (int i = 0; i < 9; i++) {
        out << obj.board[i];
    }
    out << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, Tic& obj) {
    char c;
    for (int i = 0; i < 9; i++) {
        in >> c;
        if (c != '\n')
            obj.board[i] = c;
    }
    return in;
}

// 1 is an 'X' win, false is no result, 0 is a draw, -1 is an 'O' win
std::pair<int, bool> Tic::detectWin() {
    if (board[0] == board[1] && board[1] == board[2]) {
        if (board[0] == 'X') {
            return std::make_pair(1, true);
        } else if (board[0] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[0] == board[4] && board[4] == board[8]) {
        if (board[0] == 'X') {
            return std::make_pair(1, true);
        } else if (board[0] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[0] == board[3] && board[3] == board[6]) {
        if (board[0] == 'X') {
            return std::make_pair(1, true);
        } else if (board[0] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[1] == board[4] && board[4] == board[7]) {
        if (board[1] == 'X') {
            return std::make_pair(1, true);
        } else if (board[1] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[2] == board[4] && board[4] == board[6]) {
        if (board[2] == 'X') {
            return std::make_pair(1, true);
        } else if (board[2] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[2] == board[5] && board[5] == board[8]) {
        if (board[2] == 'X') {
            return std::make_pair(1, true);
        } else if (board[2] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[3] == board[4] && board[4] == board[5]) {
        if (board[3] == 'X') {
            return std::make_pair(1, true);
        } else if (board[3] == 'O') {
            return std::make_pair(-1, true);
        }
    }
    if (board[6] == board[7] && board[7] == board[8]) {
        if (board[6] == 'X') {
            return std::make_pair(1, true);
        } else if (board[6] == 'O') {
            return std::make_pair(-1, true);
        }
    }

    // if there are no '.' in the board, then it is a draw
    if (std::find(board, board + 9, '.') == board + 9) {
        return std::make_pair(0, true);
    }
    return std::make_pair(-31337, false);
}
