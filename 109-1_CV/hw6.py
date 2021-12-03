import numpy as np
import cv2

BLACK = 0
WHITE = 255
SIZE = 64


def downsample(bin_img, ds_img):
	height, width = bin_img.shape
	for i in range(0, height, 8):
		for j in range(0, width, 8):
			y = int(i / 8)
			x = int(j / 8)
			ds_img[y, x] = bin_img[i, j]

def hfunc(b, c, d, e):
	q, r = 0, 0
	if b == c:
		if b == d and b == e:
			r += 1
		else:
			q += 1
	return q, r



def Yokoi(ds_img, output):
	corner_offset = [[[0, 1], [-1, 1], [-1, 0]], [[-1, 0], [-1, -1], [0, -1]], [[0, -1], [1, -1], [1, 0]], [[1, 0], [1, 1], [0, 1]]]
	for i in range(SIZE):
		for j in range(SIZE):
			y0, x0 = i, j
			z0 = WHITE
			Q, R = 0, 0
			for k in range(4):
				y1, x1 = i + corner_offset[k][0][0], j + corner_offset[k][0][1]
				y2, x2 = i + corner_offset[k][1][0], j + corner_offset[k][1][1]
				y3, x3 = i + corner_offset[k][2][0], j + corner_offset[k][2][1]
				if y1 >= SIZE or y1 < 0 or x1 >= SIZE or x1 < 0:
					z1 = BLACK
				else:
					z1 = ds_img[y1, x1]
				if y2 >= SIZE or y2 < 0 or x2 >= SIZE or x2 < 0:
					z2 = BLACK
				else:
					z2 = ds_img[y2, x2]
				if y3 >= SIZE or y3 < 0 or x3 >= SIZE or x3 < 0:
					z3 = BLACK
				else:
					z3 = ds_img[y3, x3]
				q, r = hfunc(z0, z1, z2, z3)
				Q, R = Q + q, R + r

			if ds_img[i, j] == BLACK:
				output[i, j] = 0
			elif R == 4:
				output[i, j] = 5
			else:
				output[i, j] = Q

			if output[i, j] == 0:
				print(' ', end = '')
			else:
				print(output[i, j], end = '')
		print()



def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
	ret, bin_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
	height, width = img.shape
	ds_img = np.zeros((SIZE, SIZE), dtype = int)
	downsample(bin_img, ds_img)
	output = np.copy(ds_img)
	Yokoi(ds_img, output)


if __name__ == '__main__':
	main()