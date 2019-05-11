#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

typedef vector<vector<bool>> GFMatrix;

bool checkRank(GFMatrix mat) {
    int m = mat.size();
    if (m == 0) {
        throw "Matrix has no rows";
    } 

    int n = mat[0].size();
    for (auto row : mat) {
        if ((int) row.size() != n) {
            throw "Rows are not of equal length";
        }
    }

    if (n > m) {
        throw "Matrix has more columns than rows";
    }

    for (int i = 0; i < n; i++) {
        int s = i;
        while (s < m && !mat[s][i]) {
            s++;
        }

        if (s == m) {
            return false;
        }
        swap(mat[i], mat[s]);

        for (int j = i+1; j < m; j++) {
            if (mat[j][i]) {
                for (int k = i; k < n; k++) {
                    mat[j][k] = mat[j][k] ^ mat[i][k];
                }
            }
        }
    }

    return true;
} 

GFMatrix import_triples(std::string filename) {
    std::ifstream fin(filename);
    int m,n;
    fin >> m >> n;
    
    cout << m << " " << n << endl;

    GFMatrix v(m, vector<bool>(n, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            int var_index;
            fin >> var_index;

            // variable indices are 1-indexed
            v[i][var_index-1] = 1;
        }
    }

    return v;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "No filename supplied";
        return 1;
    }

    string input_file = argv[1];
    GFMatrix matrix = import_triples(input_file);

    int rank_is_n = checkRank(matrix);

    if (rank_is_n) {
        cout << "Great the rank is n.\n";
        return 0;
    } else {
        cout << "Rank is < n.\n";
        return 2;
    }
}
