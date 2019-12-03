
with open("../inputs/day03.txt") as file:
    routes = [ route.strip().split(",") for route in file.readlines()]

def trace_route(route):
    cx, cy = 0,0 # current
    trace = set()
    for inst in route:
        dist = int(inst[1:])

        if inst[0] == "U":
            x,y = 0,1
        elif inst[0] == "D":
            x,y = 0,-1
        elif inst[0] == "L":
            x,y = -1,0
        elif inst[0] == "R":
            x,y = 1,0

        for i in range(dist):
            cx += x
            cy += y
            trace.add((cx, cy))
        
    return trace

trace0 = trace_route(routes[0])
trace1 = trace_route(routes[1])
distances = []

for position in trace0:
    if position in trace1:
        distances.append(abs(position[0]) + abs(position[1]))

print(min(distances))
