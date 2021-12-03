import numpy as np
import cv2

BLACK = 0
WHITE = 255


def dilation(img, ori_img, kernel, x, y):
	width, height = img.shape
	maxval = 0
	for i in range(len(kernel)):
		col = x + kernel[i][0]
		row = y + kernel[i][1]
		if col < 0 or col >= width:		continue
		if row < 0 or row >= height:	continue
		if ori_img[col, row] > maxval:
			maxval = ori_img[col, row]
	img[x, y] = maxval
	return

def erosion(img, ori_img, kernel, x, y):
	width, height = img.shape
	minval = 255
	for i in range(len(kernel)):
		col = x + kernel[i][0]
		row = y + kernel[i][1]
		if col < 0 or col >= width:		return
		if row < 0 or row >= height:	return
		if ori_img[col, row] < minval:
			minval = ori_img[col, row]
	img[x, y] = minval
	return

def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
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

	#dilation and erosion
	dilation_img = np.copy(img)
	erosion_img = np.copy(img)
	for i in range(width):
		for j in range(height):
			dilation(dilation_img, img, kernel, i, j)
			erosion(erosion_img, img, kernel, i, j)
	cv2.imwrite('dilation.bmp', dilation_img)
	cv2.imwrite('erosion.bmp', erosion_img)

	#opening and closing
	opening_img = np.copy(erosion_img)
	closing_img = np.copy(dilation_img)
	for i in range(width):
		for j in range(height):
			dilation(opening_img, erosion_img, kernel, i, j)
			erosion(closing_img, dilation_img, kernel, i, j)
	cv2.imwrite('opening.bmp', opening_img)
	cv2.imwrite('closing.bmp', closing_img)



if __name__ == '__main__':
	main()