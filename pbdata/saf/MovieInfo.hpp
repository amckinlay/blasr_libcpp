#ifndef DATASTRUCTURES_SAF_HDF_MOVIE_INFO_H_
#define DATASTRUCTURES_SAF_HDF_MOVIE_INFO_H_

#include <string>
#include <vector>
#include "../Types.h"

using namespace std;

class MovieInfo
{
public:
    vector<string> name;
    vector<UInt> run;
    vector<UInt> experiment;
    vector<UInt> id;
    int FindMovie(UInt idKey, string &nameVal);
};

#endif
