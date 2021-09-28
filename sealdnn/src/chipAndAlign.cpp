#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <dirent.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/array2d.h>
#include <dlib/pixel.h>
#include <dlib/image_transforms.h>
#include <dlib/image_processing.h>

#include <math.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace dlib;

#define PI 3.14159265

const double DESIRED_LEFTEYE[2] = {0.25, 0.28};

/**
 * Read configuration from config.txt file
 */
void configRead(std::string *config)
{
  std::string line;
  std::ifstream configfile("config.txt");
  int counter = 0;
  if (configfile.is_open())
  {
    while (getline(configfile, line))
    {
      std::string arg = line.substr(line.find('=') + 1);
      config[counter] = arg;
      counter++;
    }
    configfile.close();
  }

  else
    std::cout << "Unable to open file" << std::endl;
}

cv::Mat align(
    dlib::image_dataset_metadata::image i,
    dlib::image_dataset_metadata::box b)
{
    const int width = b.rect.right() - b.rect.left();
    const int height = b.rect.bottom() - b.rect.top();

    cv::Mat img = cv::imread(i.filename, cv::IMREAD_COLOR);

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

    // double dist = sqrt((dX * dX) + (dY * dY));
    // double desiredDist = (DESIRED_RIGHTEYE[0] - DESIRED_LEFTEYE[0]);
    // desiredDist *= CHIP_SIZE;
    // double scale = desiredDist / dist;

    double scale = 1;

    // compute center (x, y)-coordinates (i.e., the median point)
    // between the two eyes in the input image
    cv::Point2d eyesCenter((reye.x() + leye.x()) / 2, (reye.y() + leye.y()) / 2);
    cv::Mat M = getRotationMatrix2D(eyesCenter, angle, scale); // M is the rotational matrix

    // update the translation component of the matrix
    double tX = width * 0.5;
    double tY = height * DESIRED_LEFTEYE[1];
    M.at<double>(0, 2) += (tX - eyesCenter.x);
    M.at<double>(1, 2) += (tY - eyesCenter.y);

    // apply the affine transformation
    cv::Mat output;
    cv::warpAffine(img, output, M, cv::Size(width, height));

    return output;
}

// ----------------------------------------------------------------------------------------

/**
 * Chip (i.e export) the images and store them in corresponding folders 
 * The name of the chipped images will contain relevant information regarding the seals
 */
int main(int argc, char **argv)
{
    using namespace dlib::image_dataset_metadata;
   
    std::string configargs[3];    
    std::cout << "Reading from config" << std::endl;
    configRead(configargs);
    std::string xmlFile = configargs[0];
    
    dlib::image_dataset_metadata::dataset metadata;
    load_image_dataset_metadata(metadata, xmlFile);
	
    // for each image
    for (image i : metadata.images)
    {
        matrix<rgb_pixel> img;
        dlib::load_image(img, i.filename);
        // for each box in the image
        for (box b : i.boxes)
        {
            if (b.ignore)
                continue;
            matrix<rgb_pixel> face_chip;
            chip_details face_chip_details = chip_details(b.rect);
            extract_image_chip(img, face_chip_details, face_chip); //Img, rectangle for each chip, chip destination

            //Get the directory and remove the .jpg part of the curImg
            string temp = i.filename;
            int slashIdx = 0;
            for (int t = i.filename.size() - 1; t >= 0; t--)
            {
                if (temp.at(t) == '/')
                {
                    slashIdx = t;
                    break;
                }
            }
            temp.erase(0, slashIdx + 1);
            int dotIdx = temp.find(".");
            string curImg;
            if (dotIdx != std::string::npos)
                curImg = temp.substr(0, dotIdx);

            // Make the Chip directory = PATH + "Chips"
            //creating chipfolder with name = PATH + Chips
            string chipFolder = i.filename.substr(0, slashIdx) + "Chips";
            struct stat st = {0};
            if (stat(chipFolder.c_str(), &st) == -1)
            {
                if (mkdir(chipFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
                    std::cout << "Error creating directory. Exiting..." << std::endl;
                else
                    std::cout << "Directory created. Chipped photos will be stored at " << chipFolder << std::endl;
            }

            // save the face chip
            // the name of the chipped photo will be in the format:
            // <count>_<original_photo>_ChippedAt_<top_left_coordinate_of_the_chipped_photo_within_the_original_photo>_<bottom_right_coordinate_of_the_chipped_photo_within_the_original_photo>.jpeg
            // located in the folder <original_folder>Chips
            int left = face_chip_details.rect.left(), top = face_chip_details.rect.top();
            int right = face_chip_details.rect.right(), bottom = face_chip_details.rect.bottom();

            string location = to_string(left) + "_" + to_string(top) + "_" + to_string(right) + "_" + to_string(bottom);
            string filename = curImg + "_ChippedAt_" + location + ".jpeg";
            string filedir = chipFolder + "/" + filename;

            // only align boxes that have locations of the eyes
            if (b.parts.size() == 0)
                save_jpeg(face_chip, filedir, 100);
            else
                cv::imwrite(filedir, align(i, b)); // align and export
        }
    }

    std::cout << "Done Chipping." << std::endl;
}

