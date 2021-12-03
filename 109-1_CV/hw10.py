import numpy as np
import cv2

width = 512
height = 512

BLACK = 0
WHITE = 255

P_ONE = 255
ZERO = 128
N_ONE = 0


def Laplacian(img, offset, threshold):
	img_result = np.copy(img)
	for row in range(height):
		for col in range(width):
			grad = 0
			for i in range(len(offset)):
				rtmp = row + offset[i][0]
				ctmp = col + offset[i][1]
				if rtmp < 0:		rtmp = 0
				if rtmp >= height:	rtmp = height - 1
				if ctmp < 0:		ctmp = 0
				if ctmp >= width:	ctmp = width - 1
				grad = grad + img[rtmp][ctmp] * offset[i][2]
			if grad >= threshold:
				img_result[row][col] = P_ONE
			elif grad <= -threshold:
				img_result[row][col] = N_ONE
			else:
				img_result[row][col] = ZERO
	return img_result

def Zero_Crossing(Laplacian_img):
	offset = [[-1, -1], [-1, 0], [-1, 1], [0, -1], [0, 1], [1, -1], [1, 0], [1, 1]]
	img_result = np.copy(Laplacian_img)
	for row in range(height):
		for col in range(width):
			cross = 0
			if (Laplacian_img[row][col] == P_ONE):
				for i in range(len(offset)):
					rtmp = row + offset[i][0]
					ctmp = col + offset[i][1]
					if rtmp < 0:		rtmp = 0
					if rtmp >= height:	rtmp = height - 1
					if ctmp < 0:		ctmp = 0
					if ctmp >= width:	ctmp = width - 1
					if Laplacian_img[rtmp][ctmp] == N_ONE:
						cross = 1
						break
			if (cross):
				img_result[row][col] = BLACK
			else:
				img_result[row][col] = WHITE
	return img_result


