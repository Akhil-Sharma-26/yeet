#include"include/diff_utils.hpp"

// Diffs Algo

// A function to convert a string to differnt lines. like a vector of strings
std::vector<std::string> splitIntoLines(const std::string& str) {
    std::vector<std::string> lines;
    std::string temp;
    for (char c : str) {
        if (c == '\n') {
            lines.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        lines.push_back(temp);
    }
    return lines;
}

// computes the shortest edit distance (minimum number of insertions, deletions, and substitutions) required to transform one vector of strings into another. It uses a diagonal-based dynamic programming approach, tracks intermediate states in the trace parameter, and returns the edit distance as an integer
int Shortest_Edit_Search(const std::vector<std::string>& a, const std::vector<std::string>& b, 
                         std::vector<std::vector<int>>& trace) {
    int n = a.size(), m = b.size();
    if (n == 0) return m;  // All insertions if `a` is empty
    if (m == 0) return n;  // All deletions if `b` is empty

    int max_distance = n + m;
    int diagonal_offset = max_distance;  // To shift diagonals into the array
    std::vector<int> diagonals(2 * max_distance + 1, -1);  // Track edit points for each diagonal
    trace.clear();

    diagonals[diagonal_offset + 1] = 0;  // Initialize diagonal k=1

    for (int d = 0; d <= max_distance; ++d) {
        for (int k = -d; k <= d; k += 2) {
            int x;
            if (k == -d || (k != d && diagonals[diagonal_offset + k - 1] < diagonals[diagonal_offset + k + 1])) {
                // Move down
                x = diagonals[diagonal_offset + k + 1];
            } else {
                // Move right
                x = diagonals[diagonal_offset + k - 1] + 1;
            }

            int y = x - k;  // Corresponding y-coordinate
            while (x < n && y < m && a[x] == b[y]) {
                ++x;
                ++y;
            }

            diagonals[diagonal_offset + k] = x;

            // If we've reached the end of both strings
            if (x >= n && y >= m) {
                trace.push_back(diagonals);  // Store the final state
                return d;
            }
        }
        trace.push_back(diagonals);  // Store the state for this edit distance
    }

    return -1;  // This should never happen
}


// computes the differences between two sequences of strings (a and b) using a trace matrix and a maximum edit distance (d). It returns a vector of Edit objects representing the sequence of edits (insertions, deletions, and equalities) required to transform a into b.
std::vector<Edit> diff(const std::vector<std::string>& a, 
                      const std::vector<std::string>& b,
                      const std::vector<std::vector<int>>& trace,
                      int d) {
    std::vector<Edit> result;
    if (trace.empty() || d < 0) return result;
    
    int x = a.size(), y = b.size();
    int offset = x + y;
    
    for (int i = d; i > 0; --i) {
        const std::vector<int>& v = trace[i];
        int k = x - y;
        
        bool down = (k == -i || (k != i && v[offset + k - 1] < v[offset + k + 1]));
        int k_prev = down ? k + 1 : k - 1;

        int x_prev = v[offset + k_prev];
        int y_prev = x_prev - k_prev;

        while (x > x_prev && y > y_prev) {
            if (a[x - 1] == b[y - 1]) {
                result.push_back(Edit(Edit::EQL, a[x - 1], b[y - 1]));
            } else {
                result.push_back(Edit(Edit::DEL, a[x - 1], ""));
                result.push_back(Edit(Edit::INS, "", b[y - 1]));
            }
            --x;
            --y;
        }

        while (x > x_prev) {
            result.push_back(Edit(Edit::DEL, a[x - 1], ""));
            --x;
        }

        while (y > y_prev) {
            result.push_back(Edit(Edit::INS, "", b[y - 1]));
            --y;
        }
    }

    while (x > 0) {
        result.push_back(Edit(Edit::DEL, a[x - 1], ""));
        --x;
    }

    while (y > 0) {
        result.push_back(Edit(Edit::INS, "", b[y - 1]));
        --y;
    }

    std::reverse(result.begin(), result.end());

    // for (const auto& edit : result) {
    //     std::cout << "Edit Type: " << (edit.type == Edit::EQL ? "EQL" : 
    //                                 (edit.type == Edit::INS ? "INS" : "DEL"))
    //             << ", Orig: \t" << edit.old_line
    //             << ", Updated: \t" << edit.new_line
    //             << std::endl;
    // }

    return result;
}


// Storing diffs in file
void storeDiff(const std::vector<Edit>& edits) {
    std::ofstream diff_file(fs::path(".yeet/Diff").string(), std::ios::trunc); 
    if (diff_file.is_open()) {
        for (const auto& edit : edits) {
            std::string tag = (edit.type == Edit::INS) ? "+" : "-";
            std::string old_line = edit.old_line.empty() ? "" : edit.old_line;
            std::string new_line = edit.new_line.empty() ? "" : edit.new_line;
            diff_file << tag << " " << std::setw(4) << old_line << " " << std::setw(4) << new_line << "    " << (old_line.empty() ? new_line : old_line) << std::endl;
        }
        diff_file.close();
    } else {
        std::cerr << "ERROR::DIFFS:: Unable to open file for writing diffs" << std::endl;
    }
}
