#include "../utils/utils.h"



#include "coin-or/CbcModel.hpp"
#include "coin-or/OsiClpSolverInterface.hpp"

#include "glpk.h"

struct Game {
    int64_t ax, ay, bx, by;
    int64_t xg, yg;

    void print() const {
        fmt::print("A: ({}, {}), B: ({}, {}), G: ({}, {})\n", ax, ay, bx, by, xg, yg);
    }
};

// Test function to solve a linear programming problem using CBC
void test() {
    OsiClpSolverInterface solver;

    // Number of variables (a and b)
    int numVars = 2;

    // Objective function: Minimize 3a + b (using int64_t but passing as double)
    double obj[] = { static_cast<double>(3), static_cast<double>(1) };
    solver.setObjective(obj);
    solver.setObjSense(1.0); // Minimize

    // Define constraints (using int64_t but converting to double for the solver)
    CoinPackedMatrix matrix;
    matrix.setDimensions(0, numVars);

    // Constraint 1: 94a + 22b = 8400 (using int64_t but passing as double)
    int row1Index[] = { 0, 1 };
    double row1Values[] = { static_cast<double>(94), static_cast<double>(22) };
    matrix.appendRow(2, row1Index, row1Values);

    // Constraint 2: 34a + 67b = 5400 (using int64_t but passing as double)
    int row2Index[] = { 0, 1 };
    double row2Values[] = { static_cast<double>(34), static_cast<double>(67) };
    matrix.appendRow(2, row2Index, row2Values);

    solver.loadProblem(matrix, nullptr, nullptr, nullptr, nullptr, nullptr);

    solver.setColBounds(0, -COIN_DBL_MAX, COIN_DBL_MAX); // a can be any real number
    solver.setColBounds(1, -COIN_DBL_MAX, COIN_DBL_MAX); // b can be any real number

    // Equality constraints are modeled as two inequalities
    solver.setRowBounds(0, 8400.0, 8400.0); // 94a + 22b = 8400
    solver.setRowBounds(1, 5400.0, 5400.0); // 34a + 67b = 5400

    // Solve using CBC
    CbcModel model(solver);
    model.branchAndBound(); // Use the correct solve method

    // Get and print the solution
    const double* solution = model.bestSolution();
    std::cout << "Optimal solution:\n";
    std::cout << "a = " << solution[0] << "\n";
    std::cout << "b = " << solution[1] << "\n";
}

int64_t solve_game(const Game& g) {
    //std::cout << "\n\n\n";
    //std::cout << "Solving game: \n";
    //g.print();

    OsiClpSolverInterface solver;

    int numVars = 2;

    double obj[] = { 3.0, 1.0 };
    solver.setObjective(obj);
    solver.setObjSense(1.0); // Minimize

    CoinPackedMatrix matrix;
    matrix.setDimensions(0, numVars);

    double row1Values[] = { static_cast<double>(g.ax), static_cast<double>(g.bx) };
    int row1Index[] = { 0, 1 };
    matrix.appendRow(2, row1Index, row1Values);

    double row2Values[] = { static_cast<double>(g.ay), static_cast<double>(g.by) };
    int row2Index[] = { 0, 1 };
    matrix.appendRow(2, row2Index, row2Values);

    solver.loadProblem(matrix, nullptr, nullptr, nullptr, nullptr, nullptr);

    solver.setInteger(0);
    solver.setInteger(1);

    // Set bounds for variables
    solver.setColBounds(0, -COIN_DBL_MAX, COIN_DBL_MAX);
    solver.setColBounds(1, -COIN_DBL_MAX, COIN_DBL_MAX);

    solver.setRowBounds(0, static_cast<double>(g.xg), static_cast<double>(g.xg));
    solver.setRowBounds(1, static_cast<double>(g.yg), static_cast<double>(g.yg));

    // Solve using CBC
    CbcModel model(solver);
    model.branchAndBound();

    // Get and print the solution
    const double* solution = model.bestSolution();
    if (solution == nullptr) {
        //std::cout << "No solution found\n";
        return 0;
    }

    //std::cout << "Optimal solution:\n";
    //std::cout << "a = " << solution[0] << "\n";
    //std::cout << "b = " << solution[1] << "\n";

    // Return the objective value as an integer
    return static_cast<int64_t>(solution[0] * 3 + solution[1]);
}


