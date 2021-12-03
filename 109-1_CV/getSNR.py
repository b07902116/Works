import numpy as np
import cv2

width = 512
height = 512


def getVS(img, US):
	total = np.float64(0);
	for i in range(height):
		for j in range(width):
			total = total + (img[i, j] - US) ** 2
	return np.sqrt(total / (width * height))

def getUN(img_noise, img):
	total = np.float64(0);
	for i in range(height):
		for j in range(width):
			total = total + (img_noise[i, j] - img[i, j])
	return total / (width * height)

def getVN(img_noise, img, UN):
	total = np.float64(0);
	for i in range(height):
		for j in range(width):
			total = total + (img_noise[i, j] - img[i, j] - UN) ** 2
	return np.sqrt(total / (width * height))

def getSNR(img_noise, img, US, VS):
	UN = getUN(img_noise, img)
	VN = getVN(img_noise, img, UN)
	SNR = 20 * np.log10(VS / VN)
	return SNR

def main():
	ori_img = np.float64(cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE))
	US = np.average(ori_img)
	VS = getVS(ori_img, US)

	#a
	img = np.float64(cv2.imread('gauss_noisy_img_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('gauss_noisy_img_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('gauss_noisy_img_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('gauss_noisy_img_30 : ', getSNR(img, ori_img, US, VS))

	#b
	img = np.float64(cv2.imread('salt_pepper_img_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('salt_pepper_img_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('salt_pepper_img_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('salt_pepper_img_10 : ', getSNR(img, ori_img, US, VS))

	#c
	img = np.float64(cv2.imread('box_filter_3x3_gauss_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_3x3_gauss_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_5x5_gauss_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_5x5_gauss_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_3x3_gauss_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_3x3_gauss_30 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_5x5_gauss_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_5x5_gauss_30 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_3x3_salt_pepper_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_3x3_salt_pepper_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_5x5_salt_pepper_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_5x5_salt_pepper_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_3x3_salt_pepper_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_3x3_salt_pepper_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('box_filter_5x5_salt_pepper_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('box_filter_5x5_salt_pepper_10 : ', getSNR(img, ori_img, US, VS))

	#d
	img = np.float64(cv2.imread('median_filter_3x3_gauss_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_3x3_gauss_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_5x5_gauss_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_5x5_gauss_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_3x3_gauss_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_3x3_gauss_30 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_5x5_gauss_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_5x5_gauss_30 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_3x3_salt_pepper_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_3x3_salt_pepper_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_5x5_salt_pepper_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_5x5_salt_pepper_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_3x3_salt_pepper_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_3x3_salt_pepper_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('median_filter_5x5_salt_pepper_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('median_filter_5x5_salt_pepper_10 : ', getSNR(img, ori_img, US, VS))

	#e
	img = np.float64(cv2.imread('OPCL_gauss_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('OPCL_gauss_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('OPCL_gauss_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('OPCL_gauss_30 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('OPCL_salt_pepper_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('OPCL_salt_pepper_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('OPCL_salt_pepper_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('OPCL_salt_pepper_10 : ', getSNR(img, ori_img, US, VS))

	img = np.float64(cv2.imread('CLOP_gauss_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('CLOP_gauss_10 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('CLOP_gauss_30.bmp', cv2.IMREAD_GRAYSCALE))
	print('CLOP_gauss_30 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('CLOP_salt_pepper_05.bmp', cv2.IMREAD_GRAYSCALE))
	print('CLOP_salt_pepper_05 : ', getSNR(img, ori_img, US, VS))
	img = np.float64(cv2.imread('CLOP_salt_pepper_10.bmp', cv2.IMREAD_GRAYSCALE))
	print('CLOP_salt_pepper_10 : ', getSNR(img, ori_img, US, VS))


if __name__ == '__main__':
	main()