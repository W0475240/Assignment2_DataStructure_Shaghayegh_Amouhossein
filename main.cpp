#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

// initionalizing
const char WALL = '+';
const char PATH = ' ';
const char USED = '#';  // Mark for solved path
const char DEAD = 'D';  // Mark for dead ends
const char START = 'S';
const char END = 'E';

// A structure for points in the maze
struct Point {
    int row, col;
    Point(int r, int c) : row(r), col(c) {}
    Point() : row(0), col(0) {}
    bool equals(Point p) {
        return row == p.row && col == p.col;
    }
};

// Function that allow to read the maze from a file and dynamically allocate space for it
std::vector<std::vector<char>> readMaze(const std::string &filename, int &rows, int &cols) {
    std::ifstream file(filename);
    std::vector<std::vector<char>> maze;

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            cols = line.size();
            maze.push_back(std::vector<char>(line.begin(), line.end()));
        }
        rows = maze.size();
        file.close();
    } else {
        std::cerr << "Unable to open the maze file." << std::endl;
    }
    return maze;
}

// Function to print the maze to the console (I added for debugging)
void printMaze(const std::vector<std::vector<char>> &maze) {
    for (const auto &row : maze) {
        for (const auto &ch : row) {
            std::cout << ch;
        }
        std::cout << '\n';
    }
}

// Function to save the solved maze to a file
void saveSolvedMaze(const std::vector<std::vector<char>> &maze, const std::string &filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto &row : maze) {
            for (const auto &ch : row) {
                file << ch;
            }
            file << '\n';
        }
        file.close();
    } else {
        std::cerr << "Unable to save the maze file." << std::endl;
    }
}

// Check if a move is valid
bool isValidMove(const std::vector<std::vector<char>> &maze, int row, int col) {
    return row >= 0 && row < maze.size() && col >= 0 && col < maze[0].size() && maze[row][col] == PATH;
}

// Non-recursive DFS to solve the maze
bool solveMaze(std::vector<std::vector<char>> &maze, Point start, Point end) {
    std::stack<Point> s;
    s.push(start);
    maze[start.row][start.col] = START;

    int dirX[] = {0, 1, 0, -1};  // East, South, West, North
    int dirY[] = {1, 0, -1, 0};

    while (!s.empty()) {
        Point current = s.top();
        int x = current.row;
        int y = current.col;

        if (current.equals(end)) {
            maze[x][y] = END;
            return true;
        }

        bool moveMade = false;
        for (int i = 0; i < 4; ++i) {
            int newX = x + dirX[i];
            int newY = y + dirY[i];

            if (isValidMove(maze, newX, newY)) {
                s.push(Point(newX, newY));
                maze[newX][newY] = USED;  // Mark as part of the solution path
                moveMade = true;
                break;
            }
        }

        if (!moveMade) {
            // Mark current position as a dead-end and backtrack
            maze[x][y] = DEAD;
            s.pop();
        }
    }
    return false;  // No solution found
}

int main() {

    int rows = 0, cols = 0;
    std::string mazeFile = "/Users/shaghayegh/Desktop/NSCC 2024 Sep/Data Structures/Assignment 2/tests/test.txt";
    std::vector<std::vector<char>> maze = readMaze(mazeFile, rows, cols);

    // Define start and end points based on the assignment specification
    Point start(1, 0);  // Entry at top-left corner
    Point end(rows - 2, cols - 1);  // Exit at bottom-right corner

    // Print the original maze for debugging
    std::cout << "Original Maze:\n";
    printMaze(maze);

    // Solve the maze using non-recursive DFS
    if (solveMaze(maze, start, end)) {
        std::cout << "Maze solved!\n";
        printMaze(maze);  // Print the solved maze
        saveSolvedMaze(maze, "/Users/shaghayegh/Desktop/NSCC 2024 Sep/Data Structures/Assignment 2/solved/test_solved.txt");
    } else {
        std::cout << "No solution found for the maze.\n";
    }

    return 0;
}
