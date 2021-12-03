import numpy as np
import cv2

width = 512
height = 512

BLACK = 0
WHITE = 255


def edge_detect(img, offset, threshold, category):
	img_result = np.copy(img)
	for row in range(height):
		for col in range(width):
			max_val = float('-inf')
			square_sum = 0
			for i in range(len(offset)):
				mask_sum = 0
				for j in range(len(offset[i])):
					rtmp = row + offset[i][j][0]
					ctmp = col + offset[i][j][1]
					if rtmp < 0:		rtmp = 0
					if rtmp >= height:	rtmp = height - 1
					if ctmp < 0:		ctmp = 0
					if ctmp >= width:	ctmp = width - 1
					mask_sum = mask_sum + img[rtmp][ctmp] * offset[i][j][2]
				if (category == 0):
					square_sum = square_sum + mask_sum ** 2
				else:
					if max_val < mask_sum:
						max_val = mask_sum
			if (category == 0):
				grad = np.sqrt(square_sum)
			else:
				grad = max_val
			if grad >= threshold:
				img_result[row][col] = BLACK
			else:
				img_result[row][col] = WHITE
	return img_result



def main():
	img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)
	
	Roberts_offset = [	[[0, 0, -1], [1, 1, 1]],
						[[0, 1, -1], [1, 0, 1]]	]
	Roberts_img = edge_detect(img, Roberts_offset, 12, 0)
	cv2.imwrite('Roberts_img.bmp', Roberts_img)


	Prewitt_offset = [	[[-1, -1, -1], [-1, 0, -1], [-1, 1, -1], [1, -1, 1], [1, 0, 1], [1, 1, 1]],
						[[-1, -1, -1], [0, -1, -1], [1, -1, -1], [-1, 1, 1], [0, 1, 1], [1, 1, 1]] ]
	Prewitt_img = edge_detect(img, Prewitt_offset, 24, 0)
	cv2.imwrite('Prewitt_img.bmp', Prewitt_img)


	Sobel_offset = [	[[-1, -1, -1], [-1, 0, -2], [-1, 1, -1], [1, -1, 1], [1, 0, 2], [1, 1, 1]],
						[[-1, -1, -1], [0, -1, -2], [1, -1, -1], [-1, 1, 1], [0, 1, 2], [1, 1, 1]] ]
	Sobel_img = edge_detect(img, Sobel_offset, 38, 0)
	cv2.imwrite('Sobel_img.bmp', Sobel_img)


	Frei_and_Chen_offset = [	[[-1, -1, -1], [-1, 0, -np.sqrt(2)], [-1, 1, -1], [1, -1, 1], [1, 0, np.sqrt(2)], [1, 1, 1]],
								[[-1, -1, -1], [0, -1, -np.sqrt(2)], [1, -1, -1], [-1, 1, 1], [0, 1, np.sqrt(2)], [1, 1, 1]] ]
	Frei_and_Chen_img = edge_detect(img, Frei_and_Chen_offset, 30, 0)
	cv2.imwrite('Frei_and_Chen_img.bmp', Frei_and_Chen_img)


	Kirsch_offset = [	[[-1, -1, -3], [-1, 0, -3], [-1, 1,  5], [0, -1, -3], [0, 1,  5], [1, -1, -3], [1, 0, -3], [1, 1,  5]],
						[[-1, -1, -3], [-1, 0,  5], [-1, 1,  5], [0, -1, -3], [0, 1,  5], [1, -1, -3], [1, 0, -3], [1, 1, -3]],
						[[-1, -1,  5], [-1, 0,  5], [-1, 1,  5], [0, -1, -3], [0, 1, -3], [1, -1, -3], [1, 0, -3], [1, 1, -3]],
						[[-1, -1,  5], [-1, 0,  5], [-1, 1, -3], [0, -1,  5], [0, 1, -3], [1, -1, -3], [1, 0, -3], [1, 1, -3]],
						[[-1, -1,  5], [-1, 0, -3], [-1, 1, -3], [0, -1,  5], [0, 1, -3], [1, -1,  5], [1, 0, -3], [1, 1, -3]],
						[[-1, -1, -3], [-1, 0, -3], [-1, 1, -3], [0, -1,  5], [0, 1, -3], [1, -1,  5], [1, 0,  5], [1, 1, -3]],
						[[-1, -1, -3], [-1, 0, -3], [-1, 1, -3], [0, -1, -3], [0, 1, -3], [1, -1,  5], [1, 0,  5], [1, 1,  5]],
						[[-1, -1, -3], [-1, 0, -3], [-1, 1, -3], [0, -1, -3], [0, 1,  5], [1, -1, -3], [1, 0,  5], [1, 1,  5]] ]
	Kirsch_img = edge_detect(img, Kirsch_offset, 135, 1)
	cv2.imwrite('Kirsch_img.bmp', Kirsch_img)


	Robinson_offset = [	[[-1, -1, -1], [-1, 1,  1], [0, -1, -2], [0, 1,  2], [1, -1, -1], [1, 1,  1]],
						[[-1, -1,  1], [-1, 1, -1], [0, -1,  2], [0, 1, -2], [1, -1,  1], [1, 1, -1]],
						[[-1, 0,  1], [-1, 1,  2], [0, -1, -1], [0, 1,  1], [1, -1, -2], [1, 0, -1]],
						[[-1, 0, -1], [-1, 1, -2], [0, -1,  1], [0, 1, -1], [1, -1,  2], [1, 0,  1]],
						[[-1, -1,  1], [-1, 0,  2], [-1, 1,  1], [1, -1, -1], [1, 0, -2], [1, 1, -1]],
						[[-1, -1, -1], [-1, 0, -2], [-1, 1, -1], [1, -1,  1], [1, 0,  2], [1, 1,  1]],
						[[-1, -1,  2], [-1, 0,  1], [0, -1,  1], [0, 1, -1], [1, 0, -1], [1, 1, -2]],
						[[-1, -1, -2], [-1, 0, -1], [0, -1, -1], [0, 1,  1], [1, 0,  1], [1, 1,  2]] ]

	Robinson_img = edge_detect(img, Robinson_offset, 43, 1)
	cv2.imwrite('Robinson_img.bmp', Robinson_img)


	Nevatia_Babu_offset = [[[-2, -2, 100], [-2, -1, 100], [-2, 0, 100], [-2, 1, 100], [-2, 2, 100],
							[-1, -2, 100], [-1, -1, 100], [-1, 0, 100], [-1, 1, 100], [-1, 2, 100],
							[1, -2, -100], [1, -1, -100], [1, 0, -100], [1, 1, -100], [1, 2, -100],
							[2, -2, -100], [2, -1, -100], [2, 0, -100], [2, 1, -100], [2, 2, -100] ],

						   [[-2, -2, 100], [-2, -1, 100], [-2, 0, 100], [-2, 1, 100], [-2, 2, 100],
							[-1, -2, 100], [-1, -1, 100], [-1, 0, 100], [-1, 1,  78], [-1, 2, -32],
							[0, -2,  100], [0, -1,   92],				[0, 1,  -92], [0, 2, -100],
							[1, -2,   32], [1, -1,  -78], [1, 0, -100], [1, 1, -100], [1, 2, -100],
							[2, -2, -100], [2, -1, -100], [2, 0, -100], [2, 1, -100], [2, 2, -100] ],

						   [[-2, -2, 100], [-2, -1, 100], [-2, 0, 100], [-2, 1,  32], [-2, 2, -100],
							[-1, -2, 100], [-1, -1, 100], [-1, 0,  92], [-1, 1, -78], [-1, 2, -100],
							[0, -2,  100], [0, -1,  100],				[0, 1, -100], [0, 2,  -100],
							[1, -2,  100], [1, -1,   78], [1, 0,  -92], [1, 1, -100], [1, 2,  -100],
							[2, -2,  100], [2, -1,  -32], [2, 0, -100], [2, 1, -100], [2, 2,  -100] ],

						   [[-2, -2, -100], [-2, -1, -100], [-2, 1, 100], [-2, 2, 100],
							[-1, -2, -100], [-1, -1, -100], [-1, 1, 100], [-1, 2, 100],
							[0, -2,  -100], [0, -1,  -100],	[0, 1,  100], [0, 2,  100],
							[1, -2,  -100], [1, -1,  -100], [1, 1,  100], [1, 2,  100],
							[2, -2,  -100], [2, -1,  -100], [2, 1,  100], [2, 2,  100] ],

						   [[-2, -2, -100], [-2, -1,   32], [-2, 0, 100], [-2, 1, 100], [-2, 2, 100],
							[-1, -2, -100], [-1, -1,  -78], [-1, 0,  92], [-1, 1, 100], [-1, 2, 100],
							[0, -2,  -100], [0, -1,  -100],				  [0, 1,  100], [0, 2,  100],
							[1, -2,  -100], [1, -1,  -100], [1, 0,  -92], [1, 1,   78], [1, 2,  100],
							[2, -2,  -100], [2, -1,  -100], [2, 0, -100], [2, 1,  -32], [2, 2,  100] ],

						   [[-2, -2, 100], [-2, -1, 100], [-2, 0, 100], [-2, 1, 100], [-2, 2, 100],
							[-1, -2, -32], [-1, -1,  78], [-1, 0, 100], [-1, 1, 100], [-1, 2, 100],
							[0, -2, -100], [0, -1,  -92],				[0, 1,   92], [0, 2,  100],
							[1, -2, -100], [1, -1, -100], [1, 0, -100], [1, 1,  -78], [1, 2,   32],
							[2, -2, -100], [2, -1, -100], [2, 0, -100], [2, 1, -100], [2, 2, -100] ] ]

	Nevatia_Babu_img = edge_detect(img, Nevatia_Babu_offset, 12500, 1)
	cv2.imwrite('Nevatia_Babu_img.bmp', Nevatia_Babu_img)

if __name__ == '__main__':
	main()