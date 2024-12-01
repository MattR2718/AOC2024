#include <gtest/gtest.h>
#include "utils.h"
#include <fstream>



namespace {

    // Helper function to create temporary test files
    std::string createTempFile(const std::vector<std::string>& lines) {
        std::string filename = "test_file.txt";
        std::ofstream file(filename);
        for (const auto& line : lines) {
            file << line << "\n";
        }
        file.close();
        return filename;
    }

    // Test Fixture
    class UtilsTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup code if needed
        }

        void TearDown() override {
            // Cleanup code if needed
            std::remove("test_file.txt");
        }
    };

    // Direction Vectors Tests
    TEST_F(UtilsTest, CardinalDirectionsTest) {
        EXPECT_EQ(aoc_utils::cardinal_directions.size(), 4);
        EXPECT_TRUE(std::find(aoc_utils::cardinal_directions.begin(),
            aoc_utils::cardinal_directions.end(),
            std::make_pair(0, -1)) != aoc_utils::cardinal_directions.end());
    }

    TEST_F(UtilsTest, DiagonalDirectionsTest) {
        EXPECT_EQ(aoc_utils::diagonal_directions.size(), 4);
        EXPECT_TRUE(std::find(aoc_utils::diagonal_directions.begin(),
            aoc_utils::diagonal_directions.end(),
            std::make_pair(-1, -1)) != aoc_utils::diagonal_directions.end());
    }

    TEST_F(UtilsTest, AllDirectionsTest) {
        EXPECT_EQ(aoc_utils::all_directions.size(), 8);
    }

    // CTRE Split Tests
    TEST_F(UtilsTest, SplitCtreBasicTest) {
        std::string input = "1,2,3,4,5";
		constexpr auto pattern = ctll::fixed_string{ "," };
        auto result = aoc_utils::split_ctre<std::string, pattern>(input, [](std::string s) { return s; });
        EXPECT_EQ(result.size(), 5);
        //EXPECT_EQ(result[0], "1");
    }

    TEST_F(UtilsTest, SplitCtreWithConversionTest) {
        std::string input = "1,2,3,4,5";
        constexpr auto pattern = ctll::fixed_string{ "," };
        auto result = aoc_utils::split_ctre<int, pattern>(input,
            [](const std::string& s) { return std::stoi(s); });
        EXPECT_EQ(result.size(), 5);
        //EXPECT_EQ(result[0], 1);
    }

    // Tokenize Tests
    TEST_F(UtilsTest, TokenizeBasicTest) {
        std::string input = "hello world\ttest";
        auto result = aoc_utils::tokenize(input, " \t");
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], "hello");
    }

    TEST_F(UtilsTest, TokenizeEmptyDelimitersTest) {
        std::string input = "hello";
        auto result = aoc_utils::tokenize(input, "");
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], "hello");
    }

    // File Reading Tests
    TEST_F(UtilsTest, ReadLinesTest) {
        std::vector<std::string> testLines = { "line1", "line2", "line3" };
        auto filename = createTempFile(testLines);
        auto result = aoc_utils::read_lines(filename);
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], "line1");
    }

    TEST_F(UtilsTest, MemoryMapFileTest) {
        std::vector<std::string> testLines = { "test content" };
        auto filename = createTempFile(testLines);
        auto result = aoc_utils::memory_map_file(filename);
        EXPECT_TRUE(result.has_value());
        EXPECT_TRUE(result->find("test content") != std::string::npos);
    }

    TEST_F(UtilsTest, ReadLinesMmapTest) {
        std::vector<std::string> testLines = { "line1", "line2", "line3" };
        auto filename = createTempFile(testLines);
        auto result = aoc_utils::read_lines_mmap<std::string>(filename);
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], "line1");
    }

    // Graph Algorithm Tests
    TEST_F(UtilsTest, AStarTest) {
        // Simple grid pathfinding test
        auto neighbors = [](std::pair<int, int> pos) -> std::vector<std::pair<std::pair<int, int>, int>> {
            std::vector<std::pair<std::pair<int, int>, int>> result;
            for (const auto& dir : aoc_utils::cardinal_directions) {
                result.push_back({ {pos.first + dir.first, pos.second + dir.second}, 1 });
            }
            return result;
            };

        auto heuristic = [](std::pair<int, int> a, std::pair<int, int> b) {
            return std::abs(a.first - b.first) + std::abs(a.second - b.second);
            };

        // Custom hash function for std::pair<int, int>
        auto custom_hash = [](const std::pair<int, int>& p) -> std::size_t {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
            };

        auto path = aoc_utils::a_star<std::pair<int, int>, int, decltype(custom_hash)>({ 0, 0 }, { 2, 2 }, neighbors, heuristic, custom_hash);
        EXPECT_TRUE(path.has_value());
        EXPECT_EQ(path->size(), 5); // Should be 4 steps to reach {2,2} from {0,0}
    }

    TEST_F(UtilsTest, DijkstraTest) {
        // Simple grid pathfinding test
        auto neighbors = [](std::pair<int, int> pos) -> std::vector<std::pair<std::pair<int, int>, int>> {
            std::vector<std::pair<std::pair<int, int>, int>> result;
            for (const auto& dir : aoc_utils::cardinal_directions) {
                result.push_back({ {pos.first + dir.first, pos.second + dir.second}, 1 });
            }
            return result;
            };

        // Custom hash function for std::pair<int, int>
        auto custom_hash = [](const std::pair<int, int>& p) -> std::size_t {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
            };

        auto path = aoc_utils::dijkstra<std::pair<int, int>, int, decltype(custom_hash)>({ 0, 0 }, { 2, 2 }, neighbors, custom_hash);
        EXPECT_TRUE(path.has_value());
        EXPECT_EQ(path->size(), 5); // Should be 4 steps to reach {2,2} from {0,0}
    }

    TEST_F(UtilsTest, DepthFirstSearchTest) {
        // Simple tree traversal test
        auto neighbors = [](int node) -> std::vector<int> {
            if (node == 1) return { 2, 3 };
            if (node == 2) return { 4, 5 };
            return {};
            };

        // Custom hash function for int
        auto custom_hash = [](const int& value) -> std::size_t {
            return std::hash<int>()(value);
            };

        auto result = aoc_utils::depth_first_search<int, decltype(custom_hash)>(1, neighbors, custom_hash);
        EXPECT_FALSE(result.empty());
        EXPECT_EQ(result[0], 1); // Root should be first
    }

    TEST_F(UtilsTest, BreadthFirstSearchTest) {
        // Simple tree traversal test
        auto neighbors = [](int node) -> std::vector<int> {
            if (node == 1) return { 2, 3 };
            if (node == 2) return { 4, 5 };
            return {};
            };

        // Custom hash function for int
        auto custom_hash = [](const int& value) -> std::size_t {
            return std::hash<int>()(value);
            };

        auto result = aoc_utils::breadth_first_search<int, decltype(custom_hash)>(1, neighbors, custom_hash);
        EXPECT_FALSE(result.empty());
        EXPECT_EQ(result[0], 1); // Root should be first
        // Check that nodes are visited in level order
        if (result.size() > 2) {
            EXPECT_TRUE(result[1] == 2 || result[1] == 3);
        }
    }


} // namespace