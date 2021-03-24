#include "ImgProcessor.h"


ImgProcessor::ImgProcessor(const Mat&img, const Mat& kernelX, const Mat& kernelY, 
                        int threshold, int maxVal){
    this->img = img;
    this->threshold = threshold;
    this->maxVal = maxVal;
    this->kernelX = kernelX;
    this->kernelY = kernelY;
}

//thresholding Image -> set value of pixels below a certain threshold to 0 and above to 255.
void ImgProcessor::thresholdImage(){
    MatIterator_<uchar> it;
    int val;  
    for (it = img.begin<uchar>(); it!=img.end<uchar>(); ++it ){
        val = *it >=threshold ? maxVal:0;
        *it = val;

    }
}

//eliminate noise at border of image.
void ImgProcessor::cleanBorder(int col){

    for (unsigned int y=0; y < img.rows; y++){
		for (unsigned int x=0; x < img.cols; x++){
			if (x < col){
				img.at<uchar>(y,x) = 0;

			}
		}
		
	}

}

void ImgProcessor::run(bool save_images){

    cleanBorder(5); // set the the first 5 columns to zero to get rid of the noise at the edge of image..
    thresholdImage();
    
    ImageType img_type{ImageType::horizontal};
    Mat maskedImgH = erodeImage(img, kernelX, img_type);
    img_type = ImageType::vertical;

    Mat maskedImgV = erodeImage(img, kernelY, img_type);
    maskedImages.first = maskedImgH;
    maskedImages.second = maskedImgV;

    if (save_images){
        imwrite ("../MaskedImg.png", img);
        imwrite("../MaskedImgH.png", maskedImgH);
        imwrite("../MaskedImgV.png", maskedImgV);
    }
}

pair<Mat, Mat> ImgProcessor::getMaskedImages() const{
    return maskedImages;

}

bool ImgProcessor::checkWest(MatConstIterator_<uchar> ptr, int anchorX)
{
   for (unsigned int i=1; i <= anchorX; i++){ 
        if (!*(ptr-i)){
            return false;
        }    
    }
    return true;
}

bool ImgProcessor::checkEast(MatConstIterator_<uchar> ptr, int anchorX)
{
    for (unsigned int i=1; i <= anchorX; i++){
        
        if (!*(ptr+i)){
            return false;
        }
    
    }  
    return true;
}
bool ImgProcessor::checkNorth(MatConstIterator_<uchar> ptr, int width, int anchorY){
    for(unsigned int i =1; i <= anchorY; i++){

        if (!*(ptr-i*width)){
            return false;
        }
    }
    return true;
}
bool ImgProcessor::checkSouth(MatConstIterator_<uchar> ptr, int width, int anchorY){
    for(unsigned int i =1; i <= anchorY; i++){
        if (!*(ptr+i*width)){
            return false;
        }
    }
    return true;
}

Mat ImgProcessor::erodeImage(const Mat& image, const Mat & kernel, const ImageType& type){
    
    int anchorY = kernel.rows/2.0;
    int anchorX = kernel.cols/2.0;

    MatConstIterator_<uchar> itr;
    itr = image.begin<uchar>();
    Mat processedImg = image.clone();

    MatIterator_<uchar> out = processedImg.begin<uchar>();
    // avoid starting from 0 to avoid conflict with image boundary ..
    for (unsigned int y=anchorY; y < image.rows-anchorY; y++)
    {
        for (unsigned int x=anchorX; x < image.cols-anchorX; x++)
        {
            switch(type)
            {
                case ImageType::horizontal:

                    if (*itr)
                    {
                        if (!(checkWest(itr, anchorX) && checkEast(itr,anchorX)))
                        {
                            *out = 0;
                        }
                    }
                    break;
                case ImageType::vertical:
                    if (*itr){
                        if (!(checkNorth(itr, image.cols,anchorY)&& checkSouth(itr, image.cols, anchorY)))
                        {
                            *out = 0;
                         }        
                    }       
                    break;
            }
 
            itr++; out++;
        }
    }
    return processedImg;
}
