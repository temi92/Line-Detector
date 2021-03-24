#include "CsvWriter.h"

CsvWriter::CsvWriter(const LineInfo& h, const LineInfo& v):info_h{h}, info_v{v}
{

}

void CsvWriter::writeToString(const LineInfo& info, ofstream& of)
{
    
   for (size_t i=0; i < info.size(); i++)
   {
        //get label and points ..
        string label = info[i].first;
        string points = pointsToString(info[i].second.first, info[i].second.second) + "\n";
        of << label << ", " << points << endl;
   }
   
}

string CsvWriter::pointsToString(const vector<int>& x, const vector<int>& y){

    string points;
    for (size_t i =0; i < x.size(); i++){
        points += "(" + to_string(x[i]) + ":"+ to_string(y[i]) + "),";

    }
    return points;

}
bool CsvWriter::save(const string& filename)
{
    ofstream out_file{filename};
    string header = "Line name,  Point (x:y)\n";
    out_file << header;
    if (!out_file){
        return false;
    }
    writeToString(info_h, out_file);
    writeToString(info_v, out_file);
    out_file.close();
    return true;

}