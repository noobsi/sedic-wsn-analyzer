#ifndef GEOMETRY_GEOMATHHELPER_H_
#define GEOMETRY_GEOMATHHELPER_H_

#include <float.h>
#include <math.h>
#include <sys/types.h>
#include <limits>
#include <stack>
#include <vector>
#include <iostream>

#define g_min(x,y) (((x)<(y))?(x):(y))
#define g_max(x,y) (((x)>(y))?(x):(y))
#define in_range(x,a,y) ((x) <= (a) && (a) <= (y)) || ((x) >= (a) && (a) >= (y))

#define EPSILON 0.000001
#define INVALID -10000

#ifndef NaN
#define NaN std::numeric_limits<double>::quiet_NaN()
#endif

typedef double Angle;
class G;

using namespace std;


struct Point {
  Point() {
    x_ = INVALID;
    y_ = INVALID;
  }

  Point(double x, double y) {
    x_ = x;
    y_ = y;
  }

  inline double x() const {
    return x_;
  }

  bool isUnspecified() {
    return x_ == INVALID;
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

  friend std::ostream& operator<< ( std::ostream& os, const Point& c );

};

struct NeighborRecord {
  int id;      // the node's ID
  Point location;
  NeighborRecord(){}
  NeighborRecord(int id, Point location): id(id), location(location) {}
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
    static void centers(Point p1, Point p2, double radius, Point &center1, Point &center2);
    static vector<Point> centers(Point p1, Point p2, double radius);
    static Point nearestCenterCCW(Point pivot, Point next, Point center, double radius) {
      Point center1, center2;
      G::centers(pivot, next, radius, center1, center2);

      double pivotAngle = norm(atan2(pivot.y() - center1.y(), pivot.x() - center1.x()));
      double nextAngle = norm(atan2(next.y() - center1.y(), next.x() - center1.x()));
      double diffCCWAngle = norm(pivotAngle - nextAngle);

      if (diffCCWAngle < M_PI) {
        return center1;
      } else {
        return center2;
      }
    }
    static double cross(const Point &O, const Point &A, const Point &B) {
    	return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
    }
    static int findNextHopRollingBall(Point pivot, Point ballCenter, double ballRadius, vector<NeighborRecord> candidates, Point &nextCenter) {
      int nextHop = -1;
      double ballCenterAngle = norm(atan2(ballCenter.y() - pivot.y(), ballCenter.x() - pivot.x()));
      double minAngle = 3 * M_PI;
      for (auto &n: candidates) {
        Point candidateCenter = nearestCenter(pivot, n.location, ballCenter, ballRadius);
        double candidateAngle = norm(atan2(candidateCenter.y() - pivot.y(), candidateCenter.x() - pivot.x()));
        if (norm(candidateAngle - ballCenterAngle) < minAngle) {
          minAngle = norm(candidateAngle - ballCenterAngle);
          nextHop = n.id;
          nextCenter = candidateCenter;
        }
      }

      return nextHop;
    }
    static Point findNextHopRollingBall(Point pivot, Point ballCenter, double ballRadius, vector<Point> candidates, Point &nextCenter) {
      Point nextHop;
      double ballCenterAngle = norm(atan2(ballCenter.y() - pivot.y(), ballCenter.x() - pivot.x()));
      double minAngle = 3 * M_PI;
      for (auto &n: candidates) {
        Point candidateCenter = nearestCenter(pivot, n, ballCenter, ballRadius);
        double candidateAngle = norm(atan2(candidateCenter.y() - pivot.y(), candidateCenter.x() - pivot.x()));
        if (norm(candidateAngle - ballCenterAngle) < minAngle) {
          minAngle = norm(candidateAngle - ballCenterAngle);
          nextHop = n;
          nextCenter = candidateCenter;
        }
      }

      return nextHop;
    }