def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)


	Laplace_Mask1_offset = [			[-1, 0, 1],
							[0, -1, 1], [0, 0, -4], [0, 1, 1],
										[1, 0,  1]				]
	Laplacian_Mask1 = Laplacian(img, Laplace_Mask1_offset, 15)
	Laplace_Mask1_img = Zero_Crossing(Laplacian_Mask1)
	cv2.imwrite('Laplace_Mask1_img.bmp', Laplace_Mask1_img)
	

	Laplace_Mask2_offset = [[-1, -1, 1/3], [-1, 0,  1/3], [-1, 1, 1/3],
							[ 0, -1, 1/3], [ 0, 0, -8/3], [ 0, 1, 1/3],
							[ 1, -1, 1/3], [ 1, 0,  1/3], [ 1, 1, 1/3]	]
	Laplacian_Mask2 = Laplacian(img, Laplace_Mask2_offset, 15)
	Laplace_Mask2_img = Zero_Crossing(Laplacian_Mask2)
	cv2.imwrite('Laplace_Mask2_img.bmp', Laplace_Mask2_img)


	Minimum_Variance_offset = [	[-1, -1,  2/3], [-1, 0, -1/3], [-1, 1,  2/3],
								[ 0, -1, -1/3], [ 0, 0, -4/3], [ 0, 1, -1/3],
								[ 1, -1,  2/3], [ 1, 0, -1/3], [ 1, 1,  2/3]	]
	Laplacian_Minimum_Variance = Laplacian(img, Minimum_Variance_offset, 20)
	Minimum_Variance_img = Zero_Crossing(Laplacian_Minimum_Variance)
	cv2.imwrite('Minimum_Variance_img.bmp', Minimum_Variance_img)


	LOG_offset = [	[-5, -5,  0], [-5, -4,  0], [-5, -3,   0], [-5, -2,  -1], [-5, -1,  -1], [-5, 0,  -2], [-5, 1,  -1], [-5, 2,  -1], [-5, 3,   0], [-5, 4,  0], [-5, 5,  0],
					[-4, -5,  0], [-4, -4,  0], [-4, -3,  -2], [-4, -2,  -4], [-4, -1,  -8], [-4, 0,  -9], [-4, 1,  -8], [-4, 2,  -4], [-4, 3,  -2], [-4, 4,  0], [-4, 5,  0],
					[-3, -5,  0], [-3, -4, -2], [-3, -3,  -7], [-3, -2, -15], [-3, -1, -22], [-3, 0, -23], [-3, 1, -22], [-3, 2, -15], [-3, 3,  -7], [-3, 4, -2], [-3, 5,  0],
					[-2, -5, -1], [-2, -4, -4], [-2, -3, -15], [-2, -2, -24], [-2, -1, -14], [-2, 0,  -1], [-2, 1, -14], [-2, 2, -24], [-2, 3, -15], [-2, 4, -4], [-2, 5, -1],
					[-1, -5, -1], [-1, -4, -8], [-1, -3, -22], [-1, -2, -14], [-1, -1,  52], [-1, 0, 103], [-1, 1,  52], [-1, 2, -14], [-1, 3, -22], [-1, 4, -8], [-1, 5, -1],
					[ 0, -5, -2], [ 0, -4, -9], [ 0, -3, -23], [ 0, -2,  -1], [ 0, -1, 103], [ 0, 0, 178], [ 0, 1, 103], [ 0, 2,  -1], [ 0, 3, -23], [ 0, 4, -9], [ 0, 5, -2],
					[ 1, -5, -1], [ 1, -4, -8], [ 1, -3, -22], [ 1, -2, -14], [ 1, -1,  52], [ 1, 0, 103], [ 1, 1,  52], [ 1, 2, -14], [ 1, 3, -22], [ 1, 4, -8], [ 1, 5, -1],
					[ 2, -5, -1], [ 2, -4, -4], [ 2, -3, -15], [ 2, -2, -24], [ 2, -1, -14], [ 2, 0,  -1], [ 2, 1, -14], [ 2, 2, -24], [ 2, 3, -15], [ 2, 4, -4], [ 2, 5, -1],
					[ 3, -5,  0], [ 3, -4, -2], [ 3, -3,  -7], [ 3, -2, -15], [ 3, -1, -22], [ 3, 0, -23], [ 3, 1, -22], [ 3, 2, -15], [ 3, 3,  -7], [ 3, 4, -2], [ 3, 5,  0],
					[ 4, -5,  0], [ 4, -4,  0], [ 4, -3,  -2], [ 4, -2,  -4], [ 4, -1,  -8], [ 4, 0,  -9], [ 4, 1,  -8], [ 4, 2,  -4], [ 4, 3,  -2], [ 4, 4,  0], [ 4, 5,  0],
					[ 5, -5,  0], [ 5, -4,  0], [ 5, -3,   0], [ 5, -2,  -1], [ 5, -1,  -1], [ 5, 0,  -2], [ 5, 1,  -1], [ 5, 2,  -1], [ 5, 3,   0], [ 5, 4,  0], [ 5, 5,  0] ]
	Laplacian_LOG = Laplacian(img, LOG_offset, 3000)
	LOG_img = Zero_Crossing(Laplacian_LOG)
	cv2.imwrite('LOG_img.bmp', LOG_img)


	DOG_offset = [	[-5, -5, -1], [-5, -4,  -3], [-5, -3,  -4], [-5, -2,  -6], [-5, -1,  -7], [-5, 0,  -8], [-5, 1,  -7], [-5, 2,  -6], [-5, 3,  -4], [-5, 4,  -3], [-5, 5, -1],
					[-4, -5, -3], [-4, -4,  -5], [-4, -3,  -8], [-4, -2, -11], [-4, -1, -13], [-4, 0, -13], [-4, 1, -13], [-4, 2, -11], [-4, 3,  -8], [-4, 4,  -5], [-4, 5, -3],
					[-3, -5, -4], [-3, -4,  -8], [-3, -3, -12], [-3, -2, -16], [-3, -1, -17], [-3, 0, -17], [-3, 1, -17], [-3, 2, -16], [-3, 3, -12], [-3, 4,  -8], [-3, 5, -4],
					[-2, -5, -6], [-2, -4, -11], [-2, -3, -16], [-2, -2, -16], [-2, -1,   0], [-2, 0,  15], [-2, 1,   0], [-2, 2, -16], [-2, 3, -16], [-2, 4, -11], [-2, 5, -6],
					[-1, -5, -7], [-1, -4, -13], [-1, -3, -17], [-1, -2,   0], [-1, -1,  85], [-1, 0, 160], [-1, 1,  85], [-1, 2,   0], [-1, 3, -17], [-1, 4, -13], [-1, 5, -7],
					[ 0, -5, -8], [ 0, -4, -13], [ 0, -3, -17], [ 0, -2,  15], [ 0, -1, 160], [ 0, 0, 283], [ 0, 1, 160], [ 0, 2,  15], [ 0, 3, -17], [ 0, 4, -13], [ 0, 5, -8],
					[ 1, -5, -7], [ 1, -4, -13], [ 1, -3, -17], [ 1, -2,   0], [ 1, -1,  85], [ 1, 0, 160], [ 1, 1,  85], [ 1, 2,   0], [ 1, 3, -17], [ 1, 4, -13], [ 1, 5, -7],
					[ 2, -5, -6], [ 2, -4, -11], [ 2, -3, -16], [ 2, -2, -16], [ 2, -1,   0], [ 2, 0,  15], [ 2, 1,   0], [ 2, 2, -16], [ 2, 3, -16], [ 2, 4, -11], [ 2, 5, -6],
					[ 3, -5, -4], [ 3, -4,  -8], [ 3, -3, -12], [ 3, -2, -16], [ 3, -1, -17], [ 3, 0, -17], [ 3, 1, -17], [ 3, 2, -16], [ 3, 3, -12], [ 3, 4,  -8], [ 3, 5, -4],
					[ 4, -5, -3], [ 4, -4,  -5], [ 4, -3,  -8], [ 4, -2, -11], [ 4, -1, -13], [ 4, 0, -13], [ 4, 1, -13], [ 4, 2, -11], [ 4, 3,  -8], [ 4, 4,  -5], [ 4, 5, -3],
					[ 5, -5, -1], [ 5, -4,  -3], [ 5, -3,  -4], [ 5, -2,  -6], [ 5, -1,  -7], [ 5, 0,  -8], [ 5, 1,  -7], [ 5, 2,  -6], [ 5, 3,  -4], [ 5, 4,  -3], [ 5, 5, -1] ]
	Laplacian_DOG = Laplacian(img, DOG_offset, 1)
	DOG_img = Zero_Crossing(Laplacian_DOG)
	cv2.imwrite('DOG_img.bmp', DOG_img)



if __name__ == '__main__':
	main()