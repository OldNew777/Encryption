import sys

skip = [' ', '\n', '\r']
if len(sys.argv) != 2:
    raise ValueError("bad arguments")
MAX_PRIME_NUM = int(sys.argv[1])

prefix = ['', 'Not']

for pref in prefix:
    for i in range(MAX_PRIME_NUM):
        # read Binary
        name = pref + 'Prime_Bin_' + str(i) + '.in'
        n = ''
        with open(name, 'r') as f:
            sum = 8 * 64
            while sum > 0:
                ch = f.read(1)
                if ch in skip:
                    continue
                sum -= 1
                n += ch
        n = int(n, 2)
        print(n)
    
        # write Dec
        name = pref + 'Prime_Dec_' + str(i) + '.in'
        with open(name, 'w') as f:
            f.write(str(n))