    static Point nearestCenter(Point pivot, Point next, Point center, double ballRadius) {
      Point center1, center2;
      centers(pivot, next, ballRadius, center1, center2);

      double pivotAngle = norm(atan2(pivot.y() - center1.y(), pivot.x() - center1.x()));
      double nextAngle = norm(atan2(next.y() - center1.y(), next.x() - center1.x()));
      double diffCCWAngle = norm(pivotAngle - nextAngle);

      if (diffCCWAngle < M_PI) {
        return center1;
      } else {
        return center2;
      }
    }
    static vector<Point> convexHull1(vector<Point> P) {
    	int n = P.size(), k = 0;
    	if (n <= 3) return P;
    	vector<Point> H(2*n);

    	// Sort points lexicographically
    	sort(P.begin(), P.end());

    	// Build lower hull
    	for (int i = 0; i < n; ++i) {
    		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
    		H[k++] = P[i];
    	}

    	// Build upper hull
    	for (int i = n-1, t = k+1; i >= 0; --i) {
    		while (k >= t && cross(H[k-2], H[k-1], P[i-1]) <= 0) k--;
    		H[k++] = P[i-1];
    	}

    	H.resize(k-1);
    	return H;
    }
    static double distanceToLineSegment(Point lp1, Point lp2, Point p) {
      double lx1 = lp1.x(), ly1 = lp1.y();
      double lx2 = lp2.x(), ly2 = lp2.y();
      double px = p.x(), py = p.y();

      double ldx = lx2 - lx1,
             ldy = ly2 - ly1,
             lineLengthSquared = ldx*ldx + ldy*ldy;
      double t;
      if (lineLengthSquared == 0) {
        t = 0;
      } else {
        t = ((px - lx1) * ldx + (py - ly1) * ldy) / lineLengthSquared;
        if (t < 0) t = 0;
        else if (t > 1) t = 1;
      }

       double lx = lx1 + t * ldx,
              ly = ly1 + t * ldy,
              dx = px - lx,
              dy = py - ly;

      return sqrt(dx * dx + dy * dy);
    }
    static bool pointInPolygon(Point point, vector<Point> vs) {
      double x = point.x(), y = point.y();
      bool inside = false;

      for (int i = 0, j = vs.size() - 1; i < vs.size(); j = i++) {
        double xi = vs[i].x(), yi = vs[i].y();
        double xj = vs[j].x(), yj = vs[j].y();

        double intersect = ((yi > y) != (yj > y))
                    && (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
      }

      return inside;
    }

    static int ccw(Point a, Point b, Point c) {
        int area = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
        if (area > 0)
            return -1;
        else if (area < 0)
            return 1;
        return 0;
    }


    static vector<Point> convexHull(vector<Point> points)    {
        stack<Point> hull;

        if (points.size() < 3)
            return vector<Point>();

        // find the point having the least y coordinate (pivot),
        // ties are broken in favor of lower x coordinate
        int leastY = 0;
        for (int i = 1; i < points.size(); i++)
            if (points[i] < points[leastY])
                leastY = i;

        // swap the pivot with the first point
        Point temp = points[0];
        points[0] = points[leastY];
        points[leastY] = temp;

        // sort the remaining point according to polar order about the pivot
        Point pivot = points[0];
        sort(points.begin() + 1, points.end(), [pivot](Point a, Point b) {
          int order = ccw(pivot, a, b);
          if (order == 0)
              return distance(pivot, a) < distance(pivot, b);
          return (order == -1);
        });

        hull.push(points[0]);
        hull.push(points[1]);
        hull.push(points[2]);

        for (int i = 3; i < points.size(); i++) {
            Point top = hull.top();
            hull.pop();
            while (ccw(hull.top(), top, points[i]) != -1)   {
                top = hull.top();
                hull.pop();
            }
            hull.push(top);
            hull.push(points[i]);
        }

        vector<Point> result;
        while (!hull.empty()) {
          Point top = hull.top();
          hull.pop();
          result.push_back(top);
        }
        reverse(result.begin(), result.end());

        return result;
    }
};


#endif /* GEOMETRY_GEOMATHHELPER_H_ */
