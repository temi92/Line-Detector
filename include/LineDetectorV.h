#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
using namespace cv;
using namespace std;

/**
 * This class performs detection of points on a vertical line using a sliding window approach
 * The algorithm is as follows:
 * 1. Calculate histogram of the image 
 * 2. Determine starting points of the lines by inspecting histogram peaks
 * 3. A sliding window is moved along the Y axis to search for non zero pixels.
 * 4. Extract non zero pixels in the current window
 * 5. If the current window has more non-zero pixels than a specified minimum threshold
 *    recenter window based on the mean position
 * 6. Repeat the steps above.
 **/

// data structure to hold X and Y pixel positions for all the lines detected
using LineInfo = vector<pair<string, pair<vector<int>, vector<int>>>>;

class LineDetectorV{
public:
    LineDetectorV(const Mat& image, int minpix, int margin, int nwindows);
    virtual void run(bool save_images);
    virtual LineInfo getLines() const;
    virtual ~LineDetectorV() = default;
private:
    vector<int> computeHistogram();
    void findPeaks();
    vector<int> starting_points; //starting points to start for line search
    pair<vector<int>, vector<int>> lineFinder(int index);
protected:
    virtual void labelLines();
    vector<int> indxToPixel(const vector<int>& pixels, const vector<vector<int>>& indx);
    void computeNonZeroPoints(); //compute x and y positions of all non zero pixels in image
    void colorLinePixels(const vector<int> x, const vector<int>y, int blue, int green, int red);
    vector<int> computeNonZeroPoints(int y_low, int y_high, int x_low, int x_high); // identify non zero pixels in x and y within window
    Mat image;
    Mat out_img; // img containing lines and detected rectangles.
    int window_size;
    int nwindows;
    int margin;
    int minpix;
    vector<int> nonZeroY; //vector to hold y pixel position of nonZero points
	vector<int> nonZeroX; //vector to hold x pixel position of nonZero points
    LineInfo info;
};