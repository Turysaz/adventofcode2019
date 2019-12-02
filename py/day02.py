
with open("../inputs/day02.txt") as file:
    mem = [ int(x) for x in file.readline().split(",")]

def intcode(ram, noun, verb):
    ram[1] = noun
    ram[2] = verb
    pc = 0 # program count

    while pc < len(ram):
        opc = ram[pc]
        if opc == 99:
            return ram
            break
        elif opc == 1:
            ram[ram[pc+3]] = ram[ram[pc+1]] + ram[ram[pc+2]]
        elif opc == 2:
            ram[ram[pc+3]] = ram[ram[pc+1]] * ram[ram[pc+2]]
        else:
            print("OPC ERROR: {}".format(opc))
            break
        pc += 4


print("part 1")
print(intcode(mem.copy(), 12, 2)[0])

print("part 2")
for n in range(100):
    for v in range(100):
        if intcode(mem.copy(), v, n)[0] == 19690720:
            print(v,n)

