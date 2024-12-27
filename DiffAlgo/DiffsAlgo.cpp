#include <bits/stdc++.h>

// A function to convert a string to differnt lines. like a vector of strings

// Shortest edit search
int Shortest_Edit_Search(std::string a, std::string b, std::vector<std::vector<int>> &trace)
{
    int n = a.size(), m = b.size();
    int max = n + m;
    std::vector<int> v((2 * max) + 1, 0);
    int offset = max;
    v[offset + 1] = 0; // it begins with a virtual move downwards from (x, y) = (0,–1).
    int x, y;
    // To store the state of v at each iteration
    for (int d = 0; d < max; d++)
    {
        trace.push_back(v);
        for (int k = -d; k <= d; k += 2)
        {
            int idx = offset + k;
            if (k == -d || (k != d && v[idx - 1] < v[idx + 1]))
            {
                x = v[idx + 1];
            }
            else
            {
                x = v[idx - 1] + 1;
            }
            y = x - k;
            std::cout << "idx = " << idx << ", x = " << x << ", y = " << y << std::endl;
            while (x < n && y < m && a[x] == b[y])
            {
                x++;
                y++;
            }
            v[idx] = x;
            // std::cout<<v[idx];
            if (x >= n && y >= m)
            {
                return d;
            }
        }
    }
    return 0;
}

// To perform this backtracking, we need to make a small adjustment to shortest_edit. Rather than just recording the latest x value for each k, we need to keep a trace of the array before each turn of the outer loop.
// Backtracking the graph
void Backtrack(std::string a, std::string b, std::vector<std::vector<int>> trace)
{
    int x = a.size(), y = b.size();
    int offset = trace[0].size() / 2; // Offset to handle negative indices
    int prev_k = 0;

    for (int d = trace.size() - 1; d >= 0; --d)
    {
        std::vector<int> v = trace[d];
        int k = x - y;

        int prev_k;
        if (k == -d || (k != d && v[offset + k - 1] < v[offset + k + 1]))
        {
            prev_k = offset + k + 1;
        }
        else
        {
            prev_k = offset + k - 1;
        }

        int prev_x = v[prev_k];
        int prev_y = prev_x - (prev_k - offset);

        while (x > prev_x && y > prev_y)
        {
            std::cout << "Move from (" << x - 1 << ", " << y - 1 << ") to (" << x << ", " << y << ")" << std::endl;
            x--;
            y--;
        }

        if (d > 0)
        {
            std::cout << "Move from (" << prev_x << ", " << prev_y << ") to (" << x << ", " << y << ")" << std::endl;
        }

        x = prev_x;
        y = prev_y;
    }
}

struct Edit
{
    enum Type
    {
        EQL,
        DEL,
        INS
    } type;
    std::string old_line;
    std::string new_line;

    Edit(Type t, const std::string &oldLine, const std::string &newLine)
        : type(t), old_line(oldLine), new_line(newLine) {}
};

std::vector<Edit> diff(const std::vector<std::string> &a, const std::vector<std::string> &b, const std::vector<std::vector<int>> &trace)
{
    std::vector<Edit> diff;
    int x = a.size(), y = b.size();
    int offset = trace[0].size() / 2;

    for (int d = trace.size() - 1; d >= 0; --d)
    {
        std::vector<int> v = trace[d];
        int k = x - y;

        int prev_k;
        if (k == -d || (k != d && v[offset + k - 1] < v[offset + k + 1]))
        {
            prev_k = offset + k + 1;
        }
        else
        {
            prev_k = offset + k - 1;
        }

        int prev_x = v[prev_k];
        int prev_y = prev_x - (prev_k - offset);

        while (x > prev_x && y > prev_y)
        {
            diff.insert(diff.begin(), Edit(Edit::EQL, a[x - 1], b[y - 1]));
            x--;
            y--;
        }

        if (d > 0)
        {
            if (x == prev_x)
            {
                diff.insert(diff.begin(), Edit(Edit::INS, "", b[prev_y]));
            }
            else if (y == prev_y)
            {
                diff.insert(diff.begin(), Edit(Edit::DEL, a[prev_x], ""));
            }
        }

        x = prev_x;
        y = prev_y;
    }

    return diff;
}

class Printer
{
public:
    Printer(std::ostream &output = std::cout) : output(output) {}

    void print(const std::vector<Edit> &diff)
    {
        for (const auto &edit : diff)
        {
            print_edit(edit);
        }
    }

private:
    std::ostream &output;

    void print_edit(const Edit &edit)
    {
        std::string col, reset = "\033[39m";
        std::string tag;

        switch (edit.type)
        {
        case Edit::EQL:
            col = "";
            tag = " ";
            break;
        case Edit::DEL:
            col = "\033[31m";
            tag = "-";
            break;
        case Edit::INS:
            col = "\033[32m";
            tag = "+";
            break;
        }

        std::string old_line = edit.old_line.empty() ? "" : edit.old_line;
        std::string new_line = edit.new_line.empty() ? "" : edit.new_line;

        output << col << tag << " " << std::setw(4) << old_line << " " << std::setw(4) << new_line << "    " << (old_line.empty() ? new_line : old_line) << reset << std::endl;
    }
};

int main()
{
    // std::string a="ABCABBA";
    // std::string b="CBABAC";
    // std::vector<std::vector<int>> trace;
    // int ans = Shortest_Edit_Search(a,b,trace); // returns the minimum number of steps required to change A to B
    // std::cout<<ans<<std::endl;

    // Backtrack(a,b,trace);

    std::string a = "ABCABBA";
    std::string b = "CBABAC";
    std::vector<std::vector<int>> trace;
    int ans = Shortest_Edit_Search(a, b, trace); // returns the minimum number of steps required to change A to B
    std::cout << ans << std::endl;

    std::vector<std::string> a_lines = {"A", "B", "C", "A", "B", "B", "A"};
    std::vector<std::string> b_lines = {"C", "B", "A", "B", "A", "C"};

    std::vector<Edit> diff_result = diff(a_lines, b_lines, trace);

    Printer printer;
    printer.print(diff_result);

    return 0;
}