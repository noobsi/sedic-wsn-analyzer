#include "GeoMathHelper.h"

Line G::line(Point p1, Point p2) {
    Line re;
    line(p1.x_, p1.y_, p2.x_, p2.y_, re.a_, re.b_, re.c_);
    return re;
}

void G::line(double x1, double y1, double x2, double y2, double &a, double &b,
        double &c) {
    a = y1 - y2;
    b = x2 - x1;
    c = -y1 * x2 + y2 * x1;
}

double G::distance(Point p1, Point p2) {
    return distance(p1.x_, p1.y_, p2.x_, p2.y_);
}
double G::distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool G::is_intersect(Point* p1, Point* p2, Point* p3, Point* p4) {
  Line l1 = line(p1, p2);
  Line l2 = line(p3, p4);
  Point in;
  return (intersection(l1, l2, in)
          && ((in.x_ - p1->x_) * (in.x_ - p2->x_) <= 0)
          && ((in.y_ - p1->y_) * (in.y_ - p2->y_) <= 0)
          && ((in.x_ - p3->x_) * (in.x_ - p4->x_) <= 0)
          && ((in.y_ - p3->y_) * (in.y_ - p4->y_) <= 0));
}
bool G::intersection(Point* p1, Point* p2, Point* p3, Point* p4, Point& p) {
    Line l1 = line(p1, p2);
    Line l2 = line(p3, p4);
    return (intersection(l1, l2, p) && ((p.x_ - p1->x_) * (p.x_ - p2->x_) <= 0)
            && ((p.y_ - p1->y_) * (p.y_ - p2->y_) <= 0)
            && ((p.x_ - p3->x_) * (p.x_ - p4->x_) <= 0)
            && ((p.y_ - p3->y_) * (p.y_ - p4->y_) <= 0));
}

bool G::intersection(Line l1, Line l2, Point& p) {
  return intersection(l1.a_, l1.b_, l1.c_, l2.a_, l2.b_, l2.c_, p.x_, p.y_);
}


bool G::intersection(double a1, double b1, double c1, double a2, double b2,
        double c2, double &x, double &y) {
    if (a1 == 0 && b1 == 0)
        return false;
    if (a2 == 0 && b2 == 0)
        return false;

    if (a1 == 0 && b2 == 0) {
        x = -c2 / a2;
        y = -c1 / b1;
    } else if (a2 == 0 && b1 == 0) {
        x = -c1 / a1;
        y = -c2 / b2;
    } else if (a1 * b2 != a2 * b1) {
        x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
        y = (c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1);
    } else
        return a1 * c2 == a2 * c1;

    return true;
}