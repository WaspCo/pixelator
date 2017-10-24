/* Give a 8bits style to image and video
 * Use of bilateral filtering, pixel averaging and color downsampling
 * First version -> 221017
 * Last version -> ?
 * Victor Deleau
 */

#ifndef PROCESSING_H
#define PROCESSING_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;


int colorDownsampling(Mat& image_bilateral_blur, Mat& image_256, float& rast);

int pixelate(Mat& image, Mat& image_pixelated, unsigned int& psize);


#endif
