from hashlib import sha512
from random import randint
from pwn import *



def reduction_f(hashpwd, i, passwords):
    index = (int(hashpwd[:-32], 16) + i) % len(passwords)
    tail = str((int(hashpwd[-32:], 16) + i) % 10000).rjust(4, '0')
    return passwords[index] + tail


def salted(s):
    return (s + "CNS2021").encode()


def hash_f(s):
    return sha512(s).hexdigest()


def binary_search(endval, rainbow, left, right):
    if right - left == 1:
        if endval == rainbow[left][1]:
            return left
        else:
            return -1
    mid = (left + right) // 2
    end_mid = rainbow[mid][1]
    if endval > end_mid:
        return binary_search(endval, rainbow, mid, right)
    elif endval < end_mid:
        return binary_search(endval, rainbow, left, mid)
    else:
        return mid



def main():
    with open("10-million-password-list-top-1000000.txt", 'r') as infile:
        passwords = infile.read().splitlines()

    rainbow = []
    with open("rainbow_len150.txt", 'r') as infile:
        for line in infile:
            rainbow.append(line.rstrip('\n').split(' '))
    print('finish reading rainbow table')


    # sort rainbow table according to endpoint value
    rainbow.sort(key = lambda pair : pair[1])
    print('finish sorting rainbow table')

    # read hashpwd
    r = remote('cns.csie.org', '8505')
    txt = r.recvuntil('[*] Challenge: ')
    hashstrings = r.recvline(keepends = False).decode('ascii', 'ignore')

    hashtable = hashstrings.split(';')
    pwdtable = []
    count = 0
    chainlen = 150

    for n in range(0, len(hashtable)):
        #print(n)
        for i in range(chainlen - 1, -1, -1):
            hit = False
            hashpwd = hashtable[n]
            for j in range(i, chainlen - 1):
                pwd = reduction_f(hashpwd, j, passwords)
                hashpwd = hash_f(salted(pwd))
            pwd = reduction_f(hashpwd, chainlen - 1, passwords)
            index = binary_search(pwd, rainbow, 0, len(rainbow))
            if index != -1:
                pwd_guess = rainbow[index][0]
                for j in range(0, chainlen):
                    hashpwd = hash_f(salted(pwd_guess))
                    if (hashpwd == hashtable[n]):
                        hit = True
                        break
                    else:
                        pwd_guess = reduction_f(hashpwd, j, passwords)
            if hit is True:
                print('%d-th correct pwd : %s' %(count, pwd_guess))
                count += 1
                pwdtable.append(pwd_guess)
                break
        #if hit is False:
        #    print('%d-th not found in table' %n)
    r.sendline(f'{"<CNS>".join(pwdtable)}')

    txt = r.recvuntil('flag: ')
    print(r.recvline(keepends = False).decode())



if __name__ == '__main__':
    main()
