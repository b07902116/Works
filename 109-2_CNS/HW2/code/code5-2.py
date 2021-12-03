from pwn import *
from hashlib import sha3_256
from base64 import b32encode
import socks
import socket

def pubkey_to_addr():
	f = open('pubkey', 'rb')
	pubkey = f.read()
	pubkeyheader = b'== ed25519v1-public: type0 ==\x00\x00\x00'	
	pubkey = pubkey[len(pubkeyheader):]
	version = b'\x03'	
	checksum = sha3_256('.onion checksum'.encode() + pubkey + version).digest()[:2]
	address = b32encode(pubkey + checksum + version).decode().lower() + '.onion'
	print(address)
	return address	


def main():
	address = pubkey_to_addr()	
	socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, '127.0.0.1', 9050, True)
	s = socks.socksocket()
	s.connect((address, 3001))
	out = s.recv(1024).decode()
	print(out)
	x = int(out.split('^')[1][0:2], 10)
	
	left, right = 0, 2 ** x
	while True:
		mid = (left + right) // 2
		s.sendall((str(mid) + '\n').encode())
		out = s.recv(1024).decode()
		print(out, end = '')
		if 'big' in out:
			right = mid
		elif 'small' in out:
			left = mid
		else:
			while True:
				out = s.recv(1024).decode()
				print(out, end = '')
				if (len(out) < 1024 // 8):
					break
			break
	return



if __name__ == '__main__':
	main()
