# this test generates a lot of graphs and records their run times on the solver.
# each graph is solved multiple to times to take into account variations in solve times.

# remember that Python has a dreaded global interpretator lock, so we should use the parallel command for
# true parallelism. https://realpython.com/python-gil/

import subprocess
import shlex

def generate_construction_d(folder_name, m, n, clean=True, filters=True):
    command = ["./sat_cfi.sh", str(m), str(n), folder_name]
    
    if clean:
        command.append("--clean")

    if not filters:
        command.append("--no-filters")

    p1 = subprocess.Popen(command, cwd="./bin/", stdout=subprocess.PIPE)
    p1.wait()

    p2 = subprocess.Popen(["mv", folder_name, "../tests/working/" + folder_name], cwd="./bin/")
    p2.wait()

def generate_construction_ns(folder_name, n, clean=True):
    command = ["./reg_cfi.sh", str(n), folder_name]
    
    if clean:
        command.append("--clean")   
    
    p1 = subprocess.Popen(command, cwd="./bin/", stdout=subprocess.PIPE)
    p1.wait()

    p2 = subprocess.Popen(["mv", folder_name, "../tests/working/" + folder_name], cwd="./bin/")
    p2.wait()

def dreadnaut_solve(filename, mode="nauty"):
    print("Solving " + filename)
    p = subprocess.Popen(["./time.sh", "working/" + filename, "--" + mode], cwd="./tests/", stdout=subprocess.PIPE)
    stdout = p.communicate()[0]
    runtime = float(stdout.decode("utf-8").replace("\n", ""))

    return runtime
