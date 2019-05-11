from commands import *
from numpy import arange

def d_params(size, ratio):
    n = int(size // (4*ratio + 2))
    m = n * ratio

    return (m, n)

def parse_arg(s):
    s = s.strip()
    if s[0] == "[" and s[-1] == "]":
        r = s[1:-1].split(",")
    else:
        r = [s,s]
    
    return map(float, r)

def sat_cfi_gen(name, size, ratio, increments):
    s_start, s_end = parse_arg(size)
    r_start, r_end = parse_arg(ratio)
    s_inc = (s_end - s_start) / (increments-1)
    r_inc = (r_end - r_start) / (increments-1)

    s = s_start
    r = r_start
    for i in range(increments):
        m, n = d_params(s,r) 
        print(m, n)
        generate_construction_d(name + ("%04d" % i), m, n, clean=False)  
        s += s_inc
        r += r_inc

size  = "[100,200]"
ratio = "3"
increments = 5

sat_cfi_gen("test", size, ratio, increments)
