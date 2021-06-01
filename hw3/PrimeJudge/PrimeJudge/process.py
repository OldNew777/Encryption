skip = [' ', '\n', '\r']

for i in range(10):
    # read Binary
    name = str(i) + '.in'
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
    name = str(i) + '_dec.in'
    with open(name, 'w') as f:
        f.write(str(n))
