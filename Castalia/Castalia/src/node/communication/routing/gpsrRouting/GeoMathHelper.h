#ifndef GEOMETRY_GEOMATHHELPER_H_
#define GEOMETRY_GEOMATHHELPER_H_

#include <float.h>
#include <math.h>
#include <sys/types.h>
#include <limits>
#include <vector>

#define g_min(x,y) (((x)<(y))?(x):(y))
#define g_max(x,y) (((x)>(y))?(x):(y))
#define in_range(x,a,y) ((x) <= (a) && (a) <= (y)) || ((x) >= (a) && (a) >= (y))

#define EPSILON 0.000001

#ifndef NaN
#define NaN std::numeric_limits<double>::quiet_NaN()
#endif
typedef double Angle;
class G;

struct Point {
  Point() {
  }

  Point(double x, double y) {
    x_ = x;
    y_ = y;
  }

  inline double x() const {
    return x_;
  }

  inline double y() const {
    return y_;
  }

  double x_;
  double y_;

  inline bool operator==(const Point& rhs) {
    return x_ == rhs.x_ && y_ == rhs.y_;
  }
  inline bool operator!=(const Point& rhs) {
    return !operator==(rhs);
  }
  inline bool operator<(const Point& rhs) const {
    return x_ < rhs.x_ || (x_ == rhs.x_ && y_ < rhs.y_);
  }
};

struct Vector {
  double a_;
  double b_;
};

class G {
  public:
    static double distance(Point p1, Point p2);
    static double distance(double x1, double y1, double x2, double y2);
};


#endif /* GEOMETRY_GEOMATHHELPER_H_ */