import numpy as np
import matplotlib.pyplot as plt
import cv2


def binarize(img, count):
	(height, width) = img.shape
	for i in range(height):
		for j in range(width):
			count[img[i, j]] = count[img[i, j]] + 1
			if img[i, j] >= 128:
				img[i, j] = 255
			else:
				img[i, j] = 0
	return

def find_set(equ, element):
	for i in range(len(equ)):
		if element in equ[i]:
			return i


def set_merge(equ, a, b):
	p = find_set(equ, a)
	q = find_set(equ, b)
	if p == q:
		return
	equ[p] = equ[p] | equ[q]
	equ[q] = set()
	equ.remove(equ[q])
	return

def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
	count = np.zeros(256, dtype = np.int32)
	binarize(img, count)
	#plt.hist(np.arange(0, 256), bins = 256, facecolor = "black", weights = count)
	#plt.show()
	cv2.imwrite('binarylena.bmp', img)
	#4-connected
	(height, width) = img.shape
	table = np.zeros((height, width), dtype = np.int32)
	equ = [set()]
	index = 1
	
	#label each white pixel
	for i in range(height):
		for j in range(width):
			if img[i, j] != 0:
				if i == 0 and j == 0:
					equ.append(set())
					equ[len(equ) - 1].add(index)
					table[i, j] = index
					index = index + 1
				elif i != 0 and j != 0:
					if img[i - 1, j] == 0 and img[i, j - 1] == 0:
						equ.append(set())
						equ[len(equ) - 1].add(index)
						table[i, j] = index
						index = index + 1
					elif img[i - 1, j] != 0 and img[i, j - 1] != 0:
						table[i, j] = min(table[i - 1, j], table[i, j - 1])
						set_merge(equ, table[i, j - 1], table[i - 1, j])
					elif img[i - 1, j] == 0 and img[i, j - 1] != 0:
						table[i, j] = table[i, j - 1]
					elif img[i - 1, j] != 0 and img[i, j - 1] == 0:
						table[i, j] = table[i - 1, j]
				elif i == 0 and j != 0:
					if img[i, j - 1] != 0:
						table[i, j] = table[i, j - 1]
					else:
						equ.append(set())
						equ[len(equ) - 1].add(index)
						table[i, j] = index
						index = index + 1
				elif i != 0 and j == 0:
					if img[i - 1, j] != 0:
						table[i, j] = table[i - 1, j]
					else:
						equ.append(set())
						equ[len(equ) - 1].add(index)
						table[i, j] = index
						index = index + 1

	#count number of pixels of each connected component
	pixel_count = {}
	for i in range(height):
		for j in range(width):
			if img[i, j] != 0:
				p = find_set(equ, table[i, j])
				table[i, j] = int(min(equ[p]))
				if table[i, j] not in pixel_count:
					pixel_count[table[i, j]] = 1
				else:
					pixel_count[table[i, j]] += 1

	color = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
	for key, value in pixel_count.items():
		if value >= 500:
			xsum, ysum = 0, 0
			left, right, top, bottom = 512, 0, 512, 0
			for i in range(height):
				for j in range(width):
					if table[i, j] == key:
						xsum += j
						ysum += i
						if j < left:
							left = j
						if j > right:
							right = j
						if i < top:
							top = i
						if i > bottom:
							bottom = i
			xsum /= value
			ysum /= value
			cv2.rectangle(color, (left, top), (right, bottom), (255, 0, 0), 2)
			cv2.circle(color, (int(xsum), int(ysum)), 3, (0, 0, 255), 2)
	cv2.imwrite('ans.bmp', color)


if __name__ == '__main__':
	main()