from pwn import *
import binascii
import hashlib
import random


def MD5(contribution):
    m = hashlib.md5()
    m.update(contribution)
    return m.hexdigest()

def generate_randomness(contributions, mycontribution):
    randomness = 0
    for contribution in contributions:
        randomness ^= int.from_bytes(contribution, 'big')
    randomness ^= int.from_bytes(mycontribution, 'big')
    random.seed(randomness)
    return random.random()


def read_collision_file():
    f1 = open("collision1.bin", "rb")
    f2 = open("collision2.bin", "rb")
    c1 = f1.read(128)
    c2 = f2.read(128)
    f1.close()
    f2.close()
    return [c1, c2]


def play(r, contribution_option):
    r.recvuntil("Your choice: ")
    r.sendline("1")
    r.recvuntil("Give me your commitment (MD5(contribution)) in hexadecimal: ")
    r.sendline(MD5(contribution_option[0]))
    
    contributions = []
    for i in range(10):
        contribution = r.recvline(keepends = False).decode()[len("Contribution: "):]
        contributions.append(binascii.unhexlify(contribution))

    if generate_randomness(contributions, contribution_option[0]) > 0.5:
        mycontribution = contribution_option[0]
    else:
        mycontribution = contribution_option[1]
    
    r.sendline(binascii.hexlify(mycontribution))

        
def getflag(r):
    r.recvuntil("Your choice: ")
    r.sendline("2")
    r.recvuntil("flag: ")
    print(r.recvline(keepends = False).decode())


def main():
    contribution_option = read_collision_file()
    r = remote("cns.csie.org", 7681)
    for i in range(200):
        play(r, contribution_option)
        r.recvuntil("Current money: ")
        print(r.recvline(keepends = False).decode())
    getflag(r)
    r.sendline("3")



if __name__ == "__main__":
    main()
