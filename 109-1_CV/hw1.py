import numpy as np
import cv2

img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
(height, width) = img.shape


def upside_down():
	imga = np.empty((height, width))
	for i in range(height):
		imga[height - 1 - i] = img[i]
	cv2.imwrite('outputa.bmp', imga)

def rightside_left():
	imgb = np.empty((height, width))
	for i in range(width):
		imgb[:, width - 1 - i] = img[:, i]
	cv2.imwrite('outputb.bmp', imgb)

def diagnal_flip():
	imgc = np.empty((height, width))
	for i in range(height):
		imgc[i, :] = img[:, i]
	cv2.imwrite('outputc.bmp', imgc)

def rotate_45_degree():
	M = cv2.getRotationMatrix2D((height / 2, width / 2), -45, 1)
	imgd = cv2.warpAffine(img, M, (height, width), borderMode = 0, borderValue = 255)
	cv2.imwrite('outputd.bmp', imgd)

def shrink_half():
	imge = cv2.resize(img, (int(height / 2), int(width / 2)))
	cv2.imwrite('outpute.bmp', imge)

def binarize():
	ret, imgf = cv2.threshold(img, 128, 255, cv2.THRESH_BINARY)
	cv2.imwrite('outputf.bmp', imgf)


def main():
	#Part1
	upside_down()
	rightside_left()
	diagnal_flip()
	#Part2
	rotate_45_degree()
	shrink_half()
	binarize()


if __name__ == '__main__':
	main()