#pragma once 
#include "LineDetectorV.h"

/**
 * This class performs detections of points on a horizontal line. It inherits from LineDectorV to 
 * reuse code functionality
 * The algorithm here is as follows:
 * 1. A sliding window approach is also used. 
 * 2. The width of each window span across the entire width of the image to
 *   allow horizontal lines of any length to be detected
 * 3. For each window, Non Zero pixels are computed, if the non zero pixels greater than a specified threshold,
 *    a line has been found
 * 4. Repeat steps above for the entire image height
 * */
class LineDetectorH:public LineDetectorV{

public:
    LineDetectorH(const Mat&image, int min_pixels, int margin, int n_windows);
    virtual void run(bool save_images) override;
    virtual ~LineDetectorH() = default;

private:
    void lineFinder();
    vector<int> indxToPixel(const vector<int>& pixels, const vector<int>& indx);
    vector<pair<vector<int>, vector<int>>> total_lines;   
    virtual void labelLines() override;
    int delta = 50; 
    

};