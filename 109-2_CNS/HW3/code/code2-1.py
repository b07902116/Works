from pwn import *
import binascii


def play(r):
    r.recvuntil("Your choice: ")
    r.sendline("1")
    myint = 5566
    for i in range(10):
        contribution = r.recvline(keepends = False).decode()[len("Contribution: "):]
        myint ^= int(contribution, 16)
    reply = "{:x}".format(myint)
    r.sendline(reply.zfill(512))
        
def getflag(r):
    r.recvuntil("Your choice: ")
    r.sendline("2")
    r.recvuntil("flag: ")
    print(r.recvline(keepends = False).decode())


def main():
    r = remote("cns.csie.org", 7680)
    for i in range(100):
        play(r)
        r.recvuntil("Current money: ")
        print(r.recvline(keepends = False).decode())
    getflag(r)
    r.sendline("3")



if __name__ == "__main__":
    main()
