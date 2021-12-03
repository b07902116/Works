from pwn import *
from Crypto.Cipher import AES


def main():
	Alice = remote('cns.csie.org', '30001')
	Bob = remote('cns.csie.org', '30002')

	myprivate = 7414
	# recv p, g, publicA from Alice
	Alice.recvuntil('p: ')
	p = int(Alice.recvline(keepends = False).decode())
	Alice.recvuntil('g: ')
	g = int(Alice.recvline(keepends = False).decode())
	Alice.recvuntil('public: ')
	publicA = int(Alice.recvline(keepends = False).decode())

	mypublic = (g ** myprivate) % p
	
	# send p, g, mypublic to Bob
	Bob.sendline('p: %d' %p)
	Bob.sendline('g: %d' %g)
	Bob.sendline('public: %d' %mypublic)
	
	# recv publicB, iv from Bob
	Bob.recvuntil('public: ')	
	publicB = int(Bob.recvline(keepends = False).decode())
	Bob.recvuntil('iv: ')
	iv = Bob.recvline(keepends = False).decode()


	# send mypublic, iv to Alice
	Alice.sendline('public: %d' %mypublic)
	Alice.sendline('iv: %s' %iv)

	# generate sessionKey
	mask = (1 << 128) - 1
	sessionKA = (((publicA ** myprivate) % p) & mask).to_bytes(16, "big")
	sessionKB = (((publicB ** myprivate) % p) & mask).to_bytes(16, "big")

	# AES
	encryptforA = AES.new(key = sessionKA, mode = AES.MODE_CBC, iv = bytes.fromhex(iv))
	encryptforB = AES.new(key = sessionKB, mode = AES.MODE_CBC, iv = bytes.fromhex(iv))	
	decryptforA = AES.new(key = sessionKA, mode = AES.MODE_CBC, iv = bytes.fromhex(iv))
	decryptforB = AES.new(key = sessionKB, mode = AES.MODE_CBC, iv = bytes.fromhex(iv))


	# recv flagA1 from Alice
	Alice.recvuntil('flag1: ')
	cipherA1 = bytes.fromhex(Alice.recvline(keepends = False).decode())
	plainA1 = decryptforA.decrypt(cipherA1).decode()

	# send myflagA1 to Bob
	mycipherA1 = encryptforB.encrypt(plainA1.encode())
	Bob.sendline('flag1: %s' %mycipherA1.hex())	

	# recv flagB1 from Bob
	Bob.recvuntil('flag1: ')
	cipherB1 = bytes.fromhex(Bob.recvline(keepends = False).decode())
	plainB1 = decryptforB.decrypt(cipherB1).decode()

	# send myflagB1 to Alice
	mycipherB1 = encryptforA.encrypt(plainB1.encode())
	Alice.sendline('flag1: %s' %mycipherB1.hex())

	# recv flagA2 from Alice
	Alice.recvuntil('flag2: ')
	cipherA2 = bytes.fromhex(Alice.recvline(keepends = False).decode())
	plainA2 = decryptforA.decrypt(cipherA2).decode()

	# Get the flag
	print(plainA1, plainB1, plainA2, sep = '')

if __name__ == '__main__':
	main()
