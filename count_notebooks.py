# script used to count number of lines in notebooks
from json import load
from sys import argv

def loc(f):
    cells = load(open(nb))['cells']
    return sum(len(c['source']) for c in cells if c['cell_type'] == 'code')

def count(files):
    return sum(loc(f) for f in files)

if __name__ == '__main__':
    print(count(argv[1:]))
