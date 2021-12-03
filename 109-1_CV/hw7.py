import numpy as np
import cv2

BLACK = 0
WHITE = 255
SIZE = 64
MARKED = 0
UNMARKED = 255

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

def Yokoi_pixel(ds_img, i, j):
	if ds_img[i, j] == BLACK:
		return 0
	corner_offset = [[[0, 1], [-1, 1], [-1, 0]], [[-1, 0], [-1, -1], [0, -1]], [[0, -1], [1, -1], [1, 0]], [[1, 0], [1, 1], [0, 1]]]
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
	if R == 4:
		return 5
	else:
		return Q

def Yokoi(ds_img, yokoi_img):
	for i in range(SIZE):
		for j in range(SIZE):
			yokoi_img[i, j] = Yokoi_pixel(ds_img, i, j)


def pair_relationship(yokoi_img, marked_img):
	height, width = yokoi_img.shape
	neighbor_offset = [[0, 1], [-1, 0], [0, -1], [1, 0]]
	for i in range(height):
		for j in range(width):
			neighbor = 0
			if yokoi_img[i, j] == 1:
				for k in range(len(neighbor_offset)):
					y, x = i + neighbor_offset[k][0], j + neighbor_offset[k][1]
					if y >= SIZE or y < 0 or x >= SIZE or x < 0:
						continue
					if yokoi_img[y, x] == 1:
						neighbor += 1
			if neighbor >= 1:
				marked_img[i, j] = MARKED

def connected_shrink(ds_img, marked_img):
	change = 0
	height, width = ds_img.shape
	for i in range(height):
		for j in range(width):
			if Yokoi_pixel(ds_img, i, j) == 1 and marked_img[i, j] == MARKED:
				ds_img[i, j] = BLACK
				change = 1
	return change


def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
	ret, bin_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
	height, width = img.shape
	ds_img = np.zeros((SIZE, SIZE), dtype = int)
	downsample(bin_img, ds_img)
	change = 1
	while change:
		yokoi_img = np.copy(ds_img)
		marked_img = np.copy(ds_img)
		Yokoi(ds_img, yokoi_img)
		pair_relationship(yokoi_img, marked_img)
		change = connected_shrink(ds_img, marked_img)
	cv2.imwrite('thining_lena.bmp', ds_img)

if __name__ == '__main__':
	main()