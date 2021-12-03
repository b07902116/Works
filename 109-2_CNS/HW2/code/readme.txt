python version 3.6.9
TLS:

Tor:
python3 code5-2.py
in /tor3
cnshw.c generate the desired keypair, then keys_to_addr.py in generate the onion address
cnshw.c needs fe.o, ge.o, keypair.o, seed.o, sha512.o to compile and execute
keys_to_addr.py has hardcoded keypair value and can execute by python3 keys_to_addr.py

Mix Network:
python3 code6.py

NTLM:
python3 code7.py
code7-d.c brute-force search the password