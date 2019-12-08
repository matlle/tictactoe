#include <iostream>
#include <climits>

#define GRID_COL 4
#define GRID_ROW 4
#define PLAYER_MAX 'X'
#define PLAYER_MIN 'O'

typedef struct Move {
    int y = 0;
    int x = 0;
} Move;

class Node {
public:
    explicit Node(char b[GRID_COL][GRID_ROW]) {
        for(int y = 0; y < GRID_COL; y++) {
            for(int x = 0; x < GRID_ROW; x++) {
                board[y][x] = b[y][x];
            }
        }
    }

    char board[GRID_COL][GRID_ROW] = {0};
    Move move = {};
};

class Tree {
public:
    Node *root = nullptr;

    // If there is at least an empty cell on this board then the board is still 'playable'
    static bool isBoardStillPlayable(char b[GRID_COL][GRID_ROW]) {
        bool r = false;
        bool wasBroken = false;
        for(int y = 0; y < GRID_COL; y++) {
            if(wasBroken) break;
            for(int x = 0; x < GRID_ROW; x++) {
                if(b[y][x] == 0) {
                    r = true;
                    wasBroken = true;
                    break;
                }
            }
        }
        return r;
    }

    /*
     * Function to evaluate a board.
     * Return 10 if Player MAX has horizontally, vertically or diagonally aligned symbol
     * Return -10 if Player MIN has horizontally, vertically or diagonally aligned symbol
     * Return 0 if no Player have aligned symbol
     * */
    static int evaluateBoard(char b[GRID_COL][GRID_ROW]) {
        int r = 0, i = 0, j = 0;
        bool maxWon = false, minWon = false;
        for(int y = 0; y < GRID_COL; y++) { // horizontal checking
            i = 0;
            maxWon = false, minWon = false;
            while(i < GRID_ROW - 1) {
                if(b[y][i] != b[y][i + 1]) {
                    maxWon = false;
                    minWon = false;
                    break;
                } else if(b[y][i] == b[y][i + 1] && b[y][0] == PLAYER_MAX) {
                    maxWon = true;
                    minWon = false;
                } else if(b[y][i] == b[y][i + 1] && b[y][0] == PLAYER_MIN) {
                    minWon = true;
                    maxWon = false;
                }
                i++;
            }
            if(maxWon) {
                return 10;
            } else if(minWon) {
                return -10;
            }
        }
        for(int x = 0; x < GRID_ROW; x++) { // vertical checking
            i = 0;
            maxWon = false, minWon = false;
            while(i < GRID_COL - 1) {
                if(b[i][x] != b[i + 1][x]) {
                    maxWon = false, minWon = false;
                    break;
                } else if(b[i][x] == b[i + 1][x] && b[0][x] == PLAYER_MAX) {
                    maxWon = true, minWon = false;
                } else if(b[i][x] == b[i + 1][x] && b[0][x] == PLAYER_MIN) {
                    minWon = true, maxWon = false;
                }
                i++;
            }
            if(maxWon) {
                return 10;
            } else if(minWon) {
                return -10;
            }
        }

        // diagonal checking from top-left to bottom-right
        i = 0;
        while(i < GRID_COL - 1) {
            if(b[i][i] != b[i + 1][i + 1]) {
                maxWon = false, minWon = false;
                break;
            } else if(b[i][i] == b[i + 1][i + 1] && b[0][0] == PLAYER_MAX) {
                maxWon = true, minWon = false;
            } else if(b[i][i] == b[i + 1][i + 1] && b[0][0] == PLAYER_MIN) {
                minWon = true, maxWon = false;
            }
            i++;
        }
        if(maxWon) {
            return 10;
        } else if(minWon) {
            return -10;
        }

        // diagonal checking from top-right to bottom-left
        i = 0, j = GRID_ROW - 1;
        maxWon = false, minWon = false;
        while(i < GRID_COL - 1 && j > 0) {
            if(b[i][j] != b[i + 1][j - 1]) {
                maxWon = false, minWon = false;
                break;
            } else if(b[i][j] == b[i + 1][j - 1] && b[0][GRID_ROW - 1] == PLAYER_MAX) {
                maxWon = true, minWon = false;
            } else if(b[i][j] == b[i + 1][j - 1] && b[0][GRID_ROW - 1] == PLAYER_MIN) {
                minWon = true, maxWon = false;
            }
            i++, j--;
        }
        if(maxWon) {
            return 10;
        } else if(minWon) {
            return -10;
        }
        return r;
    }

    ~Tree() {
        delete root;
    }
};

int findOptimalValueForNextPlayer(char, char [GRID_COL][GRID_ROW]);
void drawBoard(char, Node *);
void playerMove(char, Node *);

int main() {
    std::cout << "=== [ Tic-Tac-Toe " << GRID_COL << "X" << GRID_ROW << " ] ===" << std::endl;
    std::cout << "(we assume both players play optimally)" << std::endl << std::endl;
    char board[GRID_COL][GRID_ROW] = {0};
    Tree *tree = new Tree();
    Node *node =  new Node(board);
    tree->root = node; // The initial root Node of the game Tree is a Node with a clean board (all cells are empty on this board)
    drawBoard('0', tree->root); // Initial drawing of the board game
    playerMove(PLAYER_MAX, tree->root); // Player 'MAX' plays first
    std::cout << "Game result: "; // At the end of the game, when player 'MAX' or player 'MIN' won or there is not empty cell on the board, we print the game result
    if(Tree::evaluateBoard(tree->root->board) == 10) {
        std::cout << "max won!\n";
    } else if(Tree::evaluateBoard(tree->root->board) == -10) {
        std::cout << "min won!\n";
    } else {
        std::cout << "draw...\n";
    }

    delete node;
    delete tree;
    return 0;
}

