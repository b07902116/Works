from pwn import *
import ClassGroup
import time


def getpower(u, n, power_list):
    bitlen = n.bit_length()
    ret = u
    for i in range(bitlen):
        if (1 << i) & n:
            ret = ret * power_list[i]
    return ret

    


def main():
    s = time.time()
    r = remote("cns.csie.org", 7685)
    txt = r.recvuntil("d = ")
    d = int(r.recvline(keepends = False).decode())
    print("d:", d)
    txt = r.recvuntil("T = ")
    T = int(r.recvline(keepends = False).decode())
    print("T:", T)

    group = ClassGroup.ClassGroup(d)
    x = group.generator()
    u = group.identity()
    power_list = [x]
    p = 1
    y = x
    for i in range(1, T + 1):
        p <<= 1
        y = y * y
        power_list.append(y)
    print("y: " + str(y))
    r.sendline(str(y))
    txt = r.recvuntil("L = ")
    L = int(r.recvline(keepends = False).decode())
    print("L:", L)
    proof = getpower(u, p // L, power_list)
    print("proof: " + str(proof))
    e = time.time()
    print("spending time:", e - s)
    r.sendline(str(proof))
    txt = r.recvuntil("flag: ", timeout = 1)
    print(r.recvline(keepends = False).decode())





if __name__ == "__main__":
    main()
