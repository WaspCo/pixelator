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
    char* imageName = argv[1];
    int rast, psize;

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
    // How it should work //////////////////////////////////////////////////////

    //bilateral_blur();
    //pixelate();
    //color_downsampling();

    ////////////////////////////////////////////////////////////////////////////
    // Apply bilateral bluring filter to the image /////////////////////////////
    Mat image_bilateral_blur = image.clone();
    Mat image_pixelated = image.clone();
    Mat image_256 = image.clone();
    /*bilateralFilter(image, image_bilateral_blur, 10, 75, 75);*/

    int x,y,xx,yy;

    ////////////////////////////////////////////////////////////////////////////
    // Pixelate the image //////////////////////////////////////////////////////
    long blue_mean, green_mean, red_mean;
    int psize2 = psize*psize;
    for(x=0; x<(image.cols-psize); x += psize){
      for(y=0; y<(image.rows-psize); y += psize){

        for(xx=0; xx<psize; xx++){// calculate mean value for the group
          for(yy=0; yy<psize; yy++){
            Vec3b intensity = image.at<Vec3b>(y+yy, x+xx);
            blue_mean += intensity.val[0];//B
            green_mean += intensity.val[1];//G
            red_mean += intensity.val[2];//R
          }
        }
        Vec3b average;
        average.val[0] = floor(blue_mean /= (psize2));
        average.val[1] = floor(green_mean /= (psize2));
        average.val[2] = floor(red_mean /= (psize2));

        for(xx=0; xx<psize; xx++){// write new color for the group
          for(yy=0; yy<psize; yy++){
            image_pixelated.at<Vec3b>(y+yy, x+xx) = average;
          }
        }

      }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Change color possibilities //////////////////////////////////////////////

    for(x=0; x<image.cols; x++){
      for(y=0; y<image.rows; y++){
        Vec3b intensity = image_pixelated.at<Vec3b>(y, x);

        intensity.val[0] = rast * floor(intensity.val[0] / rast);//B
        intensity.val[1] = rast * floor(intensity.val[1] / rast);//G
        intensity.val[2] = rast * floor(intensity.val[2] / rast);//R

        image_256.at<Vec3b>(y, x) = intensity;
      }
    }

    string s = "pixelator_";
    string output_name = s + basename(imageName);//extract filename
    imwrite( output_name , image_256 );//write new image
    //namedWindow( "image_prime" );
    //imshow( "image_prime", image_256 );
    //waitKey(0);
    cout << "\n";

    /*QApplication app(argc, argv);
    return app.exec();*/

    return 0;
}
