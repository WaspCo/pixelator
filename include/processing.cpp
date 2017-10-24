/* Give a 8bits style to image and video
 * Use of bilateral filtering, pixel averaging and color downsampling
 * First version -> 221017
 * Last version -> ?
 * Victor Deleau
 */

#include "processing.h"

using namespace cv;
using namespace std;

////////////////////////////////////////////////////////////////////////////
// Pixelate the image //////////////////////////////////////////////////////
unsigned int x, y, xx, yy;
int pixelate(Mat& image, Mat& image_pixelated, unsigned int& psize)
{

    float blue_mean, green_mean, red_mean;
    unsigned int remain_x = image.cols % psize;
    unsigned int remain_y = image.rows % psize;
    cout << "remain_x: " << remain_x << " remain_y: " << remain_y << "\n";

    unsigned int psize2 = psize*psize;
    unsigned int psize_x = psize*remain_x;
    unsigned int psize_y = psize*remain_y;
    unsigned int psize_xy = remain_x*remain_y;

    Vec3b intensity(0,0,0), average(0,0,0);

    for(x=0; x<((uint)image.cols-(psize-1)); x += psize){

      //Main columns processing
      for(y=0; y<=((uint)image.rows-(psize)); y += psize){
          for(xx=0; xx<psize; xx++){// calculate mean value for the group
            for(yy=0; yy<psize; yy++){
              intensity = image.at<Vec3b>(y+yy, x+xx);
              blue_mean += (float)intensity.val[0];//B
              green_mean += (float)intensity.val[1];//G
              red_mean += (float)intensity.val[2];//R
            }
          }
          blue_mean = blue_mean / psize2;
          green_mean = green_mean / psize2;
          red_mean = red_mean / psize2;
          average.val[0] = blue_mean;
          average.val[1] = green_mean;
          average.val[2] = red_mean;

          for(xx=0; xx<psize; xx++){// write new color for the group
            for(yy=0; yy<psize; yy++){
              image_pixelated.at<Vec3b>(y+yy, x+xx) = average;
            }
          }
      }

      //End of main columns, incomplete block processing
      if(remain_y > 0)
      {
          for(xx=0; xx<psize; xx++){// calculate mean value for the group
            for(yy=0; yy<remain_y; yy++){
              intensity = image.at<Vec3b>(y+yy, x+xx);
              blue_mean += (float)intensity.val[0];//B
              green_mean += (float)intensity.val[1];//G
              red_mean += (float)intensity.val[2];//R
            }
          }
          blue_mean = blue_mean / psize_y;
          green_mean = green_mean / psize_y;
          red_mean = red_mean / psize_y;
          average.val[0] = blue_mean;
          average.val[1] = green_mean;
          average.val[2] = red_mean;

          for(xx=0; xx<psize; xx++){// write new color for the group
            for(yy=0; yy<remain_y; yy++){
              image_pixelated.at<Vec3b>(y+yy, x+xx) = average;
            }
          }
      }

    }//End of main columns processing //////////////////////////////////////////

    //Last column incomplete block processing
    if(remain_x > 0)
    {
        for(y=0; y<=((uint)image.rows-psize); y += psize){
          for(xx=0; xx<remain_x; xx++){// calculate mean value for the group
            for(yy=0; yy<psize; yy++){
              intensity = image.at<Vec3b>(y+yy, x+xx);
              blue_mean += (float)intensity.val[0];//B
              green_mean += (float)intensity.val[1];//G
              red_mean += (float)intensity.val[2];//R
            }
          }
          blue_mean = blue_mean / psize_x;
          green_mean = green_mean / psize_x;
          red_mean = red_mean / psize_x;
          average.val[0] = blue_mean;
          average.val[1] = green_mean;
          average.val[2] = red_mean;

          for(xx=0; xx<remain_x; xx++){// write new color for the group
            for(yy=0; yy<psize; yy++){
              image_pixelated.at<Vec3b>(y+yy, x+xx) = average;
            }
          }
        }
    }

    //Last block in the bottom-right corner
    if((remain_y > 0) && (remain_x > 0))
    {
        for(xx=0; xx<remain_x; xx++){// calculate mean value for the group
          for(yy=0; yy<remain_y; yy++){
            intensity = image.at<Vec3b>(y+yy, x+xx);
            blue_mean += (float)intensity.val[0];//B
            green_mean += (float)intensity.val[1];//G
            red_mean += (float)intensity.val[2];//R
          }
        }
        blue_mean = blue_mean / psize_xy;
        green_mean = green_mean / psize_xy;
        red_mean = red_mean / psize_xy;
        average.val[0] = blue_mean;
        average.val[1] = green_mean;
        average.val[2] = red_mean;


        for(xx=0; xx<remain_x; xx++){// write new color for the group
          for(yy=0; yy<remain_y; yy++){
            image_pixelated.at<Vec3b>(y+yy, x+xx) = average;
          }
        }
    }
    return 0;
}




////////////////////////////////////////////////////////////////////////////
// Change color possibilities //////////////////////////////////////////////

int colorDownsampling(Mat& image_bilateral_blur, Mat& image_256, float& rast)
{
    for(x=0; x<(uint)image_bilateral_blur.cols; x++){
      for(y=0; y<(uint)image_bilateral_blur.rows; y++){
        Vec3b intensity = image_bilateral_blur.at<Vec3b>(y, x);
        Vec3b average;

        average.val[0] = rast * floor(intensity.val[0] / rast);//B
        average.val[1] = rast * floor(intensity.val[1] / rast);//G
        average.val[2] = rast * floor(intensity.val[2] / rast);//R

        image_256.at<Vec3b>(y, x) = average;
      }
    }
    return 0;
}
