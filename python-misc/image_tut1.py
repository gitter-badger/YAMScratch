import os
import cv2

filename = 'ebola.jpg'

if os.path.isfile(filename):
	img = cv2.imread(filename,64)
	cv2.imshow('image',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()