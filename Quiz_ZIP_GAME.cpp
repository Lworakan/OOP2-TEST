#include <iostream>
#include <vector>
#include <map>
using namespace std;

class Cell {
    bool visited;
    int checkpoint;
public:
    Cell() : visited(false), checkpoint(0) {}
    void setVisited(bool v) { visited = v; }
    bool isVisited() const { return visited; }
    void setCheckpoint(int c) { checkpoint = c; }
    int getCheckpoint() const { return checkpoint; }
    bool isCheckpoint() const { return checkpoint > 0; }
};

class Grid {
    int size;
    vector<vector<Cell>> cells;
    map<int, pair<int,int>> checkpoints;
public:
    Grid(int s) : size(s), cells(s, vector<Cell>(s)) {}

    void addCheckpoint(int num, int r, int c) {
        checkpoints[num] = {r, c};
        cells[r][c].setCheckpoint(num);
    }

    Cell& getCell(int r, int c) { return cells[r][c]; }
    pair<int,int> getCheckpoint(int num) { return checkpoints[num]; }

    bool isValid(int r, int c) const {
        return r >= 0 && r < size && c >= 0 && c < size && !cells[r][c].isVisited();
    }

    bool allVisited() const {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (!cells[i][j].isVisited()) return false;
        return true;
    }

    int getSize() const { return size; }

    void display() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (cells[i][j].isCheckpoint())
                    cout << "[" << cells[i][j].getCheckpoint() << "]";
                else if (cells[i][j].isVisited())
                    cout << "[*]";
                else
                    cout << "[ ]";
            }
            cout << endl;
        }
    }
};

class ZipGame {
    Grid grid;
    vector<pair<int,int>> path;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    bool backtrack(int r, int c, int checkpoint) {
        if (grid.allVisited() && grid.getCell(r, c).getCheckpoint() == 8)
            return true;

        int next = (checkpoint < 8) ? checkpoint + 1 : -1;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];

            if (!grid.isValid(nr, nc)) continue;

            Cell& cell = grid.getCell(nr, nc);

            if (cell.isCheckpoint() && next != -1 && cell.getCheckpoint() != next)
                continue;

            int curCp = checkpoint;
            if (cell.isCheckpoint() && cell.getCheckpoint() == next)
                curCp = next;

            grid.getCell(nr, nc).setVisited(true);
            path.push_back({nr, nc});

            if (backtrack(nr, nc, curCp)) return true;

            path.pop_back();
            grid.getCell(nr, nc).setVisited(false);
        }
        return false;
    }

public:
    ZipGame(int size) : grid(size) {}

    void addCheckpoint(int num, int r, int c) {
        grid.addCheckpoint(num, r, c);
    }

    bool solve() {
        auto start = grid.getCheckpoint(1);
        path.clear();
        path.push_back(start);
        grid.getCell(start.first, start.second).setVisited(true);
        return backtrack(start.first, start.second, 1);
    }

    void showSolution() {
        cout << "\nSOLUTION:\n";
        grid.display();
        cout << "\nPath: " << path.size() << " cells\n";
    }
};

int main() {
    ZipGame game(8);

    game.addCheckpoint(1, 0, 1);
    game.addCheckpoint(2, 7, 6);
    game.addCheckpoint(3, 5, 1);
    game.addCheckpoint(4, 2, 1);
    game.addCheckpoint(5, 4, 6);
    game.addCheckpoint(6, 4, 2);
    game.addCheckpoint(7, 2, 6);
    game.addCheckpoint(8, 3, 4);



    if (game.solve())
        game.showSolution();
    else
        cout << "error";

    return 0;
}
