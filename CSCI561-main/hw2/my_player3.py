from cmath import inf
from copy import deepcopy
from itertools import product
from random import choice

OPPOSITE, OPTIMAL, DEPTH = {}, [], 5

def violates_liberty_rule(state, point, player):
    stack, seen = [point], {point}
    while stack:
        for current, action in product([stack.pop()], [(-1, 0), (1, 0), (0, -1), (0, 1)]):
            i, j = map(sum, zip(current, action))
            if i in range(5) and j in range(5):
                if state[i][j] == 0:
                    return False
                if state[i][j] == player and (i, j) not in seen:
                    stack.append((i, j))
                    seen.add((i, j))
    return True

def violates_ko_rule(previous, next):
    return previous == next

def capture_stones(state, player):
    captured = []
    for i, j in product(range(5), repeat=2):
        if state[i][j] == OPPOSITE[player] and violates_liberty_rule(state, (i, j), OPPOSITE[player]):
            captured.append((i, j))
    for i, j in captured:
        state[i][j] = 0
    return state

def heuristic_evaluation(state, player):
    player_stones = sum([row.count(player) for row in state]) + (2.5 if player == 2 else 0)
    enemy_stones = sum([row.count(OPPOSITE[player]) for row in state]) + (2.5 if OPPOSITE[player] == 2 else 0)
    return 1 if player_stones > enemy_stones else -1 if player_stones < enemy_stones else 0

def possible_states(previous, current, player):
    for i, j in product(range(5), repeat=2):
        if current[i][j] == 0:
            result = deepcopy(current)
            result[i][j] = player
            result = capture_stones(deepcopy(result), player)
            if not (violates_ko_rule(previous, result) or violates_liberty_rule(result, (i, j), player)):
                yield (i, j), result

def minimize(previous, current, depth, alpha, beta, player):
    if not depth:
        return heuristic_evaluation(current, OPPOSITE[player])
    for _, state in possible_states(previous, current, player):
        beta = min(beta, maximize(deepcopy(current), deepcopy(state), depth-1, alpha, beta, OPPOSITE[player]))
        if beta <= alpha:
            return alpha
    return beta
    
def maximize(previous, current, depth, alpha, beta, player):
    global OPTIMAL
    if not depth:
        return heuristic_evaluation(current, player)
    for action, state in possible_states(previous, current, player):
        score = minimize(deepcopy(current), deepcopy(state), depth-1, alpha, beta, OPPOSITE[player])
        if score > alpha and depth == DEPTH:
            alpha = score
            OPTIMAL = [action]
        elif score == alpha and depth == DEPTH:
            OPTIMAL.append(action)
        if alpha >= beta:
            return beta
    return alpha

def write_output(file, action):
    with open(file, 'w') as file:
        file.write(f'{action[0]},{action[1]}') if action else file.write(f'PASS')

def read_input(file):
    with open(file, 'r') as file:
        player, previous, current = None, [], []
        for i, line in enumerate(file):
            if not i:
                player = int(line.strip())
            elif i in range(1, 6):
                previous.append([int(c) for c in line.strip()])
            else: 
                current.append([int(c) for c in line.strip()])
        return player, 1 if player == 2 else 2, previous, current

if __name__ == '__main__':
    player, enemy, previous, current = read_input('input.txt')
    OPPOSITE[player], OPPOSITE[enemy] = enemy, player
    _ = maximize(previous, current, DEPTH, -inf, inf, player)
    write_output('output.txt', choice([(0, 0), (0, 4), (4, 0), (4, 4)]) if all(row == [0] * len(row) for row in current) else choice(OPTIMAL))
    