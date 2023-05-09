// Copyright Andrew Bernal 2023
#include <ncurses.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "tictactoe.hpp"

// Build a tree of possibilities
struct Node {
    std::vector<Node*> children;
    int eval;
    bool isTerminal;
    Tic board;
    explicit Node(const Tic& toCopy) : board(toCopy) {}
    Node() {}
};

void buildTree(Node* root);
void destroyTree(Node* root);
std::pair<Tic, int> minMaxTree(Node* root, bool XtoMove);
int minMaxTreeHelper(Node* root, int alpha, int beta, bool XtoMove);
std::pair<int, int> getPlayerInput(const Tic& board);
void updateNcurses(char* board);
char introScreen();
void outroScreen(int eval);
bool playAgain();

bool playAgain() {
    mvprintw(7, 0, "Play again? (Y/N)");
    refresh();
    flushinp();

    int ch = getch();
    return ch == 'Y' || ch == 'y';
}

void updateNcurses(char* board) {
    clear();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char c = board[3 * i + j];
            // don't draw periods, draw spaces instead
            if (c == '.') {
                c = ' ';
            }
            addch(c);
            if (j < 2) {
                addch('|');
            }
        }
        if (i < 2) {
            addstr("\n-+-+-\n");
        }
    }
    refresh();
}

char introScreen() {
    clear();
    printw("Please enter X or O to continue: ");
    refresh();
    // the compiler gives me a warning if it is not initialized, stupid compiler
    int ch = INT32_MAX;
    while (true) {
        ch = getch();
        if (ch == 'X' || ch == 'O') {
            mvprintw(1, 0, "You have chosen %c", ch);
            break;
        }
    }
    refresh();
    return ch;
}

void outroScreen(int eval) {
    switch (eval) {
        case -1:
            mvprintw(6, 0, "O has won");
            break;
        case 0:
            mvprintw(6, 0, "The game is a draw");
            break;
        case 1:
            mvprintw(6, 0, "X has won");
            break;
        default:
            throw("Invalid eval");
    }
    refresh();
}
int main(void) {
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_CLICKED, NULL);

    Node* pRoot = new Node();
    pRoot->board = Tic();
    pRoot->eval = 0xBEEF;
    pRoot->isTerminal = false;

    buildTree(pRoot);

    Node* oldRoot = pRoot;

    bool shouldContinue = true;
    while (shouldContinue) {
        char color = introScreen();

        Node root = *pRoot;

        // if the player selected X then it is their turn
        bool playerSelectedX = color == 'X' ? true : false;
        bool playerTurn = playerSelectedX;

        // loop runs until root is a terimnal node
        while (!root.isTerminal || !root.board.detectWin().second) {
            updateNcurses(root.board.getBoard());
            std::pair<Tic, int> move;
            if (!playerTurn) {
                move = minMaxTree(&root, !playerSelectedX);
                // traverse the tree to make that the new root node
                for (auto& e : root.children) {
                    if (e->board == move.first) {
                        root = *e;
                        break;
                    }
                }
                playerTurn = !playerTurn;
            }

            updateNcurses(root.board.getBoard());

            if (root.isTerminal || root.board.detectWin().second) {
                break;
            }

            if (playerTurn) {
                // get the move from the player
                std::pair<int, int> playerMove = getPlayerInput(root.board);
                Tic copyOfBoard(root.board);
                copyOfBoard.playMove(playerMove);
                // use the node that contains where the player moved
                if (root.children.size() > 0) {
                    for (auto& e : root.children) {
                        if (e->board == copyOfBoard) {
                            root = *e;
                            break;
                        }
                    }
                } else {
                    break;
                }
                playerTurn = !playerTurn;
            }
        }

        updateNcurses(root.board.getBoard());
        outroScreen(root.board.detectWin().first);
        shouldContinue = playAgain();
    }
    endwin();
    destroyTree(oldRoot);

    return 0;
}

// no need for depth, as this traverses tic tac toe exhaustively
void buildTree(Node* root) {
    std::pair<int, bool> detectWinResult = root->board.detectWin();
    root->eval = detectWinResult.first;
    root->isTerminal = detectWinResult.second;
    if (!root->isTerminal) {
        for (auto& e : root->board.getLegalBoards()) {
            Node* child = new Node(e);
            buildTree(child);
            root->children.push_back(child);
        }
    }
}

// deallocate the tree
void destroyTree(Node* root) {
    if (root == nullptr)
        return;
    for (auto& e : root->children) {
        destroyTree(e);
    }
    delete root;
}

// Traverse the tree with alpha beta pruning
// Returns the board child that is the best move and the evaluation
std::pair<Tic, int> minMaxTree(Node* root, bool XtoMove) {
    Tic maxBoard;
    int evalToReturn;
    if (XtoMove) {
        evalToReturn = INT32_MIN;
    } else {
        evalToReturn = INT32_MAX;
    }

    for (auto& e : root->children) {
        int eval = minMaxTreeHelper(e, INT32_MIN, INT32_MAX, !XtoMove);
        if (XtoMove) {
            if (eval > evalToReturn) {
                maxBoard = e->board;
                evalToReturn = eval;
            }
        } else {
            if (eval < evalToReturn) {
                maxBoard = e->board;
                evalToReturn = eval;
            }
        }
    }
    return std::make_pair(maxBoard, evalToReturn);
}
int minMaxTreeHelper(Node* root, int alpha, int beta, bool XtoMove) {
    if (root->isTerminal) {
        return root->eval;
    }

    if (XtoMove) {
        int maxEval = INT32_MIN;
        for (auto& e : root->children) {
            int eval = minMaxTreeHelper(e, alpha, beta, false);
            if (eval > maxEval) {
                maxEval = eval;
            }
            if (eval > alpha) {
                alpha = eval;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT32_MAX;
        for (auto &e : root->children) {
            int eval = minMaxTreeHelper(e, alpha, beta, true);
            if (eval < minEval) {
                minEval = eval;
            }
            if (beta < eval) {
                beta = eval;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

std::pair<int, int> getPlayerInput(const Tic& board) {
    while (true) {
        int ch = getch();
        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    int row = event.y / 2;
                    int col = event.x / 2;
                    if (row >= 0 && row < 3 && col >= 0 && col < 3) {
                        if (board.isLegalMove(row, col))
                            return std::make_pair(row, col);
                    }
                }
            }
        }
    }
}
