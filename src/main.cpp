#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <numeric>
#include <opencv2/opencv.hpp>
#include "ImgProcessor.h"
#include "LineDetectorV.h"
#include "LineDetectorH.h"
#include "CsvWriter.h"
using namespace std;
using namespace cv;

using BYTE = unsigned char;

//declaring function prototypes
std::unique_ptr<BYTE[]> readByteFile(std::string filename, int imageWidth, int imageHeight);
Mat ByteImageConverter(std::unique_ptr<BYTE[]> buffer, int imageWidth, int imageHeight);

void help (const char **argv){
	cout << "Invalid number of parameters supplied\n"
		<< "Call:\n" 
	   << argv[0] << " <image_path> <width> <height>" << endl; 
}
int main(int argc, const char*argv[])
{	
	if (argc!=4){
		help(argv);
		return -1;
	}	
	
	const string filename = argv[1];
	const int imageWidth = atoi(argv[2]);
	const int imageHeight = atoi(argv[3]);
	const string out_file = "../tennis_lines.csv";
	//string filename  = "image.raw";
	//const int imageWidth = 1392;
	//const int imageHeight = 550;
	auto buffer = readByteFile(filename, imageWidth, imageHeight);
	Mat image = ByteImageConverter(std::move(buffer), imageWidth, imageHeight);
	//imwrite("../raw_img.png", image);
	Mat kernelX = Mat::ones(1, 9, CV_8UC1);
	Mat kernelY = Mat::ones(9, 1, CV_8UC1);

	ImgProcessor proc = ImgProcessor(image,kernelX, kernelY, 150, 255);
	bool save_images = true;
	proc.run(save_images);
	Mat maskedImgH = proc.getMaskedImages().first;
	Mat maskedImgV = proc.getMaskedImages().second;

	int minpixels = 50;
	int margin = maskedImgH.cols;
	int nwindows = 9;
	//detect horizontal line points in images
	LineDetectorH h = LineDetectorH(maskedImgH, minpixels, margin, nwindows);
	h.run(save_images);
	LineInfo info_h = h.getLines();
	
	margin =80;
	//detect vertical line points in images..
	LineDetectorV v = LineDetectorV(maskedImgV, minpixels, margin, nwindows);
	v.run(save_images);
	LineInfo info_v = v.getLines();
	//log data points to csv file
	CsvWriter csv = CsvWriter(info_h, info_v);
	if (!csv.save(out_file)){
		cout << "error saving: " << out_file  << endl;
		return -1;
 	}
	
	cout << "saved " << out_file  << endl;
	return 0;

	

}

std::unique_ptr<BYTE[]> readByteFile(std::string filename, int imageWidth, int imageHeight)
{

	std::unique_ptr<BYTE[]> buffer (new BYTE[imageWidth*imageHeight]);

	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp){

		fread(buffer.get(), imageWidth*imageHeight, 1, fp);
		fclose(fp);
	}

	return buffer;
}


Mat ByteImageConverter(std::unique_ptr<BYTE[]> buffer, int imageWidth, int imageHeight){

	Mat image(imageHeight, imageWidth, CV_8UC1);
	for(int x =0; x < image.cols; x++){
		for (int y=0; y < image.rows; y++){
			uchar byte = (uchar) buffer[y *image.cols+x];
			image.at<uchar>(y,x) = byte;
		}
	}
	return image;

}
