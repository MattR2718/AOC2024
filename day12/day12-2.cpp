#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>


struct IsValidPosition {
    inline static int minX{};
    inline static int minY{};
    inline static int maxX{ std::numeric_limits<int>::max() };
    inline static int maxY{ std::numeric_limits<int>::max() };

    static bool check(int x, int y) {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    }
};


void findAllPlantsInRegion(
    const std::vector<std::string>& gardenMap,
    std::vector<std::vector<bool>>& visited,
    const char plantType,
    int& area,
    int& perimeter,
    const int x,
    const int y
) {
    if (
        !IsValidPosition::check(x, y) ||
        gardenMap.at(x).at(y) != plantType
        ) {
        perimeter++;
        return;
    }

    if (visited.at(x).at(y)) {
        return;
    }

    visited.at(x).at(y) = true;
    area++;

    findAllPlantsInRegion(gardenMap, visited, plantType, area, perimeter, x + 1, y);
    findAllPlantsInRegion(gardenMap, visited, plantType, area, perimeter, x - 1, y);
    findAllPlantsInRegion(gardenMap, visited, plantType, area, perimeter, x, y + 1);
    findAllPlantsInRegion(gardenMap, visited, plantType, area, perimeter, x, y - 1);
}


int solutionPart1(const char* inputPath) {
    std::ifstream input(inputPath);

    std::vector<std::string> gardenMap;
    for (std::string line; std::getline(input, line); gardenMap.push_back(line));

    IsValidPosition::maxX = gardenMap.size() - 1;
    IsValidPosition::maxY = gardenMap.front().length() - 1;

    std::vector<std::vector<bool>> visited(
        IsValidPosition::maxX + 1, std::vector<bool>(IsValidPosition::maxY + 1, false)
    );

    int totalPriceOfFencing{};
    for (int i{}; i < IsValidPosition::maxX + 1; ++i) {
        for (int j{}; j < IsValidPosition::maxY + 1; ++j) {
            if (visited.at(i).at(j)) {
                continue;
            }

            int area{}, perimeter{};
            findAllPlantsInRegion(gardenMap, visited, gardenMap.at(i).at(j), area, perimeter, i, j);
            totalPriceOfFencing += area * perimeter;
        }
    }

    return totalPriceOfFencing;
}


struct RegionBounds {
    int minX{ std::numeric_limits<int>::max() };
    int minY{ std::numeric_limits<int>::max() };
    int maxX{};
    int maxY{};
};


void findAllPlantsInRegion(
    const std::vector<std::string>& gardenMap,
    std::vector<std::vector<bool>>& visitedInGarden,
    std::vector<std::vector<bool>>& visitedInRegion,
    const char plantType,
    int& area,
    RegionBounds& regionBounds,
    const int x,
    const int y
) {
    if (
        !IsValidPosition::check(x, y) ||
        gardenMap.at(x).at(y) != plantType
        ) {
        regionBounds.minX = std::min(regionBounds.minX, x);
        regionBounds.maxX = std::max(regionBounds.maxX, x);
        regionBounds.minY = std::min(regionBounds.minY, y);
        regionBounds.maxY = std::max(regionBounds.maxY, y);

        return;
    }

    if (visitedInRegion.at(x).at(y)) {
        return;
    }

    visitedInGarden.at(x).at(y) = visitedInRegion.at(x).at(y) = true;
    area++;

    findAllPlantsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x + 1, y);
    findAllPlantsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x - 1, y);
    findAllPlantsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x, y + 1);
    findAllPlantsInRegion(gardenMap, visitedInGarden, visitedInRegion, plantType, area, regionBounds, x, y - 1);
}


int solutionPart2(const char* inputPath) {

    int num = 0;

    std::ifstream input(inputPath);

    std::vector<std::string> gardenMap;
    for (std::string line; std::getline(input, line); gardenMap.push_back(line));

    IsValidPosition::maxX = gardenMap.size() - 1;
    IsValidPosition::maxY = gardenMap.front().length() - 1;

    std::vector<std::vector<bool>> visitedInGarden(
        IsValidPosition::maxX + 1, std::vector<bool>(IsValidPosition::maxY + 1, false)
    );

    int totalPriceOfFencing{};
    for (int i{}; i < IsValidPosition::maxX + 1; ++i) {
        for (int j{}; j < IsValidPosition::maxY + 1; ++j) {
            if (visitedInGarden.at(i).at(j)) {
                continue;
            }

            int area{};
            RegionBounds regionBounds;
            std::vector<std::vector<bool>> visitedInRegion(
                IsValidPosition::maxX + 1, std::vector<bool>(IsValidPosition::maxY + 1, false)
            );
            findAllPlantsInRegion(
                gardenMap,
                visitedInGarden,
                visitedInRegion,
                gardenMap.at(i).at(j),
                area,
                regionBounds,
                i,
                j
            );

            int numOfSides{};
            for (int k{ regionBounds.minX }; k < regionBounds.maxX; ++k) {
                for (int l{ regionBounds.minY }; l < regionBounds.maxY; ++l) {
                    bool topLeft{
                        IsValidPosition::check(k, l) &&
                        visitedInRegion.at(k).at(l) &&
                        gardenMap.at(k).at(l) == gardenMap.at(i).at(j)
                    },
                        topRight{
                            IsValidPosition::check(k, l + 1) &&
                            visitedInRegion.at(k).at(l + 1) &&
                            gardenMap.at(k).at(l + 1) == gardenMap.at(i).at(j)
                    },
                        bottomLeft{
                            IsValidPosition::check(k + 1, l) &&
                            visitedInRegion.at(k + 1).at(l) &&
                            gardenMap.at(k + 1).at(l) == gardenMap.at(i).at(j)
                    },
                        bottomRight{
                            IsValidPosition::check(k + 1, l + 1) &&
                            visitedInRegion.at(k + 1).at(l + 1) &&
                            gardenMap.at(k + 1).at(l + 1) == gardenMap.at(i).at(j)
                    };

                    int numOfSamePlantsIn2x2Window{
                        topLeft + topRight + bottomLeft + bottomRight
                    };

                    numOfSides += numOfSamePlantsIn2x2Window % 2 == 1;

                    if (numOfSamePlantsIn2x2Window == 2) {
                        numOfSides += 2 * ((topLeft && bottomRight) || (bottomLeft && topRight));
                    }
                }
            }

            totalPriceOfFencing += area * numOfSides;
            num++;
        }
    }

	std::cout << num << "\n";

    return totalPriceOfFencing;
}



int main() {
	const char* inputPath = "input.txt";
	auto start = std::chrono::high_resolution_clock::now();
	auto part1 = solutionPart1(inputPath);
	auto part2 = solutionPart2(inputPath);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
	std::cout << "Execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
	return 0;
}