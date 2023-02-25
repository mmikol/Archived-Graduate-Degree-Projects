import sys, os
from time import process_time
from psutil import Process

def main(X, Y):
    start_time = process_time()

    ALPHA = { 
        'A': { 'A': 0, 'C': 110, 'G': 48, 'T': 94 },
        'C': { 'A': 110, 'C': 0, 'G': 118, 'T': 48 },
        'G': { 'A': 48, 'C': 118, 'G': 0, 'T': 110 },
        'T': { 'A': 94, 'C': 48, 'G': 110, 'T': 0 } 
    }

    DELTA = 30

    OPT = alignment(X, Y, ALPHA, DELTA)
    str1, str2 = traceback(OPT, X, Y, ALPHA, DELTA)

    return (
        (str1[:50], str1[len(str1)-50:]), 
        (str2[:50], str2[len(str2)-50:]),
        process_time() - start_time, # process time in seconds
        Process().memory_info().rss / 1024 # memory usage in kilobytes
    )

def alignment(X, Y, ALPHA, DELTA):
    m, n = len(X)+1, len(Y)+1
    
    A = [[0] * n for j in range(m)]

    for i in range(m):
        A[i][0] = i * DELTA

    for j in range(n):
        A[0][j] = j * DELTA

    for j in range(1, n):
        for i in range(1, m):
            A[i][j] = min(A[i-1][j-1] + ALPHA[X[i-1]][Y[j-1]], A[i-1][j] + DELTA, A[i][j-1] + DELTA)

    return A

def traceback(M, X, Y, ALPHA, DELTA):
    str1, m = [], len(X)
    str2, n = [], len(Y)

    while m > 0 or n > 0:
        if m > 0 and n > 0 and M[m][n] == M[m-1][n-1] + ALPHA[X[m-1]][Y[n-1]]:
            str1.append(X[(m:=m-1)])
            str2.append(Y[(n:=n-1)])
        elif m > 0 and  M[m][n] == M[m-1][n] + DELTA:
            str1.append(X[(m:=m-1)])
            str2.append('_')
        else:
            str1.append('_')
            str2.append(Y[(n:=n-1)])

    return ''.join(str1[::-1]), ''.join(str2[::-1])
    
def generate_input_strings(input_file_name):
    with open(input_file_name, 'r') as input:
        parsed = [int(l.strip()) if l.strip().isdigit() else l.strip() for l in input]
       
        i = 1
        while isinstance(parsed[(i:=i+1)], int):
            continue

        str1, str1_indices = parsed[0], parsed[1:i] 
        str2, str2_indices = parsed[i], parsed[i+1:len(parsed)]

        for j in str1_indices:
            str1 = ''.join([str1[:j+1], str1, str1[j+1:]])

        for k in str2_indices:
            str2 = ''.join([str2[:k+1], str2, str2[k+1:]])

        return str1, str2
        
if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise Exception("Specification of single input file name missing.")
    
    X, Y = generate_input_strings(sys.argv[1])

    (str1_, _str1), (str2_, _str2), time_used, mem_used = main(X, Y)

    with open(f'output.txt', 'w') as output:
        output.write(f'{str1_} {_str1}\n')
        output.write(f'{str2_} {_str2}\n')
        output.write(f'{time_used}\n')
        output.write(f'{mem_used}')