/*
 * This funcion interates over all the possible states (a.k.a Node) a board can have, based on an initial board state
 * */
int findOptimalValueForNextPlayer(char player, char b[GRID_COL][GRID_ROW]) {
    int optimalValue = Tree::evaluateBoard(b);
    if(optimalValue == 10 || optimalValue == -10) return optimalValue;
    if(!Tree::isBoardStillPlayable(b)) {
        return 0;
    }
    if(player == PLAYER_MAX) {  // if current player is 'MIN', it makes a move, compute the board value based on this move and try to keep the maximum board value
        int maxValue = INT_MIN;
        for(int y = 0; y < GRID_COL; y++) {
            for(int x = 0; x < GRID_ROW; x++) {
                if(b[y][x] == 0) {
                    b[y][x] = player;
                    int optimal = Tree::evaluateBoard(b);
                    if(optimal > maxValue) {
                        maxValue = optimal;
                    }
                    b[y][x] = 0;
                }
            }
        }
        return maxValue;
    } else if(player == PLAYER_MIN) { // if current player is 'MIN', it makes a move, compute the board value based on this move and try to keep the minimum board value
        int minValue = INT_MAX;
        for(int y = 0; y < GRID_COL; y++) {
            for(int x = 0; x < GRID_ROW; x++) {
                if(b[y][x] == 0) {
                    b[y][x] = player;
                    int optimal = Tree::evaluateBoard(b);
                    if(optimal < minValue) {
                        minValue = optimal;
                    }
                    b[y][x] = 0;
                }
            }
        }
        return minValue;
    }
    return optimalValue;
}

void playerMove(char player, Node *node) {
    int optimalValue = Tree::evaluateBoard(node->board);
    if(optimalValue == 10 || optimalValue == -10) return;
    if(!Tree::isBoardStillPlayable(node->board)) return;
    int maxValue = player == PLAYER_MAX ? INT_MIN : INT_MAX;
    Node *moveNode = nullptr;
    for(int y = 0; y < GRID_COL; y++) {
        for(int x = 0; x < GRID_ROW; x++) {
            if(node->board[y][x] == 0) {
                node->board[y][x] = player;
                /*
                 * With the helping of the function 'findOptimalValueForNextPlayer()' and based on the current board state,
                 * the current player try to find 'optimal' value and move of its opponent.
                 * Player MAX: will keep the optimal value if this value is greater than 'maxValue' since maxValue == INT_MIN (long negative value)
                 * Player MIN: will keep the optimal value if this value is less than 'maxValue' since maxValue == INT_MAX (long positive value)
                 * */
                int optimal = findOptimalValueForNextPlayer(player != PLAYER_MAX ? PLAYER_MAX : PLAYER_MIN, node->board);
                node->board[y][x] = 0;
                if(player == PLAYER_MAX) {
                    if(optimal > maxValue) {
                        maxValue = optimal;
                        moveNode = new Node(node->board);
                        moveNode->move.y = y;
                        moveNode->move.x = x;
                    }
                } else if(player == PLAYER_MIN) {
                    if(optimal <= maxValue) {
                        maxValue = optimal;
                        moveNode = new Node(node->board);
                        moveNode->move.y = y;
                        moveNode->move.x = x;
                    }
                }
            }
        }
    }
    if(moveNode != nullptr) {
        node->board[moveNode->move.y][moveNode->move.x] = player;
        node->move = moveNode->move;
        drawBoard(player, node);
        playerMove(player != PLAYER_MAX ? PLAYER_MAX : PLAYER_MIN, node);
    }
}

void drawBoard(char player, Node *node) {
    if(player == PLAYER_MAX) {
        std::cout << "> Max's move:" << "\n";
    } else if(player == PLAYER_MIN) {
        std::cout << "> Min's move:" << "\n";
    } else {
        std::cout << "Board initial state:" << "\n";
    }
    for(int y = 0; y < GRID_COL; y++) {
        for(int x = 0; x < GRID_ROW; x++) {
            std::cout << "  ";
            if(node->board[y][x] == PLAYER_MAX) {
                std::cout << PLAYER_MAX;
            } else if (node->board[y][x] == PLAYER_MIN) {
                std::cout << PLAYER_MIN;
            } else {
                std::cout << " ";
            }
            if(x != GRID_ROW - 1) {
                std::cout << "  |";
            }
        }
        std::string indicator = "\n";
        for(int c = 0; c < GRID_ROW; c++) {
            indicator += "------";
        }
        indicator += "\n";
        if((player == PLAYER_MAX || player == PLAYER_MIN) && y == node->move.y) {
            int indicatorIndex = 3;
            indicator[node->move.x == 0 ? indicatorIndex : indicatorIndex + node->move.x * 6] = '^';
            std::cout << indicator;
        } else {
            std::cout << indicator;
        }
    }
    std::cout << std::endl;
}