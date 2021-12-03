import numpy as np
import cv2
import random

width = 512
height = 512

kernel = [			  [-2, -1], [-2, 0], [-2, 1],
			[-1, -2], [-1, -1], [-1, 0], [-1, 1], [-1, 2],
			[ 0, -2], [ 0, -1], [ 0, 0], [ 0, 1], [ 0, 2],
			[ 1, -2], [ 1, -1], [ 1, 0], [ 1, 1], [ 1, 2],
					  [ 2, -1], [ 2, 0], [ 2, 1]]

def gauss_noise(img, amp):
	img_result = np.copy(img)
	for i in range(height):
		for j in range(width):
			val = int(img_result[i, j] + (amp * random.gauss(0, 1)))
			if val > 255:
				val = 255
			elif val < 0:
				val = 0
			img_result[i, j] = val
	return img_result

def salt_pepper(img, threshold):
	img_result = np.copy(img)
	for i in range(height):
		for j in range(width):
			val = random.uniform(0, 1)
			if val <= threshold:
				img_result[i, j] = 0
			elif val >= 1 - threshold:
				img_result[i, j] = 255				
	return img_result

def box_filter(img, size):
	img_result = np.copy(img)
	hs = size // 2
	for i in range(height):
		for j in range(width):
			num = 0
			total = 0
			for p in range(i - hs, i + hs + 1):
				if p < 0 or p >= height:	continue
				for q in range(j - hs, j + hs + 1):
					if q < 0 or q >= width:	continue
					total = total + img[p, q]
					num = num + 1
			img_result[i, j] = total // num
	return img_result

