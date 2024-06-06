#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

class Sudoku {
private:
    vector<vector<int>> board;
    int gridSize;
    int boxSize;
    int emptyCells;

    void clearBoard() {
        board = vector<vector<int>>(gridSize, vector<int>(gridSize, 0));
    }

    bool isSafe(int row, int col, int num) {
        for (int x = 0; x < gridSize; x++) {
            if (board[row][x] == num || board[x][col] == num)
                return false;
        }

        int startRow = row - row % boxSize, startCol = col - col % boxSize;
        for (int i = 0; i < boxSize; i++)
            for (int j = 0; j < boxSize; j++)
                if (board[i + startRow][j + startCol] == num)
                    return false;

        return true;
    }

    bool fillBoard(int row, int col) {
        if (row == gridSize - 1 && col == gridSize)
            return true;
        if (col == gridSize) {
            row++;
            col = 0;
        }
        if (board[row][col] != 0)
            return fillBoard(row, col + 1);

        vector<int> nums(gridSize);
        iota(nums.begin(), nums.end(), 1); // Fill nums with 1 to gridSize
        random_shuffle(nums.begin(), nums.end()); // Shuffle nums

        for (int num : nums) {
            if (isSafe(row, col, num)) {
                board[row][col] = num;
                if (fillBoard(row, col + 1))
                    return true;
            }
            board[row][col] = 0;
        }
        return false;
    }

    void removeDigits() {
        int count = emptyCells;
        while (count != 0) {
            int cellId = rand() % (gridSize * gridSize);
            int i = cellId / gridSize;
            int j = cellId % gridSize;
            if (board[i][j] != 0) {
                int backup = board[i][j];
                board[i][j] = 0;

                // Check if the board still has a unique solution
                if (!hasUniqueSolution()) {
                    board[i][j] = backup; // Restore the cell if there's no unique solution
                } else {
                    count--;
                }
            }
        }
    }

    bool hasUniqueSolution() {
        vector<vector<int>> copy = board;
        return solve(copy);
    }

    bool solve(vector<vector<int>> &grid) {
        int row, col;
        if (!findUnassignedLocation(grid, row, col))
            return true; // success!

        for (int num = 1; num <= gridSize; num++) {
            if (isSafe(grid, row, col, num)) {
                grid[row][col] = num;
                if (solve(grid))
                    return true;
                grid[row][col] = 0;
            }
        }
        return false;
    }

    bool findUnassignedLocation(vector<vector<int>> &grid, int &row, int &col) {
        for (row = 0; row < gridSize; row++)
            for (col = 0; col < gridSize; col++)
                if (grid[row][col] == 0)
                    return true;
        return false;
    }

    bool isSafe(vector<vector<int>> &grid, int row, int col, int num) {
        for (int x = 0; x < gridSize; x++) {
            if (grid[row][x] == num || grid[x][col] == num)
                return false;
        }

        int startRow = row - row % boxSize, startCol = col - col % boxSize;
        for (int i = 0; i < boxSize; i++)
            for (int j = 0; j < boxSize; j++)
                if (grid[i + startRow][j + startCol] == num)
                    return false;

        return true;
    }

public:
    Sudoku(int size, int difficulty) {
        gridSize = size;
        boxSize = sqrt(size);
        board = vector<vector<int>>(gridSize, vector<int>(gridSize, 0));

        switch (difficulty) {
            case 1: emptyCells = 20; break; // Easy
            case 2: emptyCells = 40; break; // Medium
            case 3: emptyCells = 60; break; // Hard
            default: emptyCells = 20; break;
        }
    }

    void generateBoard() {
        clearBoard();
        fillBoard(0, 0);
        removeDigits();
    }

    void printBoard() {
        for (int r = 0; r < gridSize; r++) {
            for (int d = 0; d < gridSize; d++) {
                if (board[r][d] == 0) 
                    cout << ". ";
                else 
                    cout << board[r][d] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void solveSudoku() {
        vector<vector<int>> copy = board;
        if (solve(copy)) {
            board = copy;
            printBoard();
        } else {
            cout << "No solution exists!" << endl;
        }
    }

    void playGame() {
        int choice;
        cout << "Choose an option: (1) Solve automatically (2) Play game: ";
        cin >> choice;

        if (choice == 1) {
            solveSudoku();
        } else {
            int row, col, num;
            while (true) {
                printBoard();
                cout << "Enter your move (row col num) or -1 to exit: ";
                cin >> row;
                if (row == -1) break;
                cin >> col >> num;

                if (row >= 0 && row < gridSize && col >= 0 && col < gridSize && num >= 1 && num <= gridSize && board[row][col] == 0 && isSafe(board, row, col, num)) {
                    board[row][col] = num;
                    if (isComplete()) {
                        printBoard();
                        cout << "Congratulations! You have solved the Sudoku puzzle!" << endl;
                        break;
                    }
                } else {
                    cout << "Invalid move. Try again." << endl;
                }
            }
        }
    }

    bool isComplete() {
        for (int r = 0; r < gridSize; r++) {
            for (int d = 0; d < gridSize; d++) {
                if (board[r][d] == 0)
                    return false;
            }
        }
        return true;
    }
};

int main() {
    srand(time(0));

    int size = 9; // 9x9 Sudoku
    int difficulty;
    cout << "Select difficulty level: (1) Easy (2) Medium (3) Hard: ";
    cin >> difficulty;

    Sudoku sudoku(size, difficulty);
    sudoku.generateBoard();
    sudoku.playGame();

    return 0;
}
