import binascii
import math
from pwn import *


def cube_root(a):
	left = 0
	right = a
	while right - left > 1:
		mid = (left + right) // 2
		if mid ** 3 > a:
			right = mid
		elif mid ** 3 < a:
			left = mid
		else:
			return mid
	return -1


# from wiki
def ext_euclid(a, b):
	old_s, s = 1, 0
	old_t, t = 0, 1
	old_r, r = a, b
	if b == 0:
		return 1, 0, a
	else:
		while r != 0:
			q = old_r // r
			old_r, r = r, old_r - q * r
			old_s, s = s, old_s - q * s
			old_t, t = t, old_t - q * t
	return old_s, old_t, old_r



def invmod(val, mod):
	s, t, r = ext_euclid(val, mod)	
	return s





def main():
	r = remote('cns.csie.org', '8503')
	# 1
	r.sendline('1')
	txt = r.recvuntil('encrypted flag: ')
	cipher = r.recvline(keepends = False)
	plain = cube_root(int(cipher.decode()))
	print(binascii.unhexlify(str(format(plain, 'x'))).decode())

	# 2
	c = []
	n = []
	for i in range(0, 3):
		r.sendline('2')
		txt = r.recvuntil('encrypted flag: ')
		c.append(int(r.recvline(keepends = False).decode()))
		txt = r.recvuntil('modulus n: ')
		n.append(int(r.recvline(keepends = False).decode()))

	M = [n[1] * n[2], n[0] * n[2], n[0] * n[1]]
	
	cipher = 0	
	for i in range(0, 3):
		cipher += (c[i] * M[i] * invmod(M[i], n[i]))
	cipher %= (n[0] * n[1] * n[2])

	plain = cube_root(cipher)
	print(binascii.unhexlify(str(format(plain, 'x'))).decode())

	# 3
	# let a ** 3 > 1024 but not exceed too much	
	a = 1 << (1024 // 3 + 5)	
	c = []
	n = []
	f = []
	for i in range(0, 3):
		r.sendline('3')
		r.sendline(str(a))
		txt = r.recvuntil('encrypted number: ')
		c.append(int(r.recvline(keepends = False).decode()))
		txt = r.recvuntil('encrypted flag: ')
		f.append(int(r.recvline(keepends = False).decode()))
		n.append((a ** 3) - c[i])

	for i in range(0, 3):
		for j in range(1, 10000):
			if n[i] % j == 0:
				n[i] //= j
	M = [n[1] * n[2], n[0] * n[2], n[0] * n[1]]
	
	cipher = 0	
	for i in range(0, 3):
		cipher += (f[i] * M[i] * invmod(M[i], n[i]))
	cipher %= (n[0] * n[1] * n[2])

	plain = cube_root(cipher)
	print(binascii.unhexlify(str(format(plain, 'x'))).decode())

	# 4
	r.sendline('4')
	a = [2, 3]
	c = []
	r.sendline(str(a[0]))
	txt = r.recvuntil('encrypted number: ')
	c.append(int(r.recvline(keepends = False).decode()))

	r.sendline(str(a[1]))
	txt = r.recvuntil('decrypted number: ')
	c.append(int(r.recvline(keepends = False).decode()))
	
	
	r.recvuntil('encrypted flag: ')
	cipher = int(r.recvline(keepends = False).decode())
	n = math.gcd(c[0] ** 3 - a[0], c[1] ** 3 - a[1])
	plain = cipher ** 3 % n
	print(binascii.unhexlify(str(format(plain, 'x'))).decode())




if __name__ == '__main__':
	main()




