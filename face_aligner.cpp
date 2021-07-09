// include
#include <iostream>
#include <string> 
#include <math.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <dlib/data_io.h>
#include <dlib/image_processing.h>

#define PI 3.14159265

const int CHIP_SIZE = 112;
const double DESIRED_LEFTEYE[2] = {0.3, 0.3};

using namespace dlib::image_dataset_metadata;
using namespace cv;
using namespace std;

void align(std::string face_file)
{
    // load the data
    dataset metadata;
    load_image_dataset_metadata(metadata, face_file);

    // for each image
    for (int i = 0; i < metadata.images.size(); i++)
    {
        // load the image;
        int count = 0;
        string curImageFile = metadata.images[i].filename;
        Mat img = imread(curImageFile, IMREAD_COLOR);

        if (img.empty())
        {
            std::cout << "Could not read the image: " << curImageFile << std::endl;
            return;
        }

        for (int j = 0; j < metadata.images[i].boxes.size(); j++)
        {
            box b = metadata.images[i].boxes[j];
            if (b.ignore || b.parts.size() == 0)
                continue;

            // 0,1,2,3 corresponds to the center of left eye, right eye, nose, mouth
            dlib::point leye = b.parts["0"];
            dlib::point reye = b.parts["1"];
            dlib::point nose = b.parts["2"];
            dlib::point mouth = b.parts["3"];

            // compute the angle between the eye centroids
            double dY = reye.y() - leye.y();
            double dX = reye.x() - leye.x();
            double angle = atan(dY / dX) * 180 / PI;

            // compute the desired right eye x-coordinate based on the
            // desired x-coordinate of the left eye
            const double DESIRED_RIGHTEYE[2] = {1.0 - DESIRED_LEFTEYE[0], 1.0 - DESIRED_LEFTEYE[1]};
            // determine the scale of the new resulting image by taking
            // the ratio of the distance between eyes in the *current*
            // image to the ratio of distance between eyes in the
            // *desired* image
            double dist = sqrt((dX * dX) + (dY * dY));
            double desiredDist = (DESIRED_RIGHTEYE[0] - DESIRED_LEFTEYE[0]);
            desiredDist *= CHIP_SIZE;
            double scale = desiredDist / dist;

            // compute center (x, y)-coordinates (i.e., the median point)
            // between the two eyes in the input image
            Point2d eyesCenter((reye.x() + leye.x()) / 2, (reye.y() + leye.y()) / 2);
            Mat M = getRotationMatrix2D(eyesCenter, angle, scale); // M is the rotational matrix

            // update the translation component of the matrix
            double tX = CHIP_SIZE * 0.5;
            double tY = CHIP_SIZE * DESIRED_LEFTEYE[1];
            M.at<double>(0, 2) += (tX - eyesCenter.x);
            M.at<double>(1, 2) += (tY - eyesCenter.y);

            // apply the affine transformation
            Mat output;
            warpAffine(img, output, M, cv::Size(CHIP_SIZE, CHIP_SIZE));

            // save the photo
            count++;
            cv::imwrite(curImageFile + to_string(count), output);
        }
    }
}

int main(int argc, char** argv){
    if (argc != 2)
    {
        cout << "Give the path to your xml file as an argument to this" << endl;
        cout << "program.  For example, execute this program by running:" << endl;
        cout << "       ./align ./final.xml" << endl;
        cout << endl;
        return 0;

        align(argv[1]);
    }
}
