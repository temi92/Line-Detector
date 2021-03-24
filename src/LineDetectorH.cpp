#include "LineDetectorH.h"

LineDetectorH::LineDetectorH(const Mat&image, int min_pixels, int margin, int n_windows)
:LineDetectorV{image, min_pixels, margin, n_windows}
{


}

void LineDetectorH::run(bool save_images){

    computeNonZeroPoints();
    lineFinder();
    labelLines();
    
    if (save_images){
        for (unsigned int i=0; i < total_lines.size(); i++)
        {
            vector<int> line_pixelsX = total_lines[i].first;
            vector<int> line_pixelsY = total_lines[i].second;
            colorLinePixels(line_pixelsX, line_pixelsY, 0, 0, 255);
        }

        imwrite("../horizontal.png", out_img);
        //imshow("out_img", out_img);
        //waitKey(0);
        //destroyAllWindows();
    }
   
   
    
}

void LineDetectorH::labelLines(){
  
    string name; 
    for (unsigned int i=0; i < total_lines.size(); i++){
        vector<int> line_pixelsY = total_lines[i].second;
        int average = accumulate(line_pixelsY.begin(), line_pixelsY.end(), 0)/line_pixelsY.size();
        if (out_img.rows - average > delta){

            name = "baseline";       
            info.emplace_back(name, total_lines[i]);
        }

        else{
            name = "service line";
            //info.emplace_back(make_pair(name, total_lines));
            info.emplace_back(name, total_lines[i]);
        }

    }

}

 void LineDetectorH::lineFinder(){

    for (unsigned int window=0; window < nwindows; window++){
        
        int win_y_low = image.rows - (window+1)*window_size;
        int win_y_high = image.rows - window *window_size;

        int win_x_low =  0;
		int win_x_high = out_img.cols;

        rectangle(out_img,Point(win_x_low,win_y_low),Point(win_x_high,win_y_high),Scalar(255,0,0), 1, 8, 0);

        //identify non zero pixels indices in x and y within a window
        vector<int> good_indx = computeNonZeroPoints(win_y_low, win_y_high, win_x_low, win_x_high);
        
        //no line detected in box
        if (good_indx.size() == 0)
            continue;
        vector<int> line_pixelsX = indxToPixel(nonZeroX, good_indx);
        vector<int> line_pixelsY = indxToPixel(nonZeroY, good_indx);
        total_lines.push_back(make_pair(line_pixelsX, line_pixelsY));
     
    }

 }


vector<int> LineDetectorH::indxToPixel(const vector<int>& pixels, const vector<int>& line_indxs)
{
    vector<int> line;
    for (unsigned int i=0; i < line_indxs.size(); i++)
    {
		line.push_back(pixels[line_indxs[i]]);
	
	}
    return line;
}
