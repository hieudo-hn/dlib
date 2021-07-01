// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This example shows how to run a CNN based dog face detector using dlib.  The
    example loads a pretrained model and uses it to find dog faces in images.
    We also use the dlib::shape_predictor to find the location of the eyes and
    nose and then draw glasses and a mustache onto each dog found :)

    Users who are just learning about dlib's deep learning API should read the
    dnn_introduction_ex.cpp and dnn_introduction2_ex.cpp examples to learn how
    the API works.  For an introduction to the object detection method you
    should read dnn_mmod_ex.cpp

    TRAINING THE MODEL
        Finally, users interested in how the dog face detector was trained should
        read the dnn_mmod_ex.cpp example program.  It should be noted that the
        dog face detector used in this example uses a bigger training dataset and
        larger CNN architecture than what is shown in dnn_mmod_ex.cpp, but
        otherwise training is the same.  If you compare the net_type statements
        in this file and dnn_mmod_ex.cpp you will see that they are very similar
        except that the number of parameters has been increased.
        Additionally, the following training parameters were different during
        training: The following lines in dnn_mmod_ex.cpp were changed from
            mmod_options options(face_boxes_train, 40,40);
            trainer.set_iterations_without_progress_threshold(300);
        to the following when training the model used in this example:
            mmod_options options(face_boxes_train, 80,80);
            trainer.set_iterations_without_progress_threshold(8000);
        Also, the random_cropper was left at its default settings,  So we didn't
        call these functions:
            cropper.set_chip_dims(200, 200);
            cropper.set_min_object_size(40,40);
        The training data used to create the model is also available at
        http://dlib.net/files/data/CU_dogs_fully_labeled.tar.gz
        Lastly, the shape_predictor was trained with default settings except we
        used the following non-default settings: cascade depth=20, tree
        depth=5, padding=0.2
*/

#include <string>
#include <iostream>
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>

#include <dirent.h>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <libgen.h>
#include <string.h>

using namespace std;
using namespace dlib;

// ----------------------------------------------------------------------------------------

template <long num_filters, typename SUBNET> using con5d = con<num_filters, 5, 5, 2, 2, SUBNET>;
template <long num_filters, typename SUBNET> using con5 = con<num_filters, 5, 5, 1, 1, SUBNET>;

template <typename SUBNET> using downsampler = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16, SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5 = relu<affine<con5<45, SUBNET>>>;

using net_type = loss_mmod<con<1, 9, 9, 1, 1, rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;

// ----------------------------------------------------------------------------------------

const unsigned CHIP_SIZE = 112;
const unsigned UPSCALE = 2;
const double ADJUST_THRESHOLD = 1; 

// ----------------------------------------------------------------------------------------

void configRead(std::string *);
void executeChipping(const std::string, const std::string);

int main(int argc, char **argv)
{
  std::string configargs[3];
  std::string xmlFile, model;

  //if you run system call ./program XMLFILE MODEL
  if (argc == 3)
  {
    model = std::string(argv[2]);
    xmlFile = std::string(argv[1]);
  }
  //if you run system call ./program
  else if (argc == 1)
  {
    std::cout << "Reading from config" << std::endl;
    configRead(configargs);
    model = configargs[1];
    xmlFile = configargs[0];
  }
  else
  {
    std::cout << "Run ./program to read from config\nOr ./program YOURXMLFILE.xml YOURMODEL.dat" << std::endl;
  }

  executeChipping(xmlFile, model);

  return 0;
}

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

/**
 * Main function to execute chipping. It takes an xmlFile and chips all the photos in that file using a specified model
 */ 
void executeChipping(const std::string xmlFile, const std::string model){
    // load the xmlFile
    dlib::image_dataset_metadata::dataset metadata; 
    load_image_dataset_metadata(metadata, xmlFile); 

    // load the models
    net_type detector;
    deserialize(model) >> detector;

    int num_chips = 0;

    //go through and chip each image
    for (int i = 0; i < metadata.images.size(); i++)
    {
        string curImageFile = metadata.images[i].filename;
        matrix<rgb_pixel> img;
        try {
            load_image(img, curImageFile);
        } catch (image_load_error& e) {
            cout << "Error loading image " << curImageFile << endl; 
        }
        bool isPyramidUp;

        // dets contains the chipped boxes
        std::vector<dlib::mmod_rect> dets;
        resizable_tensor temp;

        try {
            cout << "try pyramid up " << curImageFile << endl; //upscaling image
            pyramid_up(img, pyramid_down<UPSCALE>());

            // use the detector to chip the image
            detector.to_tensor(&img, &img+1, temp);
            detector.subnet().forward(temp);
            detector.loss_details().to_label(temp, detector.subnet(), &dets, ADJUST_THRESHOLD);
            isPyramidUp = true;
        }
        catch(std::bad_alloc& ba) { //Catches bad alllocation (too big)
            cout << "bad alloc pyramid up" << endl;
            try {
                cout << "try pyramid down" << endl;
                load_image(img, curImageFile); //Reload image, smaller
                detector.to_tensor(&img, &img+1, temp);
                detector.subnet().forward(temp);
                detector.loss_details().to_label(temp, detector.subnet(), &dets, ADJUST_THRESHOLD);
                isPyramidUp = false;
            }
            catch(std::bad_alloc& ba) {
                cout << "bad alloc pyramid down, skipping" << endl;
            }
        }

        for (auto&& d : dets)
        {
            num_chips++;
            cout << "num_chips " << num_chips << endl;
            matrix<rgb_pixel> face_chip;
            cout << "d.rect... " << endl;
            chip_details face_chip_details = chip_details(d.rect, chip_dims(CHIP_SIZE, CHIP_SIZE)); //Optionally add angle
            int upscale = (isPyramidUp) ? UPSCALE : 1;
            int left = face_chip_details.rect.left() / upscale, top = face_chip_details.rect.top() / upscale;
            int right = face_chip_details.rect.right() / upscale, bottom = face_chip_details.rect.bottom() / upscale;
            
            //insert the box
            const rectangle rect(left, top, right, bottom);
            const dlib::image_dataset_metadata::box b(rect);
            metadata.images[i].boxes.push_back(b);
        }
    }
    save_image_dataset_metadata(metadata, xmlFile);
    cout << "Done Chipping" << endl;

}