int64_t solve_game_with_glpk(const Game& g) {
    std::cout << "\n\n\n";
    std::cout << "Solving game using GLPK: \n";
    g.print();

    // Scale factor for large numbers
    const int64_t scale_factor = 1000000000;

    glp_prob* lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "aoc");

    int numVars = 2;

    // Add variables to the problem
    glp_add_cols(lp, numVars);
    glp_set_col_name(lp, 1, "a");
    glp_set_col_name(lp, 2, "b");
    glp_set_col_kind(lp, 1, GLP_IV);
    glp_set_col_kind(lp, 2, GLP_IV);

    // Objective function: Minimize 3a + b
    glp_set_obj_coef(lp, 1, 3.0);
    glp_set_obj_coef(lp, 2, 1.0);
    glp_set_obj_dir(lp, GLP_MIN);

    glp_add_rows(lp, 2);

    glp_set_row_name(lp, 1, "Constraint 1");
    glp_set_row_bnds(lp, 1, GLP_FX, static_cast<double>(g.xg) / scale_factor, static_cast<double>(g.xg) / scale_factor);

    int ind1[] = { 1, 2 };
    double val1[] = { static_cast<double>(g.ax) / scale_factor, static_cast<double>(g.bx) / scale_factor };

    glp_set_mat_row(lp, 1, 2, ind1, val1);

    glp_set_row_name(lp, 2, "Constraint 2");
    glp_set_row_bnds(lp, 2, GLP_FX, static_cast<double>(g.yg) / scale_factor, static_cast<double>(g.yg) / scale_factor);


    int ind2[] = { 1, 2 };
    double val2[] = { static_cast<double>(g.ay) / scale_factor, static_cast<double>(g.by) / scale_factor };

    // Set the second constraint
    glp_set_mat_row(lp, 2, 2, ind2, val2);

    // Solve the problem
    glp_simplex(lp, NULL);
    glp_intopt(lp, NULL);

    // Check the solution status
    int status = glp_get_status(lp);
    if (status == GLP_OPT) {
        // Solution is optimal
        double a = glp_get_col_prim(lp, 1);
        double b = glp_get_col_prim(lp, 2);
        std::cout << "Optimal solution found:\n";
        std::cout << "a = " << a << "\n";
        std::cout << "b = " << b << "\n";
        glp_delete_prob(lp);
        return static_cast<int64_t>(a * 3 + b);
    }
    else {
        std::cout << "No solution found\n";
        glp_delete_prob(lp);
        return 0;
    }
}




uint64_t manual_solve(Game& g) {
    int64_t det = g.ax * g.by - g.bx * g.ay;

    if (det == 0) {
        std::cout << "No solution - determinant is zero\n";
        return 0;
    }

    int64_t a_num = g.xg * g.by - g.bx * g.yg;
    int64_t b_num = g.ax * g.yg - g.xg * g.ay;

    if (a_num % det != 0 || b_num % det != 0) {
        return 0;
    }

    int64_t a = a_num / det;
    int64_t b = b_num / det;

    if (a * g.ax + b * g.bx == g.xg && a * g.ay + b * g.by == g.yg) {
        return a * 3 + b;
    }

    return 0;
}










int main() {
    std::vector<std::string> ls = aoc_utils::read_lines("input.txt");

    std::vector<Game> games;
    for (size_t i = 0; i + 3 <= ls.size(); i += 4) {
        auto m1 = ctre::match<R"(Button A: X\+(\d+), Y\+(\d+))">(ls[i]);
        auto m2 = ctre::match<R"(Button B: X\+(\d+), Y\+(\d+))">(ls[i + 1]);
        auto m3 = ctre::match<R"(Prize: X=(\d+), Y=(\d+))">(ls[i + 2]);

        if (m1 && m2 && m3) {
            int64_t ax = m1.get<1>().to_number<int64_t>();
            int64_t ay = m1.get<2>().to_number<int64_t>();
            int64_t bx = m2.get<1>().to_number<int64_t>();
            int64_t by = m2.get<2>().to_number<int64_t>();
            int64_t xg = m3.get<1>().to_number<int64_t>();
            int64_t yg = m3.get<2>().to_number<int64_t>();

            games.emplace_back(Game{ ax, ay, bx, by, xg, yg });
        }
    }

    int p1 = 0;
    uint64_t p2 = 0;

    for (const auto& g : games) {
        p1 += solve_game(g);
    }


    for (auto& g : games) {
        g.xg += 10000000000000;
        g.yg += 10000000000000;
        p2 += manual_solve(g);

    }


    std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

    //test();
}


//Part 1: 32067
//Part 2 : 92871736253789