# import
import numpy as np
import cv2
from argparse import ArgumentParser

class FaceAligner:
    def __init__(self, desiredLeftEye=(0.35, 0.35),
        desiredFaceWidth=112, desiredFaceHeight=None):
        # store the facial landmark predictor, desired output left
        # eye position, and desired output face width + height
        self.desiredLeftEye = desiredLeftEye
        self.desiredFaceWidth = desiredFaceWidth
        self.desiredFaceHeight = desiredFaceHeight
        
        # if the desired face height is None, set it to be the
        # desired face width (normal behavior)
        if self.desiredFaceHeight is None:
            self.desiredFaceHeight = self.desiredFaceWidth
            
    def align(self, image, leftEyeCenter, rightEyeCenter):
        # compute the angle between the eye centroids
        dY = rightEyeCenter[1] - leftEyeCenter[1]
        dX = rightEyeCenter[0] - leftEyeCenter[0]
        angle = np.degrees(np.arctan2(dY, dX))
        
        # compute the desired right eye x-coordinate based on the
        # desired x-coordinate of the left eye
        desiredRightEyeX = 1.0 - self.desiredLeftEye[0]
        # determine the scale of the new resulting image by taking
        # the ratio of the distance between eyes in the *current*
        # image to the ratio of distance between eyes in the
        # *desired* image
        dist = np.sqrt((dX ** 2) + (dY ** 2))
        desiredDist = (desiredRightEyeX - self.desiredLeftEye[0])
        desiredDist *= self.desiredFaceWidth
        scale = desiredDist / dist
        
        # compute center (x, y)-coordinates (i.e., the median point)
        # between the two eyes in the input image
        eyesCenter = ((leftEyeCenter[0] + rightEyeCenter[0]) // 2,
            (leftEyeCenter[1] + rightEyeCenter[1]) // 2)
        # grab the rotation matrix for rotating and scaling the face
        M = cv2.getRotationMatrix2D(eyesCenter, angle, scale)
        print(M)
        # update the translation component of the matrix
        tX = self.desiredFaceWidth * 0.5
        tY = self.desiredFaceHeight * self.desiredLeftEye[1]
        M[0, 2] += (tX - eyesCenter[0])
        M[1, 2] += (tY - eyesCenter[1])
        print(M)
        
        # apply the affine transformation
        (w, h) = (self.desiredFaceWidth, self.desiredFaceHeight)
        output = cv2.warpAffine(image, M, (w, h),
            flags=cv2.INTER_CUBIC)
        # return the aligned face
        return output

def parse(filename):
    reye, leye = [], []
    with open(filename) as f:
        leye = f.readline().strip().split(" ")
        reye = f.readline().strip().split(" ")
    leye = [int(i) for i in leye]
    reye = [int(i) for i in reye]
    return leye, reye
            

def main():
    ap = ArgumentParser()
    
    ap.add_argument("-i", "--image", required=True, help="path to input image")
    
    ap.add_argument("-p", "--part", required=True, help="path to locations of face parts (left eyes, right eyes)")
    
    args = vars(ap.parse_args())
    
    imgPath, partPath = args["image"], args["part"]
    
    image = cv2.imread(imgPath)
    leye, reye = parse(partPath)
    
    aligner = FaceAligner()
    
    output = aligner.align(image, leye, reye)
    cv2.imshow("Aligned", output)
    cv2.waitKey()
    
if __name__ == '__main__':
    main()