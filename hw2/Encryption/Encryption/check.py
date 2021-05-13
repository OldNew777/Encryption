import hashlib

def sha3_256(fileName):
    m = hashlib.sha3_256()
    with open(fileName,'rb') as file:
        while True:
            data = file.read(4096)
            if not data:
                break
            m.update(data)
    return m.hexdigest()

if __name__ == "__main__":
    print(sha3_256("SHA_3_256.in"))
