from pwn import *
import binascii



def getIV(r):
	c1 = '243c0d89d4e8eebae43c1aa219115b43'
	guess_rev = ''	
	for i in range(0, 16):
		tail = ''
		for k in range(0, i):
			tail += chr(ord(guess_rev[i - 1 - k]) ^ (i + 1))
		append = ''.join('{:02x}'.format(ord(c)) for c in tail)
		for j in range(0, 256):
			s = '{:02x}'.format(j ^ (i + 1))
			msg = '00' * (15 - i) + s + append + c1
			print('msg = ', msg)
			txt = r.recvuntil('your choice : ')
			r.sendline('2')
			r.sendline(msg)
			txt = r.recvuntil('message(hex encoded) : ')
			#print(txt)
			txt = r.recvline().decode()
			if 'PADDING' not in txt:
				print(txt)
				guess_rev += chr(j)
				print('guess chr is :', chr(j))
				break	
	print('guess_rev', guess_rev)
	Dc1 = guess_rev[::-1]
	print('Dc1 : ', ''.join('{:02x}'.format(ord(c)) for c in Dc1))
	
	m1 = 'id=i_am_the_??||'
	IV = ''
	for i in range(0, 16):
		IV += chr(ord(Dc1[i]) ^ ord(m1[i]))
	print('IV : ', ''.join('{:02x}'.format(ord(c)) for c in IV))



def hexstringtochar(hexstr):
	ret = ''
	for i in range(0, len(hexstr) // 2):
		ret += chr(int(hexstr[i * 2 : (i + 1) * 2], 16))
	return ret

def getEncrypt(r):
	wantedmsg = ['id=i_am_the_ta||', 'act=printtheflag', '||your_message_i', 's:']
	wantedmsg[3] += chr(14) * 14
	hexxor = '856cbae6c179f53bb3046b93b3993689'
	hexIV = '796f75725f65766572796461795f6976'
	
	xor_block = hexstringtochar(hexxor)
	prevcipher = [hexstringtochar(hexIV)]
	wantedcipher = ''
	for i in range(0, 4):
		msg = ''
		for j in range(0, 16):
			msg += chr(ord(xor_block[j]) ^ ord(wantedmsg[i][j]) ^ ord(prevcipher[i][j]))
		r.sendline('1')
		r.sendline(''.join('{:02x}'.format(ord(c)) for c in msg))
		txt = r.recvuntil('message(hex encoded) : ')
		txt = r.recvline(keepends = False)
		#print('wanted block : ', txt[32 * 3 : 32 * 4].decode())
		wantedcipher += txt[32 * 3 : 32 * 4].decode()	
		prevcipher.append(hexstringtochar(txt[32 * 3 : 32 * 4]))	
	#print(wantedcipher)
	r.sendline('3')
	r.sendline(wantedcipher)
	r.sendline('4')
	txt = r.recvuntil('flag : \n')
	print(r.recvline().decode())
	r.sendline('5')


def main():

	r = remote('cns.csie.org', '8506')
	#IV = getIV(r)

	IV = '796f75725f65766572796461795f6976'
	getEncrypt(r)









if __name__ == '__main__':
	main()
