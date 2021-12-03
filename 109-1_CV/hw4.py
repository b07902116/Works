import numpy as np
import cv2

BLACK = 0
WHITE = 255

def invert(img):
	width, height = img.shape
	for i in range(width):
		for j in range(height):
			if img[i, j] == WHITE:
				img[i, j] = BLACK
			else:
				img[i, j] = WHITE
	return

def dilation(img, kernel, x, y):
	width, height = img.shape
	for i in range(len(kernel)):
		col = x + kernel[i][0]
		row = y + kernel[i][1]
		if col < 0 or col >= width:		continue
		if row < 0 or row >= height:	continue
		img[col, row] = WHITE
	return

def erosion(img, bin_img, kernel, x, y):
	width, height = img.shape
	for i in range(len(kernel)):
		col = x + kernel[i][0]
		row = y + kernel[i][1]
		if col < 0 or col >= width:
			img[x, y] = BLACK
			return
		if row < 0 or row >= height:
			img[x, y] = BLACK
			return
		if bin_img[col, row] != WHITE:
			img[x, y] = BLACK
			return
	img[x, y] = WHITE
	return

def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
	ret, bin_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
	width, height = img.shape

	#initialize kernel
	kernel = []
	for i in range(-2, 3):
		for j in range(-2, 3):
			if i == -2 and j == -2:	continue
			if i == -2 and j == 2:	continue
			if i == 2 and j == -2:	continue
			if i == 2 and j == 2:	continue
			kernel.append([i, j])

	#L-shape
	kernelJ = [[1, 0], [0, 0], [0, -1]]
	kernelK = [[0, 1], [-1, 1], [-1, 0]]

	#kernelJ = [[-1, 0], [0, 0], [0, 1]]
	#kernelK = [[0, -1], [1, -1], [1, 0]]

	#dilation and erosion
	dilation_img = np.copy(bin_img)
	erosion_img = np.copy(bin_img)
	for i in range(width):
		for j in range(height):
			if bin_img[i, j] == WHITE:
				dilation(dilation_img, kernel, i, j)
			erosion(erosion_img, bin_img, kernel, i, j)
	cv2.imwrite('dilation.bmp', dilation_img)
	cv2.imwrite('erosion.bmp', erosion_img)

	#opening and closing
	opening_img = np.copy(erosion_img)
	closing_img = np.copy(dilation_img)
	for i in range(width):
		for j in range(height):
			if erosion_img[i, j] == WHITE:
				dilation(opening_img, kernel, i, j)
			erosion(closing_img, dilation_img, kernel, i, j)
	cv2.imwrite('opening.bmp', opening_img)
	cv2.imwrite('closing.bmp', closing_img)

	#hit and miss
	hit_img = np.copy(bin_img)
	inv_img = np.copy(bin_img)
	invert(inv_img)
	miss_img = np.copy(inv_img)
	for i in range(width):
		for j in range(height):
			erosion(hit_img, bin_img, kernelJ, i, j)
			erosion(miss_img, inv_img, kernelK, i, j)
	for i in range(width):
		for j in range(height):
			if hit_img[i, j] == WHITE and miss_img[i, j] == WHITE:
				hit_img[i, j] = WHITE
			else:
				hit_img[i, j] = BLACK
	cv2.imwrite('hit_and_miss.bmp', hit_img)


if __name__ == '__main__':
	main()