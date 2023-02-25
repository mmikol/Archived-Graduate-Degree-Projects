from copy import deepcopy
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

    str1, str2 = divide_and_conquer_alignment(X, Y, ALPHA, DELTA)

    return (
        (str1[:50], str1[len(str1)-50:]), 
        (str2[:50], str2[len(str2)-50:]),
        process_time() - start_time, # process time in seconds
        Process().memory_info().rss / 1024 # memory usage in kilobytes
    )

def divide_and_conquer_alignment(X, Y, ALPHA, DELTA):
    m, n = len(X), len(Y)

    if m == 0:
        return '_' * n, Y

    elif n == 0:
        return X, '_' * m

    elif m == 1 or n == 1:
        OPT = alignment(X, Y, ALPHA, DELTA)
        return traceback(OPT, X, Y, ALPHA, DELTA)

    f = space_efficient_alignment(X[:m//2], Y, ALPHA, DELTA)
    g = space_efficient_alignment(X[m//2:][::-1], Y[::-1], ALPHA, DELTA)

    opt_mid = (t := [x + y for x, y in zip(f, g[::-1])]).index(min(t))

    str1_, str2_ = divide_and_conquer_alignment(X[:m//2], Y[:opt_mid], ALPHA, DELTA)
    _str1, _str2 = divide_and_conquer_alignment(X[m//2:], Y[opt_mid:], ALPHA, DELTA)

    return ''.join([str1_, _str1]), ''.join([str2_, _str2])
    
def space_efficient_alignment(X, Y, ALPHA, DELTA):
    m, n = len(X)+1, len(Y)+1

    B = [[0] * n for i in range(2)]
    
    for j in range(n):
        B[0][j] = j * DELTA
    
    for i in range(1, m):
        B[1][0] = i * DELTA
        for j in range (1, n):
            B[1][j] = min(B[0][j-1] + ALPHA[X[i-1]][Y[j-1]], B[0][j] + DELTA, B[1][j-1] + DELTA)
        B[0] = deepcopy(B[1]) 

    return B[1]

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