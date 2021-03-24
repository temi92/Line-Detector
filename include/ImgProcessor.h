#pragma once
#include <utility>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

/**
 * This class is responsible for generating masked images from the input image
 * The algorithm is as follows:
 * 1. Thresholding is appled to the image, This is done by setting pixels 
 *    values below a certain threshold to zero
 * 2. Peforming eroision to eliminate small white blobs present in the images.
 * 3. For each pixel we inspect neigbouring pixels. The number of neighbours 
 *    is determined by the size of kernel.
 * 4. If kernel is horizontal , inspect West and East neigbouring pixels.
   5. If kernel is vertical, inspect North and South neigbouring pixels.
   6. If any of the neigbouring values are 0, we set the centre pixel to be 0.
   7. Repeats steps above.
   8. Output is a set of clean Image masks with horizontal and vertical lines segmented
**/

enum class ImageType{
    horizontal, 
    vertical
};

class ImgProcessor{

public:
    ImgProcessor(const Mat& img, const Mat& kernelX, const Mat& kernelY,
                int threshold, int maxVal);             
    void run(bool save_images);
    std::pair<Mat,Mat> getMaskedImages() const;
    ~ImgProcessor() = default;
private:
    void thresholdImage();
    void cleanBorder(int col);
    Mat erodeImage(const Mat& image, const Mat& kernel, const ImageType& type);
    bool checkWest(MatConstIterator_<uchar> ptr, int anchorX);
    bool checkEast(MatConstIterator_<uchar> ptr, int anchorX);
    bool checkNorth(MatConstIterator_<uchar> ptr, int width, int anchorY);
    bool checkSouth(MatConstIterator_<uchar> ptr, int width, int anchorY);
    Mat kernelX;
    Mat kernelY;
    int threshold;
    int maxVal;
    Mat img;
    std::pair<Mat, Mat> maskedImages;
    
    
};

