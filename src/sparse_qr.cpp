#include <iostream>
#include <fstream>
#include <vector>


#include <Eigen/SparseQR>
#include <Eigen/SparseLU>

typedef Eigen::Triplet<int> Entry;
typedef Eigen::SparseMatrix<int, Eigen::ColMajor> SparseMatrix;


SparseMatrix import_triples(std::string filename) {
    std::ifstream fin(filename);
    int m,n;
    fin >> m >> n;
    std::vector<Entry> entries;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            int var_index;
            fin >> var_index;

            // variable indices are 1-indexed
            Entry e(i, var_index-1, 1);
            printf("%d %d %d\n", e.col(), e.row(), e.value());
            entries.push_back(e);
        }
    }

    SparseMatrix mat(m, n);
    mat.setFromTriplets(entries.begin(), entries.end());

    std::cout << mat << std::endl;

    return mat;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw "No filename supplied";
    }

    std::string input_file = argv[1];
    auto matrix = import_triples(input_file);
    matrix.makeCompressed();

    Eigen::SparseQR<SparseMatrix, Eigen::NaturalOrdering<int>> solver;
    solver.analyzePattern(matrix);
    solver.factorize(matrix);

    std::cout << matrix.isCompressed() << std::endl;

    auto rank = solver.rank();
    std::cout << "Rank is " << rank << std::endl;

    return 0;
}
