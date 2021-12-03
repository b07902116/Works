from hashlib import sha512
from hashlib import sha3_256
from base64 import b32encode


def pubkey_to_addr(pubkey):
	version = b'\x03'	
	checksum = sha3_256('.onion checksum'.encode() + pubkey + version).digest()[:2]
	address = b32encode(pubkey + checksum + version).decode().lower() + '.onion'
	print(address)
	return address	


def main():

	hssk = bytes.fromhex('68a64c6d2469f9d3ef78b58ff90f4b9acdb4c461d64286e9c89c57b071d19373def17e57981d6bb5a9ea4f2e919eff79a9163641b43ad71de6e81b86fa4c5635')
	hspk = bytes.fromhex('13647b7fef5cb213c6bd063f3270a3e4cb9cafad92f91c84116552d196671e9b')

	f = open('hs_ed25519_public_key', 'wb')
	pubkeyheader = b'== ed25519v1-public: type0 ==\x00\x00\x00'	
	f.write(pubkeyheader + hspk)	
	f.close()

	f = open('hs_ed25519_secret_key', 'wb')
	prikeyheader = b'== ed25519v1-secret: type0 ==\x00\x00\x00'	
	f.write(prikeyheader + hssk)	
	f.close()


	address = pubkey_to_addr(hspk)

	f = open('hostname', 'w')
	f.write(address)
	f.close()


if __name__ == '__main__':
	main()

