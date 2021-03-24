#include "LineDetectorV.h"


LineDetectorV::LineDetectorV(const Mat& image, int minpix, int margin, int nwindows){

    this->image = image;   
    this-> minpix = minpix;
    this->margin = margin;
    this-> nwindows = nwindows;
    this->window_size = image.rows/nwindows; 
  
    out_img = image.clone();
    cvtColor(out_img, out_img, COLOR_GRAY2BGR);	
  
}

vector<int> LineDetectorV::computeHistogram(){

    vector<int> histogram;
    for (int i =0; i < image.cols; i++){
        //grab each column image and compute the sum
        Mat col = image.col(i);
        int sum = cv::sum(col)[0];
        histogram.push_back(sum);
    }
    return histogram;

}

LineInfo LineDetectorV::getLines()const {
    return info;
}
//store x and y pixels values for image with non zero points
void LineDetectorV::computeNonZeroPoints(){
    for (unsigned int y=0; y < image.rows; y++){
		for (unsigned int x=0; x < image.cols; x++){
			if (image.at<uchar>(y,x) > 0){
				
				nonZeroY.push_back(y);
				nonZeroX.push_back(x);
			}
		}
	}
}

vector<int> LineDetectorV::computeNonZeroPoints(int win_y_low, int win_y_high, int win_x_low, int win_x_high){

    vector<int> good_inds;
    for (unsigned int i =0; i < nonZeroY.size(); i++){

			if((nonZeroY[i]>=win_y_low) && (nonZeroY[i]< win_y_high ) && (nonZeroX[i]>=win_x_low) 
				&& (nonZeroX[i] < win_x_high))
                {
					good_inds.push_back(i);

				}
		}
    return good_inds;
}

void LineDetectorV::run(bool save_images){

    computeNonZeroPoints();
    findPeaks();
    labelLines();

    if (save_images){
        for (unsigned int i=0; i < starting_points.size(); i++){
            auto line_pixels = lineFinder(starting_points[i]);
            vector<int> line_pixelsX = line_pixels.first;
            vector<int> line_pixelsY = line_pixels.second;     
            colorLinePixels(line_pixelsX, line_pixelsY, 0, 0, 255);

        }
        imwrite("../vertical.png", out_img);
        //imshow("out_img", out_img);
	    //waitKey(0);
	    //destroyAllWindows();
    
    }     
	
}

void LineDetectorV::labelLines(){
    vector<string> line_names{"single sideline", "centre service line","single sideline", "sideline"};
    
    //elements in indxs vector are added in a sorted order from left to right 
    for (unsigned int i=0; i < starting_points.size(); i++){
        
        vector<int> line_pixelsX = lineFinder(starting_points[i]).first;
        vector<int> line_pixelsY = lineFinder(starting_points[i]).second;
         auto lines = make_pair(line_pixelsX, line_pixelsY);

        info.emplace_back(line_names[i], lines);
        
    }

}
void LineDetectorV::findPeaks()
{
    //find peaks of the left and right halfs of the histogram.
    //This should be the starting point of the left and right lines where we begin to search for lines.
    vector<int> histogram = computeHistogram();
    int midpoint = histogram.size()/2;

    //split histogram into two sub regions ...
    vector<int> sub_hist_left(histogram.begin(), histogram.begin()+ midpoint);
    vector<int> sub_hist_right(histogram.begin()+midpoint, histogram.end());

    vector<vector<int>> regions{sub_hist_left, sub_hist_right};
    vector<int> sub_regions;
    
    for (unsigned int i = 0; i < 4; i++)
    {
        if (i < 2)
            sub_regions = regions[i];
           
        //find maximum index in each histogram region this would be starting for search
        int indx=  distance(sub_regions.begin(),max_element(sub_regions.begin(), sub_regions.end()));
        //
        if (i == 2){
            indx+=midpoint; // get to the beginning of 3rd line to continue line seacrch
        }
        else if (i==3){
            indx+= (midpoint+120); // gets to the beginning of 4th line to continue line search
        }

        starting_points.push_back(indx);
    }

}

pair<vector<int>, vector<int>> LineDetectorV::lineFinder(int index){
   
    int x_current  = index;	
    vector<vector<int>> line_indxs; //non zero index points for all the boxes
    for (unsigned int window=0; window < nwindows; window++){

        int win_y_low = image.rows - (window+1)*window_size;
		int win_y_high = image.rows - window *window_size;

        int win_x_low = x_current - margin;
		int win_x_high = x_current + margin; 
        rectangle(out_img,Point(win_x_low,win_y_low),Point(win_x_high,win_y_high),Scalar(255,0,0), 1, 8, 0);

        //identify non zero pixels indices in x and y within a window
        vector<int> good_indx = computeNonZeroPoints(win_y_low, win_y_high, win_x_low, win_x_high);
        line_indxs.push_back(good_indx);
        //if non zero pixels in a window > min pixels recenter next window based on mean position
        if (good_indx.size()>minpix){
            vector<int> res;
            for (unsigned int i=0; i < good_indx.size();i++){
                    res.push_back(nonZeroX[good_indx[i]]);
                }
            int average = std::accumulate(res.begin(), res.end(), 0.0)/res.size();
			x_current = average;

        }
    }
    //compute X and Y pixel position..
    vector<int> line_pixelsX = indxToPixel(nonZeroX, line_indxs);
    vector<int> line_pixelsY = indxToPixel(nonZeroY, line_indxs);
    return make_pair(line_pixelsX, line_pixelsY);

}

void LineDetectorV::colorLinePixels(const vector<int> x_pixels, const vector<int>y_pixels, int blue, int green, int red)
{
    //color pixels..
	for (unsigned int i = 0; i < y_pixels.size();i++)
	{
		int y = y_pixels[i];
		int x = x_pixels[i];
		out_img.at<Vec3b>(y,x)[0] = blue;
		out_img.at<Vec3b>(y,x)[1] = green;
		out_img.at<Vec3b>(y,x)[2] = red;

	}

}

vector<int> LineDetectorV::indxToPixel(const vector<int>& pixels, const vector<vector<int>>& line_indxs)
{
    vector<int> line;
    for (unsigned int i=0; i < line_indxs.size(); i++)
    {
		for (unsigned int j=0; j <  line_indxs[i].size(); j++)
		{
			line.push_back(pixels[line_indxs[i][j]]);
		}

	}
    return line;
}
