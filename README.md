🚀 Maze Generator Implementation with Obstacle System
📋 Main Data Structures in Code
Main Maze Map
vector<vector<char>> maze;  // Two-dimensional vector for maze storage

Positions and Directions
pair<int, int> playerPos;  // Player position
pair<int, int> exitPos;    // Exit position
const int dx[4] = {0, 1, 0, -1};  // Direction vectors
const int dy[4] = {-1, 0, 1, 0};

🔬 Algorithms
1. Maze Generation (generate method)
Algorithm: Modified DFS
Structures:
stack<pair<int, int>> stack;  // Stack for path tracking
vector<int> directions = {0, 1, 2, 3};  // Directions for random selection

Working Principle:
Start from point (1,1)
Randomly choose direction
"Carve" walls through one cell
Backtrack when no paths available
2. Path Checking (isPathPossible method)
Algorithm: BFS
Structures:
vector<vector<bool>> visited;  // Visited matrix
queue<pair<int, int>> q;      // Queue for BFS

Application: Exit reachability verification
3. Trap Placement (addRandomTraps method)
Algorithm: BFS + Random Placement
Structures:
vector<vector<bool>> safePath;  // Safe path
vector<vector<pair<int, int>>> parent;  // Parent connections

Random Position Generation:
mt19937 gen(rd());
uniform_int_distribution<> disW(1, width-2);
uniform_int_distribution<> disH(1, height-2);

📊 Algorithm Complexity
Maze Generation: O(W*H)
Path Checking: O(W*H)
Trap Placement: O(W*H + T), where T is number of traps
🎮 Game Mechanics
Controls: WASD
Lives System (3 lives)
Traps (*)
Colored console output
