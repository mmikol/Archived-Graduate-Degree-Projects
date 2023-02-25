from queue import LifoQueue, Queue, PriorityQueue
from math import sqrt, ceil

ACTIONS = {
        1: (1, 0, 0), 2: (-1, 0, 0), 3: (0, 1, 0),
        4: (0, -1, 0), 5: (0, 0, 1), 6: (0, 0, -1),
        7: (1, 1, 0), 8: (1, -1, 0), 9: (-1, 1, 0),
        10: (-1, -1, 0), 11: (1, 0, 1), 12: (1, 0, -1),
        13: (-1, 0, 1), 14: (-1, 0, -1), 15: (0, 1, 1),
        16: (0, 1, -1), 17: (0, -1, 1), 18: (0, -1, -1)
}

def in_bounds(maze, coordinate):
    return (coordinate > (0, 0, 0) and coordinate < maze['dimension']) or coordinate == maze['goal']

def breadth_first_search(maze):
    frontier, solution, discovered = Queue(), LifoQueue(), dict()
    frontier.put((None, maze['start']))
    while not frontier.empty():
        previous, current = frontier.get()
        discovered[current] = previous     
        if current == maze['goal']:
            total_cost = 0
            total_steps = 0 
            while current:
                cost = int(current != maze['start'])
                total_cost += cost
                total_steps += 1
                solution.put((*current, cost))
                current = discovered[current]
            return total_cost, total_steps, solution
        for action_code in maze['tiles'][current]:
            next = tuple(a + c for a, c in zip(ACTIONS[action_code], current))
            if next not in discovered and in_bounds(maze, next):
                frontier.put((current, next))
    return 'FAIL'

def uniform_cost(maze):
    def action_to_cost(action_code):
        if action_code == 0: return 0
        if 1 <= action_code <= 6: return 10        
        return 14
    frontier, solution, discovered = PriorityQueue(), LifoQueue(), dict()
    frontier.put((0, None, 0, maze['start']))
    while not frontier.empty():
        cost, previous, action_code, current = frontier.get()
        discovered[current] = cost, action_code, previous
        if current in discovered and discovered[current][0] < cost:
            continue
        if current == maze['goal']:
            total_cost = cost
            total_steps = 0
            while True:
                solution.put((*current, action_to_cost(action_code)))
                total_steps += 1
                current = previous
                if not previous: 
                    break
                _, action_code, previous = discovered[current]
            return total_cost, total_steps, solution
        for action_code in maze['tiles'][current]:
            next = tuple(a + c for a, c in zip(ACTIONS[action_code], current))     
            next_cost = cost + action_to_cost(action_code)
            if (next not in discovered and in_bounds(maze, next)) or (discovered[next][0] > next_cost):
                frontier.put((next_cost, current, action_code, next))    
    return 'FAIL'

def astar(maze):
    def action_to_cost(action_code):
        if action_code == 0: return 0
        if 1 <= action_code <= 6: return 10        
        return 14
    def distance_heuristic(start, end):
        return min(ceil(sqrt(sum(abs(e - a)**2 for e, a in zip(start, end)))), sum(abs(e - a) for e, a in zip(start, end)))
    frontier, solution, discovered = PriorityQueue(), LifoQueue(), dict()
    frontier.put((0, None, 0, maze['start'], 0))
    while not frontier.empty():
        cost, previous, action_code, current, heuristic_value = frontier.get()
        cost -= heuristic_value
        if current in discovered and discovered[current][0] < cost:
            continue
        discovered[current] = cost, action_code, previous
        if current == maze['goal']:
            total_cost = cost
            total_steps = 0
            while True:
                solution.put((*current, action_to_cost(action_code)))
                total_steps += 1
                current = previous
                if not current: 
                    break
                _, action_code, previous = discovered[current]
            return total_cost, total_steps, solution
        for action_code in maze['tiles'][current]:
            next = tuple(a + c for a, c in zip(ACTIONS[action_code], current))
            heuristic_value = distance_heuristic(next, maze['goal'])
            next_cost = cost + action_to_cost(action_code) + heuristic_value
            if (next not in discovered and in_bounds(maze, next)) or (discovered[next][0] > next_cost - heuristic_value):
                frontier.put((next_cost, current, action_code, next, heuristic_value))
    return 'FAIL'

def read_input(file_name):
    algorithm, dimension, start, goal, good_tile_count, tiles = str(), tuple(), tuple(), tuple(), int(), dict()
    for line_number, line in enumerate(open(file_name, 'r')):
        if line_number == 0:
            algorithm = line.strip()
        elif line_number == 1:
            dimension = tuple(int(value) for value in line.split())
        elif line_number == 2:
            start = tuple(int(value) for value in line.split())
        elif line_number == 3:
            goal = tuple(int(value) for value in line.split())
        elif line_number == 4:
            good_tile_count = int(line.split().pop())
        else: 
            tokenized_line = line.split()
            coordinate = tuple(int(value) for value in tokenized_line[:3])
            actions = tuple(int(value) for value in tokenized_line[3:])
            tiles[coordinate] = actions
    return algorithm, {
        'start': start, 
        'goal': goal, 
        'dimension': dimension,
        'good_tile_count': good_tile_count,
        'tiles': tiles
    }

def write_output(result):
    file = open('output.txt', 'w')
    if not isinstance(result, tuple):
        file.write(f'{result}')
        return
    total_cost, total_steps, solution = result
    file.write(f'{total_cost}\n')
    file.write(f'{total_steps}')
    while not solution.empty():
        file.write('\n')     
        step = solution.get() 
        line = ' '.join(map(str, step))
        file.write(f'{line}')     

if __name__ == '__main__':
    algorithm, maze = read_input('input.txt')
    result = {
        'BFS': breadth_first_search,
        'UCS': uniform_cost,
        'A*': astar,
    }[algorithm](maze)
    write_output(result)


