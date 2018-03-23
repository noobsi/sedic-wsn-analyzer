#ifndef UTIL_H__
#define UTIL_H__

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

class Util {
public:
  static string intVectorToString(vector<int> xs) {
    stringstream res;
    copy(xs.begin(), xs.end(), ostream_iterator<int>(res, " "));

    return res.str();
  }
};



#endif
