# converts files from triples to DIMACS format
import sys

lines = map(lambda line: line.replace("\n", ""),
            sys.stdin.readlines())

# strip all linebreaks
for line in lines:
    line = line.replace("\n", "")

# need to swap args in header
header = lines[0].split(" ")
num_of_clauses = int(header[0]) + 1
num_of_vars = int(header[1])
print("p cnf %d %d" % (num_of_vars, num_of_clauses))

# add CNF clause to convert uniquely satisfiable to unsatisfiable
clause = ""
for i in range(num_of_vars):
    clause += "-" + str(i+1) + " "
clause += "0"
print(clause)

for line in lines[1:]:
    print("x" + line + "0")

