from pwn import *
import ClassGroup


def getorder(group):
    x = group.generator()
    u = group.identity()
    cur = x * x
    order = 2
    power_list = [u, x, cur]
    while cur != u:
        order += 1
        cur = cur * x
        power_list.append(cur)
    return order, power_list

def mypow(x, n):
    bitlen = n.bit_length()
    ret = x
    for i in range(bitlen - 1):
        ret = ret * ret
        if (1 << (bitlen - i - 2)) & n:
            ret = ret * x
    return ret



def main():
    r = remote("cns.csie.org", 7686)
    print("========First  Part========")
    txt = r.recvuntil("d = ")
    d = int(r.recvline(keepends = False).decode())
    #d = -170141183460469231731687303715884105727
    print("d:", d)
    txt = r.recvuntil("T = ")
    T = int(r.recvline(keepends = False).decode())
    print("T:", T)

    group = ClassGroup.ClassGroup(d)
    order, power_list = getorder(group)
    x = group.generator()
    u = group.identity()
    print("Group order:", order)

    p = pow(2, T, order)
    y = mypow(x, p)
    print("y:", str(y))
    r.sendline(str(y))
    txt = r.recvuntil("L = ")
    L = int(r.recvline(keepends = False).decode())
    print("L:", L)

    remaining = power_list[pow(2, T, L) % order]
    print(remaining)
    Lmod = L % order
    for i in range(order):
        proof = power_list[i]
        if y == (mypow(proof, Lmod) * remaining):
            break
    print("proof:", str(proof))
        
    r.sendline(str(proof))
    r.recvuntil("flag: ")
    print(r.recvline(keepends = False).decode())
  


    print("========Second Part========")
    p = pow(2, T, order)
    # Generate a new y
    y = mypow(x, p + 1)
    print("y:", str(y))
    r.sendline(str(y))
    txt = r.recvuntil("L = ")
    L = int(r.recvline(keepends = False).decode())
    print("L:", L)

    remaining = power_list[pow(2, T, L) % order]
    print(remaining)
    Lmod = L % order
    for i in range(order):
        proof = power_list[i]
        if y == (mypow(proof, Lmod) * remaining):
            break
    print("proof:", str(proof))
    r.sendline(str(proof))
    r.recvuntil("flag: ")
    print(r.recvline(keepends = False).decode())





if __name__ == "__main__":
    main()
