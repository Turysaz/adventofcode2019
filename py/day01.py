import math
with open("../inputs/day01.txt") as file:
    modules = [int(s) for s in file.readlines()]


def part1():
    return int(math.fsum([math.floor(m / 3) - 2 for m in modules]))

def fuel_recursive(m):
    fuel = math.floor(m / 3) - 2
    return 0 if fuel < 0 else fuel + fuel_recursive(fuel)
    
def part2():
    return int(math.fsum([fuel_recursive(m) for m in modules]))
    
print(part1())
print(part2())


