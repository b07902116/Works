import numpy as np
import matplotlib.pyplot as plt
import cv2

def count_pixel(img):
	height, width = img.shape
	count = np.zeros(256, dtype = np.int32)
	for i in range(height):
		for j in range(width):
			count[img[i, j]] += 1
	return count

def equalization(count):
	total = 0
	new_val = np.zeros(256, dtype = np.int32)
	for i in range(256):
		total += count[i]
		new_val[i] = 255 * total / (512 * 512)
	return new_val

def output(count, img, char):
	plt.hist(np.arange(0, 256), bins = 256, facecolor = "black", weights = count)
	plt.title('histogram of problem %c' % char)
	plt.xlabel('grey value')
	plt.ylabel('pixel count')
	plt.savefig('hist%c.png' % char)
	plt.close()
	cv2.imwrite('image%c.bmp' % char, img)



def main():
	#problem a
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
	count = count_pixel(img)
	output(count, img, 'a')

	#problem b
	height, width = img.shape
	for i in range(height):
		for j in range(width):
			img[i, j] /= 3
	count = count_pixel(img)
	output(count, img, 'b')

	#problem c
	new_val = equalization(count)
	for i in range(height):
		for j in range(width):
			img[i, j] = new_val[img[i, j]]
	count = count_pixel(img)
	output(count, img, 'c')

if __name__ == '__main__':
	main()