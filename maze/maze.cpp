#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <queue>
using namespace std;

class MazeGenerator {
private:
    int width;
    int height;
    vector<vector<char>> maze;
    pair<int, int> playerPos;
    pair<int, int> exitPos;
    int lives;

    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };

    void setConsoleColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    bool isPathPossible() {
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        queue<pair<int, int>> q;
        q.push(playerPos);
        visited[playerPos.second][playerPos.first] = true;

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current == exitPos) return true;

            for (int i = 0; i < 4; i++) {
                int nx = current.first + dx[i];
                int ny = current.second + dy[i];

                if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                    !visited[ny][nx] && (maze[ny][nx] == ' ' || maze[ny][nx] == '*')) {
                    visited[ny][nx] = true;
                    q.push({ nx, ny });
                }
            }
        }
        return false;
    }

public:
    MazeGenerator(int w, int h) : width(w), height(h), lives(3) {
        maze.resize(height, vector<char>(width, '#'));
        playerPos = { 1, 0 };
        exitPos = { width - 2, height - 1 };
    }

    void generate() {
        random_device rd;
        mt19937 gen(rd());

        int startX = 1;
        int startY = 1;

        stack<pair<int, int>> stack;
        stack.push({ startX, startY });
        maze[startY][startX] = ' ';

        while (!stack.empty()) {
            int x = stack.top().first;
            int y = stack.top().second;

            vector<int> directions = { 0, 1, 2, 3 };
            shuffle(directions.begin(), directions.end(), gen);

            bool found = false;

            for (int dir : directions) {
                int nx = x + dx[dir] * 2;
                int ny = y + dy[dir] * 2;

                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#') {
                    maze[y + dy[dir]][x + dx[dir]] = ' ';
                    maze[ny][nx] = ' ';
                    stack.push({ nx, ny });
                    found = true;
                    break;
                }
            }

            if (!found) {
                stack.pop();
            }
        }

        maze[0][1] = ' ';  
        maze[height - 1][width - 2] = ' ';  
    }

    void display() {
        system("cls");
        cout << "Lives: " << lives << "\n\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == playerPos.first && y == playerPos.second) {
                    setConsoleColor(10); 
                    cout << "P ";
                    setConsoleColor(7);
                }
                else if (x == exitPos.first && y == exitPos.second) {
                    setConsoleColor(12); 
                    cout << "E ";
                    setConsoleColor(7);
                }
                else if (maze[y][x] == '*') {
                    setConsoleColor(4); 
                    cout << maze[y][x] << ' ';
                    setConsoleColor(7);
                }
                else {
                    if (maze[y][x] == '#') {
                        setConsoleColor(9);
                        cout << maze[y][x] << ' ';
                        setConsoleColor(7);
                    }
                    else {
                        cout << maze[y][x] << ' ';
                    }
                }
            }
            cout << '\n';
        }
    }

    bool movePlayer(char direction) {
        int newX = playerPos.first;
        int newY = playerPos.second;

        switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        }

        if (newX >= 0 && newX < width && newY >= 0 && newY < height &&
            (maze[newY][newX] == ' ' || maze[newY][newX] == '*')) {
            if (maze[newY][newX] == '*') {
                lives--;
                maze[newY][newX] = ' ';
            }
            playerPos = { newX, newY };
            return true;
        }
        return false;
    }

    bool isFinished() {
        return playerPos.first == exitPos.first && playerPos.second == exitPos.second;
    }

    bool isDead() {
        return lives <= 0;
    }

    void addRandomTraps(int numTraps) {
        random_device rd;
        mt19937 gen(rd());

        vector<vector<bool>> safePath(height, vector<bool>(width, false));
        queue<pair<int, int>> q;
        vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width));

        q.push(playerPos);
        safePath[playerPos.second][playerPos.first] = true;

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current == exitPos) break;

            for (int i = 0; i < 4; i++) {
                int nx = current.first + dx[i];
                int ny = current.second + dy[i];

                if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                    maze[ny][nx] == ' ' && !safePath[ny][nx]) {
                    safePath[ny][nx] = true;
                    parent[ny][nx] = current;
                    q.push({ nx, ny });
                }
            }
        }

        pair<int, int> current = exitPos;
        while (current != playerPos) {
            safePath[current.second][current.first] = true;
            current = parent[current.second][current.first];
        }

        uniform_int_distribution<> disW(1, width - 2);
        uniform_int_distribution<> disH(1, height - 2);

        int trapsPlaced = 0;
        int attempts = 0;
        const int MAX_ATTEMPTS = width * height * 2;

        while (trapsPlaced < numTraps && attempts < MAX_ATTEMPTS) {
            int x = disW(gen);
            int y = disH(gen);
            attempts++;

            if (maze[y][x] == ' ' && !safePath[y][x] &&
                !(x == playerPos.first && y == playerPos.second) &&
                !(x == exitPos.first && y == exitPos.second)) {
                maze[y][x] = '*';
                trapsPlaced++;
            }
        }
    }
};

int main() {
    cout << "Enter maze width (odd number): ";
    int width;
    cin >> width;
    if (width % 2 == 0) width++;

    cout << "Enter maze height (odd number): ";
    int height;
    cin >> height;
    if (height % 2 == 0) height++;

    MazeGenerator maze(width, height);
    maze.generate();

    cout << "Add traps? (y/n): ";
    char addTraps;
    cin >> addTraps;
    if (addTraps == 'y') {
        maze.addRandomTraps(width * height / 10); 
    }

    cout << "\nUse WASD to move, Q to quit\n";
    cout << "Watch out for traps (*) - they will cost you a life!\n";
    cout << "Press any key to start...\n";
    _getch();

    bool gameOver = false;
    while (!gameOver) {
        maze.display();

        char move = _getch();
        if (move == 'q') break;

        if (maze.movePlayer(move)) {
            if (maze.isFinished()) {
                maze.display();
                cout << "\nCongratulations! You've reached the exit!\n";
                gameOver = true;
            }
            else if (maze.isDead()) {
                maze.display();
                cout << "\nGame Over! You've lost all your lives!\n";
                gameOver = true;
            }
        }
    }

    return 0;
}
