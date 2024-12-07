#include "../utils/utils.h"
#include <execution>
#include <bitset>
#include <vector>
#include <unordered_set>

#include "p2.h"

enum {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

inline void move1(const std::bitset<17000>& walls, const int width, const int height, int& x, int& y, int& dir, std::vector<bool>& vis) {
    static const int dx[] = { 0, 1, 0, -1 };
    static const int dy[] = { -1, 0, 1, 0 };

    int next_x = x + dx[dir];
    int next_y = y + dy[dir];
    int next_pos = next_x + next_y * width;

    while (!walls[next_pos]) {
        vis[next_pos] = true;
        x = next_x;
        y = next_y;
        next_x = x + dx[dir];
        next_y = y + dy[dir];
        next_pos = next_x + next_y * width;
        if (next_pos < 0 || next_pos >= height)
            break;
    }
    if ((next_pos >= 0 || next_pos < height) && walls[next_pos]) {
        dir = (dir + 1) & 3;
    }
}

inline void move2(const std::bitset<17000>& walls, const int width, int& x, int& y, int& dir, int op = -1) {
    static const int dx[] = { 0, 1, 0, -1 };
    static const int dy[] = { -1, 0, 1, 0 };

    int next_x = x + dx[dir];
    int next_y = y + dy[dir];
    int next_pos = next_x + next_y * width;

    if (next_pos != op && !walls[next_pos]) {
        x = next_x;
        y = next_y;
    }
    else if (walls[next_pos] || next_pos == op) {
        dir = (dir + 1) & 3;
    }
}

bool creates_loop(const std::bitset<17000>& walls, int width, int height, int start_x, int start_y, int obstacle_x, int obstacle_y) {
    if (walls[obstacle_x + obstacle_y * width] ||
        (obstacle_x == start_x && obstacle_y == start_y)) {
        return false;
    }

    int obstacle_pos = obstacle_x + obstacle_y * width;

    std::vector<bool> visited(width * height, false);
    int x = start_x;
    int y = start_y;
    int dir = UP;

    std::vector<uint8_t> hist(width * height, 0);
    hist[x + y * width] |= 1 << dir;

    visited[x + y * width] = true;
    while ((x != 0 || dir != LEFT) &&
        (x != width - 2 || dir != RIGHT) &&
        (y != 0 || dir != UP) &&
        (y != height - 1 || dir != DOWN)) {

        move2(walls, width, x, y, dir, obstacle_pos);
        visited[x + y * width] = true;

        if (hist[x + y * width] & 1 << dir)
            return true;


        hist[x + y * width] |= 1 << dir;

    }

    return false;
}

int main() {

    INITIALIZE_AOC_TIMERS();

    default_timer.begin(0);
    std::string in = aoc_utils::memory_map_file_boost("input.txt").value();

    int width = in.find('\n');
    in.erase(std::remove(in.begin(), in.end(), '\n'), in.end());

    int start_pos = in.find('^');
    int start_x = start_pos % width;
    int start_y = start_pos / width;

    std::vector<uint8_t> grid(in.begin(), in.end());

    int height = grid.size() / width;

    std::bitset<17000> walls{ 0 };
    for (int y = 0; y < grid.size() / width; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[x + y * width] == '#') {
                walls[x + y * width] = true;
            }
        }
    }

    default_timer.end(0);
    
	// Part 1
    default_timer.begin(1);
    std::vector<bool> visited(grid.size(), false);
    int x = start_x;
    int y = start_y;
    int dir = UP;

    visited[start_pos] = true;
    while ((x != 0 || dir != LEFT) &&
        (x != width - 2 || dir != RIGHT) &&
        (y != 0 || dir != UP) &&
        (y != grid.size() / width - 1 || dir != DOWN)) {
        visited[x + y * width] = true;
        move1(walls, width, height, x, y, dir, visited);
    }

    int p1 = std::count(std::execution::unseq, visited.begin(), visited.end(), true);
	default_timer.end(1);

    // Part 2
    std::vector<std::pair<int, int>> candidates;
    for (int y = 0; y < grid.size() / width; y++) {
        for (int x = 0; x < width; x++) {
            if (visited[x + y * width]) {
                candidates.emplace_back(x, y);
            }
        }
    }

    default_timer.begin(2);
    int i = 0;
    int p2 = std::transform_reduce(
        std::execution::par_unseq,
        candidates.begin(),
        candidates.end(),
        0,
        std::plus<>(),
        [&](const auto& pos) {
            int j = creates_loop(walls, width, height, start_x, start_y, pos.first, pos.second) ? 1 : 0;
            i += j;
            return j;
        }
    );
    default_timer.end(2);

    std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';
    

    default_timer.display_all();
}

//Part 1: 5067
//Part 2 : 1793
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 137.7 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 51.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 12796.8 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 28
//Ticks : 280547
//TotalDays : 3.24707175925926E-07
//TotalHours : 7.79297222222222E-06
//TotalMinutes : 0.000467578333333333
//TotalSeconds : 0.0280547
//TotalMilliseconds : 28.0547