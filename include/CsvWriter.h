#pragma once
#include "LineDetectorV.h"
#include <vector>
#include <fstream>
using namespace std;


/*  This class saves the X, Y points computed from the Line detector classes to
    a csv file
     
**/
class CsvWriter{
public:
    CsvWriter(const LineInfo&, const LineInfo&); //grabs horizontal and vertical line info
    bool save(const string&filename);
private:
    void writeToString(const LineInfo&, ofstream&of);
    string pointsToString(const vector<int>&x, const vector<int>& y);
    LineInfo info_h;
    LineInfo info_v;
    ofstream out_file;
    
    
};