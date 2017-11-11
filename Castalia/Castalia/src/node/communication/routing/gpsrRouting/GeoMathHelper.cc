#include "GeoMathHelper.h"

double G::distance(Point p1, Point p2) {
    return distance(p1.x_, p1.y_, p2.x_, p2.y_);
}
double G::distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}