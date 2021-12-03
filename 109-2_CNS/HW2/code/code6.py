from pwn import *
import binascii


def table_init():
	out_table = []
	sure_table = []
	for i in range(0, 5):
		tmp = []
		zero = []
		for j in range(0, 5):
			tmp.append(1 if i == j else 0)
			zero.append(0)
		
		sure_table.append(tmp)
		out_table.append(zero)
	return out_table, sure_table

def show_out_table(out_table, sure_table):
	print('====================')
	print('  1 2 3 4 5')	
	for i in range(0, 5):
		print((i + 1), end = ' ')	
		for j in range(0, 5):
			if sure_table[i][j]:
				print(out_table[i][j], end = ' ')
			else:
				print('X', end = ' ')
		print()
	print('====================')

def generate_keys():
	keylist = []
	for i in range(1, 6):
		for j in range(i + 1, 6):
			for k in range(j + 1, 6):
				keylist.append('s{} s{} s{}'.format(i, j, k))
	return keylist

def fill_table(path, out_table, sure_table):
	n = path.replace('>', '').replace('s', '').split()
	for i in range(len(n)):
		n[i] = int(n[i]) - 1
	if (len(n) == 4):
		# format like 's1 s2 s3 >> s4'
		for i in range(0, 3):
			out_table[n[i]][n[3]] = 1
			sure_table[n[i]][n[3]] = 1
	elif (len(n) == 5):
		same = []
		for i in range(3, 5):
			for j in range(0, 3):
				if n[i] == n[j]:
					same.append(n[i])
		# format like 's1 s2 s3 >> s2 s5'
		if (len(same) == 1):
			theother = n[3] if same[0] == n[4] else n[4]
			out_table[same[0]][theother] = 1
			sure_table[same[0]][theother] = 1
		# format like 's1 s2 s3 >> s2 s3'
		elif (len(same) == 2):
			out_table[same[0]][same[1]] = 1
			out_table[same[1]][same[0]] = 1
			sure_table[same[0]][same[1]] = 1
			sure_table[same[1]][same[0]] = 1
	return

def calculate_degree(pn):
	degree = []
	degree.append(int((pn[0] * pn[1] * pn[3] / pn[6] / pn[6]) ** (1 / 3)))
	degree.append((pn[7] * degree[0]) // pn[4])
	degree.append((pn[6] * degree[0]) // pn[1])
	degree.append((pn[6] * degree[0]) // pn[0])
	degree.append((pn[7] * degree[0]) // pn[0])
	return degree


def fill_zero_by_degree(degree, out_table, sure_table):
	# fill zero
	for i in range(0, 5):
		if degree[i] == sum(out_table[i]):
			sure_table[i] = [1, 1, 1, 1, 1]
	return

def indegree_check(out_table, sure_table):
	for i in range(0, 5):
		if sum(row[i] for row in out_table) == 0 and sum(row[i] for row in sure_table) == 4:
			for j in range(0, 5):
				if sure_table[j][i] == 0:
					sure_table[j][i] = 1
					out_table[j][i] = 1
					break	

def find_path(path, cur_at, out_table):
	find = True
	for i in range(0, 5):
		if i not in path:
			find = False
	if find:
		return True, path
	for i in range(4, -1, -1):
		if i in path:
			continue
		if out_table[i][cur_at] == 1:
			trypath = path.copy()
			trypath.append(i)
			find, trypath = find_path(trypath, i, out_table)
			if find:
				return True, trypath
	return find, path


def main():
	r = remote('cns.csie.org', '8507')

	# Initialization	
	out_table, sure_table = table_init()	
	keylist = generate_keys()
	log = {k : [] for k in keylist}

	# Read input and fill table according single result	
	while True:
		path = r.recvline(keepends = False).decode()
		if path[0] != 's':
			break
		fill_table(path, out_table, sure_table)
		log[path[0:8]].append(path)

	# Fill 0 to table according outdegree
	path_num = []
	for i in range(len(keylist)):
		path_num.append(len(log[keylist[i]]))
	degree = calculate_degree(path_num)

	# Fill the table at best effort
	fill_zero_by_degree(degree, out_table, sure_table)
	indegree_check(out_table, sure_table)	
	fill_zero_by_degree(degree, out_table, sure_table)

	show_out_table(out_table, sure_table)
	
	# Find path
	find, path = find_path([4], 4, out_table)
	
	if not find:
		print('Unable to find a path this time')
		return

	txt = r.recvuntil('give me the packet:')
	rsp = ''
	for i in range(len(path) - 1, -1, -1):
		rsp += 's{}||'.format(int(path[i]) + 1)
	rsp += 'print the flag'
	r.sendline(rsp)
	txt = r.recvuntil('flag : \n')
	print(r.recvline().decode())




if __name__ == '__main__':
	main()
