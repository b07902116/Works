from pwn import *

def caesar(r):
	txt = r.recvuntil(b'c1 = ')
	cipher = r.recvline()
	cipher = cipher.decode('ascii', 'ignore')	
	txt = r.recvuntil(b'[>]: ')
	lower = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
			 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
			 'y', 'z']
	upper = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
			 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			 'Y', 'Z']
	decode = ''
	for i in range(0, 26):
		decode = ''
		for j in range(len(cipher)):	
			if cipher[j] in lower:
				decode += lower[(lower.index(cipher[j]) + i) % 26]
			elif cipher[j] in upper:
				decode += upper[(upper.index(cipher[j]) + i) % 26]
			else:
				decode += cipher[j]
		print('key = %d, decode =' %i, decode)
	key = int(input('key = ?\n'))
	decode = ''
	for j in range(len(cipher)):
		if cipher[j] in lower:
			decode += lower[(lower.index(cipher[j]) + key) % 26]
		elif cipher[j] in upper:
			decode += upper[(upper.index(cipher[j]) + key) % 26]
		else:
			decode += cipher[j]
		
	r.send(decode)



def one_time_pad(r):
	txt = r.recvuntil(b'c1 = ')
	cipher = r.recvline()
	cipher = cipher.decode('ascii', 'ignore')
	cipher = cipher[:-1]
	cipher = bytearray.fromhex(cipher).decode()	
	txt = r.recvuntil(b'Eve : ')
	xor = r.recvline()
	xor = xor.decode('ascii', 'ignore')	
	rsp = ''
	for i in range(len(cipher)):
		rsp += chr((ord(cipher[i]) ^ ord(xor[i])))
	txt = r.recvuntil(b'[>]: ')
	r.sendline(rsp)

def bacon(r):
	txt = r.recvuntil(b'c2 = ')
	cipher_2 = r.recvline().decode('ascii', 'ignore')
	txt = r.recvuntil(b'[>]: ')
	binary_2 = ''
	for i in range(len(cipher_2)):
		if cipher_2[i].islower():
			binary_2 += '0'
		elif cipher_2[i].isupper():
			binary_2 += '1'
	binary_2 = binary_2[: len(binary_2) - (len(binary_2) % 5)]
	plain_2 = ''
	for i in range(len(binary_2) // 5):
		add = 0
		for j in range(0, 5):
			if binary_2[i * 5 + j] == '1':
				add += (2 ** (4 - j)) 
		plain_2 += chr(ord('A') + add)
	while plain_2[-1] == 'A':
		plain_2 = plain_2[:-1]
	r.sendline(plain_2)


def Zig_Zag(r):
	txt = r.recvuntil(b'c1 = ')
	cipher_1 = r.recvline(keepends = False).decode('ascii', 'ignore')
	txt = r.recvuntil(b'm1 = ')
	plain_1 = r.recvline(keepends = False).decode('ascii', 'ignore')
	txt = r.recvuntil(b'c2 = ')
	cipher_2 = r.recvline(keepends = False).decode('ascii', 'ignore')
	txt = r.recvuntil(b'[>]: ')

	finish = False
	h = 1

	while not finish:
		finish = True
		h += 1
		n = h - 1
		m = len(cipher_1) % n
		gap = [2 * n, 0]
		pos = 0
		line = 0
		count = 0
		for i in range(len(cipher_1)):
			if line == 0 or line == n:
				if cipher_1[i] != plain_1[pos]:
					finish = False
					break
				else:
					pos += 2 * n

			else:
				if cipher_1[i] != plain_1[pos]:
					finish = False
					break
				else:
					pos += gap[count % 2]
					count += 1


			if pos >= len(cipher_1):
				line += 1
				pos = line
				count = 0
				gap = [2 * (n - line), 2 * line]

	plain_2 = [None] * len(cipher_2)
	n = h - 1
	pos = 0
	line = 0
	count = 0
	
	for i in range(len(cipher_2)):
		if line == 0 or line == n:
			plain_2[pos] = cipher_2[i]
			pos += 2 * n
		else:
			plain_2[pos] = cipher_2[i]
			pos += gap[count % 2]
			count += 1
		if pos >= len(cipher_2):
			line += 1
			pos = line
			count = 0
			gap = [2 * (n - line), 2 * line]
	rsp = ''.join(plain_2)
	r.sendline(rsp)




def main():
	# warmup1
	r = remote('cns.csie.org', '17277')
	txt = r.recvuntil(b'[>]: ')
	r.sendline('2')

	# warmup2
	txt = r.recvuntil(b'text = ')
	echo = r.recvline()
	txt = r.recvuntil(b'[>]: ')
	r.send(echo)

	# caesar cipher
	print('caesar')	
	caesar(r)

	# one-time pad
	print('one-time pad')
	one_time_pad(r)

	# Bacon cipher
	print('Bacon')
	bacon(r)

	# Zig Zag
	print('Zig Zag')
	Zig_Zag(r)

	txt = r.recvuntil('flag:\n')
	print(r.recvline(keepends = False).decode())

if __name__ == "__main__":
    main()
