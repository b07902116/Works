from hashlib import sha512
from random import randint



def reduction_f(hashpwd, i, passwords):
    index = (int(hashpwd[:-32], 16) + i) % len(passwords)
    tail = str((int(hashpwd[-32:], 16) + i) % 10000).rjust(4, '0')
    return passwords[index] + tail



def mutate(s):
    tail = str(randint(0, 9999)).rjust(4, '0')
    return s + tail


def salted(s):
    return (s + "CNS2021").encode()


def hash_f(s):
    return sha512(s).hexdigest()



def main():
    with open("10-million-password-list-top-1000000.txt", 'r') as infile:
        passwords = infile.read().splitlines()

    times = 10
    chainlen = 150
    for i in range(0, len(passwords)):
        for t in range(0, times):
            pwd = mutate(passwords[i])
            print(pwd, end = ' ')
            for j in range(0, chainlen):
                hashpwd = hash_f(salted(pwd))
                pwd = reduction_f(hashpwd, j, passwords)
            print(pwd)



if __name__ == '__main__':
    main()