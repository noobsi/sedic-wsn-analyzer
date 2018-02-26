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
    x_ = -1;
    y_ = -1;
  }

  Point(double x, double y) {
    x_ = x;
    y_ = y;
  }

  inline double x() const {
    return x_;
  }

  bool isUnspecified() {
    return x_ == -1;
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


struct Line {
    double a_;
    double b_;
    double c_;
};

class G {
  public:
    static Line line(Point p1, Point p2);
    static Line line(Point *p1, Point *p2) {
      return line(*p1, *p2);
    }
    static void line(double x1, double y1, double x2, double y2, double &a, double &b,
        double &c);
    static double distance(Point p1, Point p2);
    static double distance(double x1, double y1, double x2, double y2);
    static double norm(double rad) {
    	while (rad < 0) {
    		rad = rad + (2 * M_PI);
    	}
    	while (rad > (2 * M_PI)) {
    		rad = rad - (2 * M_PI);
    	}
    	return rad;
    }
    static bool is_intersect(Point p1, Point p2, Point p3, Point p4){
      return is_intersect(&p1, &p2, &p3, &p4);
    }
    static bool intersection(Line l1, Line l2, Point &p);
    static bool is_intersect(Point* p1, Point* p2, Point* p3, Point* p4);
    static bool intersection(double a1, double b1, double c1, double a2, double b2,
            double c2, double &x, double &y);
    static bool intersection(Point* p1, Point* p2, Point* p3, Point* p4, Point& p);
    static bool intersection(Point p1, Point p2, Point p3, Point p4, Point& p) {
      return intersection(&p1, &p2, &p3, &p4, p);
    };
};


#endif /* GEOMETRY_GEOMATHHELPER_H_ */