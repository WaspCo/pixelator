/* Give a 8bits style to image and video
 * Use of bilateral filtering, pixel averaging and color downsampling
 * First version -> 221017
 * Last version -> ?
 * Victor Deleau
 */

/*
 * reset && g++ -Wall pixelator.cpp -o pixelator `pkg-config --cflags --libs opencv` && ./pixelator P1030305.JPG
 */

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

//#include <QApplication>

#include "include/processing.h"
#include "include/gui.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

    ////////////////////////////////////////////////////////////////////////////
    // Input and info //////////////////////////////////////////////////////////

    char* imageName = argv[1];
    unsigned int psize;
    float rast;

    cout << "\n------------------------------\n---------- Pixelator ---------\n------------------------------\n\n";

    Mat image;//Matrice de l'image
    image = imread( imageName );
    IplImage *image_ipl = cvLoadImage( imageName );

    if( argc == 1 || !image.data )
    {
      printf( "Error -> No image data specified\n " );
      return -1;
    }
    if( argc > 2 ){ rast = strtol(argv[2], NULL, 10);} else { rast = 16; }// auto rast
    if( argc > 3 ){ psize = strtol(argv[3], NULL, 10);} else { psize = 16; }// auto psize

    cout << left << setfill(' ') <<  setw(20) << "Image filename: " << imageName << "\n" << endl;

    cout << setw(20) << "Width:" <<  image_ipl->width << " pixels" << endl;
    cout << setw(20) << "Height:" <<  image_ipl->height << " pixels" << endl;

    cout << setw(20) << "Pixel Depth:" <<  image_ipl->depth << " bits" << endl;
    cout << setw(20) << "Channels:" <<  image_ipl->nChannels << endl;

    cout << setw(20) << "Width Step:" <<  image_ipl->widthStep << endl;
    cout << setw(20) << "Image Size:" <<  image_ipl->imageSize << " octets" << endl;


    ////////////////////////////////////////////////////////////////////////////
    // Processing //////////////////////////////////////////////////////////////

    Mat image_bilateral_blur = image.clone();
    Mat image_pixelated = image.clone();
    Mat image_256 = image.clone();
    int b=1, c=1, p=1;

    if(b == 1){
      bilateralFilter(image, image_bilateral_blur, 5, 75, 75);
    }else{}

    if(c == 1){
      colorDownsampling(image_bilateral_blur, image_256, rast);
    }else{}

    if(p == 1){
      pixelate(image_256, image_pixelated, psize);
    }else{}

    ////////////////////////////////////////////////////////////////////////////
    // Export and clean ////////////////////////////////////////////////////////

    string s = "pixelator_";
    string output_name = s + basename(imageName);//extract filename
    imwrite( output_name , image_pixelated );//write new image

    cout << "\n";

    /*QApplication app(argc, argv);
    return app.exec();*/

    return 0;
}
