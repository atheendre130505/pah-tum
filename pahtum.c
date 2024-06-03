#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 7
#define EMPTY ' '
#define PLAYER 'X'
#define COMPUTER 'O'

// Function prototypes
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void playerMove(char board[BOARD_SIZE][BOARD_SIZE]);
void computerMove(char board[BOARD_SIZE][BOARD_SIZE], int moveCount);
bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player);
int calculateScore(char board[BOARD_SIZE][BOARD_SIZE], char player);
bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]);
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
int evaluatePositionScore(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char symbol);
bool checkPotentialWin(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char symbol);

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    int playerScore = 0, computerScore = 0;
    bool playerTurn = true;
    int moveCount = 0;
    char choice;

    // Initialize the board
    initializeBoard(board);

    // Ask player if they want to go first or second
    printf("Do you want to go first or second? (1 for first, 2 for second): ");
    scanf(" %c", &choice);

    if (choice == '1') {
        playerTurn = true;
    } else if (choice == '2') {
        playerTurn = false;
        // Make computer's first move
        computerMove(board, moveCount++);
        printBoard(board);  // Show the board after computer's first move
        playerTurn = true;  // Ensure the next turn is the player's
    } else {
        printf("Invalid choice. Defaulting to first.\n");
        playerTurn = true;
    }

    // Game loop - stop only when the board is full
    while (!isBoardFull(board)) {
        printBoard(board);
        if (playerTurn) {
            playerMove(board);
        } else {
            computerMove(board, moveCount++);
        }

        // Toggle turns
        playerTurn = !playerTurn;

        // Calculate scores
        playerScore = calculateScore(board, PLAYER);
        computerScore = calculateScore(board, COMPUTER);
        printf("Player Score: %d, Computer Score: %d\n", playerScore, computerScore);
    }

    // Determine the outcome based on scores after the board is full
    printBoard(board);
    if (playerScore > computerScore) {
        printf("Player wins!\n");
    } else if (computerScore > playerScore) {
        printf("Computer wins!\n");
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void playerMove(char board[BOARD_SIZE][BOARD_SIZE]) {
    int x, y;
    do {
        printf("Enter your move (x,y): ");
        scanf("%d %d", &y, &x);
    } while (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || board[x][y] != EMPTY);

    board[x][y] = PLAYER;
}

void computerMove(char board[BOARD_SIZE][BOARD_SIZE], int moveCount) {
    if (moveCount == 0) {
        // Initial move by computer at the center or at (2, 3) if (3, 3) is taken
        if (board[3][3] == EMPTY) {
            board[3][3] = COMPUTER;
        } else {
            board[2][3] = COMPUTER;
        }
    } else {
        int bestScore = -1;
        int bestX = -1;
        int bestY = -1;

        // Check if there is a need to block the opponent from winning
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    // Check if placing here blocks a four-in-a-row for the opponent
                    board[i][j] = PLAYER;
                    if (checkPotentialWin(board, i, j, PLAYER)) {
                        board[i][j] = EMPTY; // Reset after check
                        board[i][j] = COMPUTER;
                        return;
                    }
                    board[i][j] = EMPTY;
                }
            }
        }

        // Evaluate the best position for COMPUTER if no immediate block is needed
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    int score = evaluatePositionScore(board, i, j, COMPUTER);
                    // Priority: block opponent's potential four-in-a-row
                    board[i][j] = PLAYER;
                    if (checkPotentialWin(board, i, j, PLAYER)) {
                        score += 100; // High priority to block
                    }
                    board[i][j] = EMPTY;

                    if (score > bestScore) {
                        bestScore = score;
                        bestX = i;
                        bestY = j;
                    }
                }
            }
        }

        if (bestX != -1 && bestY != -1) {
            board[bestX][bestY] = COMPUTER;
        } else {
            // Take the first empty spot if no strategic move found
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == EMPTY) {
                        board[i][j] = COMPUTER;
                        return;
                    }
                }
            }
        }
    }
}

// Placeholder for win check
bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    return false; // Updated to prevent stopping the game on five in a row
}

int calculateScore(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    int score = 0;
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};

    // Horizontal chains
    for (int i = 0; i < BOARD_SIZE; i++) {
        int maxChainLength = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                int chainLength = 1;
                while (j + chainLength < BOARD_SIZE && board[i][j + chainLength] == player) {
                    chainLength++;
                }
                if (chainLength > maxChainLength) {
                    maxChainLength = chainLength;
                }
                j += chainLength - 1; // Move to the end of the current chain
            }
        }
        if (maxChainLength >= 3) {
            score += calculateChainScore(maxChainLength);
        }
    }

    // Vertical chains
    for (int j = 0; j < BOARD_SIZE; j++) {
        int maxChainLength = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][j] == player) {
                int chainLength = 1;
                while (i + chainLength < BOARD_SIZE && board[i + chainLength][j] == player) {
                    chainLength++;
                }
                if (chainLength > maxChainLength) {
                    maxChainLength = chainLength;
                }
                i += chainLength - 1; // Move to the end of the current chain
            }
        }
        if (maxChainLength >= 3) {
            score += calculateChainScore(maxChainLength);
        }
    }

    return score;
}

int calculateChainScore(int chainLength) {
    if (chainLength == 3) return 3;
    if (chainLength == 4) return 10;
    if (chainLength == 5) return 25;
    if (chainLength == 6) return 56;
    if (chainLength == 7) return 119;
    return 0;
}

int evaluatePositionScore(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char symbol) {
    int score = 0;
  int score1=0,score2=0;
    int chainLength;

    // Horizontal chain evaluation
    chainLength = 1;
    for (int i = y - 1; i >= 0 && board[x][i] == symbol; i--) chainLength++;
    for (int i = y + 1; i < BOARD_SIZE && board[x][i] == symbol; i++) chainLength++;
    if (chainLength > 1) score1 += chainLength;

    // Vertical chain evaluation
    chainLength = 1;
    for (int i = x - 1; i >= 0 && board[i][y] == symbol; i--) chainLength++;
    for (int i = x + 1; i < BOARD_SIZE && board[i][y] == symbol; i++) chainLength++;
    if (chainLength > 1) score2 += chainLength;

  score=(score1>score2)?score1:score2;
    return score;
}

bool checkPotentialWin(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char symbol) {
    // Check horizontal and vertical for potential win condition
    int i, count;

    // Check horizontal
    count = 1; // Start with the current position
    for (i = y - 1; i >= 0 && board[x][i] == symbol; i--, count++);
    for (i = y + 1; i < BOARD_SIZE && board[x][i] == symbol; i++, count++);
    if (count >= 4) return true;

    // Check vertical
    count = 1; // Reset count for vertical check
    for (i = x - 1; i >= 0 && board[i][y] == symbol; i--, count++);
    for (i = x + 1; i < BOARD_SIZE && board[i][y] == symbol; i++, count++);
    if (count >= 4) return true;

    return false;
}

bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}