def median_filter(img, size):
	img_result = np.copy(img)
	hs = size // 2
	for i in range(height):
		for j in range(width):
			pixels = []
			num = 0
			for p in range(i - hs, i + hs + 1):
				if p < 0 or p >= height:	continue
				for q in range(j - hs, j + hs + 1):
					if q < 0 or q >= width:	continue
					pixels.append(img[p, q])
					num = num + 1
			pixels.sort()
			if num % 2:
				img_result[i, j] = pixels[num // 2]
			else:
				img_result[i, j] = (int(pixels[num // 2]) + int(pixels[num // 2 + 1])) // 2
	return img_result

def dilation(img, ori_img, x, y):
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

def erosion(img, ori_img, x, y):
	minval = 255
	for i in range(len(kernel)):
		col = x + kernel[i][0]
		row = y + kernel[i][1]
		if col < 0 or col >= width:		continue
		if row < 0 or row >= height:	continue
		if ori_img[col, row] < minval:
			minval = ori_img[col, row]
	img[x, y] = minval
	return

def opening(img):
	img_result = np.copy(img)
	img_erosion = np.copy(img)
	for i in range(height):
		for j in range(width):
			erosion(img_erosion, img, i, j)
	for i in range(height):
		for j in range(width):
			dilation(img_result, img_erosion, i, j)
	return img_result

def closing(img):
	img_result = np.copy(img)
	img_dilation = np.copy(img)
	for i in range(height):
		for j in range(width):
			dilation(img_dilation, img, i, j)
	for i in range(height):
		for j in range(width):
			erosion(img_result, img_dilation, i, j)
	return img_result

def opening_then_closing(img):
	img_result = np.copy(img)
	img_opening = opening(img)
	img_result = closing(img_opening)
	return img_result

def closing_then_opening(img):
	img_result = np.copy(img)
	img_closing = closing(img)
	img_result = opening(img_closing)
	return img_result


def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)


	#generate noisy image
	gauss_noisy_img_10 = gauss_noise(img, 10)
	gauss_noisy_img_30 = gauss_noise(img, 30)
	salt_pepper_img_05 = salt_pepper(img, 0.05)
	salt_pepper_img_10 = salt_pepper(img, 0.10)

	#noisy image
	cv2.imwrite('gauss_noisy_img_10.bmp', gauss_noisy_img_10)
	cv2.imwrite('gauss_noisy_img_30.bmp', gauss_noisy_img_30)
	cv2.imwrite('salt_pepper_img_05.bmp', salt_pepper_img_05)
	cv2.imwrite('salt_pepper_img_10.bmp', salt_pepper_img_10)


	#box filter
	box_filter_3x3_gauss_10 = box_filter(gauss_noisy_img_10, 3)
	box_filter_5x5_gauss_10 = box_filter(gauss_noisy_img_10, 5)
	box_filter_3x3_gauss_30 = box_filter(gauss_noisy_img_30, 3)
	box_filter_5x5_gauss_30 = box_filter(gauss_noisy_img_30, 5)

	box_filter_3x3_salt_pepper_05 = box_filter(salt_pepper_img_05, 3)
	box_filter_5x5_salt_pepper_05 = box_filter(salt_pepper_img_05, 5)
	box_filter_3x3_salt_pepper_10 = box_filter(salt_pepper_img_10, 3)
	box_filter_5x5_salt_pepper_10 = box_filter(salt_pepper_img_10, 5)

	cv2.imwrite('box_filter_3x3_gauss_10.bmp', box_filter_3x3_gauss_10)
	cv2.imwrite('box_filter_5x5_gauss_10.bmp', box_filter_5x5_gauss_10)
	cv2.imwrite('box_filter_3x3_gauss_30.bmp', box_filter_3x3_gauss_30)
	cv2.imwrite('box_filter_5x5_gauss_30.bmp', box_filter_5x5_gauss_30)

	cv2.imwrite('box_filter_3x3_salt_pepper_05.bmp', box_filter_3x3_salt_pepper_05)
	cv2.imwrite('box_filter_5x5_salt_pepper_05.bmp', box_filter_5x5_salt_pepper_05)
	cv2.imwrite('box_filter_3x3_salt_pepper_10.bmp', box_filter_3x3_salt_pepper_10)
	cv2.imwrite('box_filter_5x5_salt_pepper_10.bmp', box_filter_5x5_salt_pepper_10)


	#median filter
	median_filter_3x3_gauss_10 = median_filter(gauss_noisy_img_10, 3)
	median_filter_5x5_gauss_10 = median_filter(gauss_noisy_img_10, 5)
	median_filter_3x3_gauss_30 = median_filter(gauss_noisy_img_30, 3)
	median_filter_5x5_gauss_30 = median_filter(gauss_noisy_img_30, 5)

	median_filter_3x3_salt_pepper_05 = median_filter(salt_pepper_img_05, 3)
	median_filter_5x5_salt_pepper_05 = median_filter(salt_pepper_img_05, 5)
	median_filter_3x3_salt_pepper_10 = median_filter(salt_pepper_img_10, 3)
	median_filter_5x5_salt_pepper_10 = median_filter(salt_pepper_img_10, 5)


	cv2.imwrite('median_filter_3x3_gauss_10.bmp', median_filter_3x3_gauss_10)
	cv2.imwrite('median_filter_5x5_gauss_10.bmp', median_filter_5x5_gauss_10)
	cv2.imwrite('median_filter_3x3_gauss_30.bmp', median_filter_3x3_gauss_30)
	cv2.imwrite('median_filter_5x5_gauss_30.bmp', median_filter_5x5_gauss_30)

	cv2.imwrite('median_filter_3x3_salt_pepper_05.bmp', median_filter_3x3_salt_pepper_05)
	cv2.imwrite('median_filter_5x5_salt_pepper_05.bmp', median_filter_5x5_salt_pepper_05)
	cv2.imwrite('median_filter_3x3_salt_pepper_10.bmp', median_filter_3x3_salt_pepper_10)
	cv2.imwrite('median_filter_5x5_salt_pepper_10.bmp', median_filter_5x5_salt_pepper_10)



	#opening-then-closing and closing-then-opening filter
	OPCL_gauss_10 = opening_then_closing(gauss_noisy_img_10)
	OPCL_gauss_30 = opening_then_closing(gauss_noisy_img_30)
	OPCL_salt_pepper_05 = opening_then_closing(salt_pepper_img_05)
	OPCL_salt_pepper_10 = opening_then_closing(salt_pepper_img_10)

	CLOP_gauss_10 = closing_then_opening(gauss_noisy_img_10)
	CLOP_gauss_30 = closing_then_opening(gauss_noisy_img_30)
	CLOP_salt_pepper_05 = closing_then_opening(salt_pepper_img_05)
	CLOP_salt_pepper_10 = closing_then_opening(salt_pepper_img_10)


	cv2.imwrite('OPCL_gauss_10.bmp', OPCL_gauss_10)
	cv2.imwrite('OPCL_gauss_30.bmp', OPCL_gauss_30)
	cv2.imwrite('OPCL_salt_pepper_05.bmp', OPCL_salt_pepper_05)
	cv2.imwrite('OPCL_salt_pepper_10.bmp', OPCL_salt_pepper_10)

	cv2.imwrite('CLOP_gauss_10.bmp', CLOP_gauss_10)
	cv2.imwrite('CLOP_gauss_30.bmp', CLOP_gauss_30)
	cv2.imwrite('CLOP_salt_pepper_05.bmp', CLOP_salt_pepper_05)
	cv2.imwrite('CLOP_salt_pepper_10.bmp', CLOP_salt_pepper_10)




if __name__ == '__main__':
	main()