#include <deque>

#include <algorithm> 
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

namespace bg = boost::geometry;

typedef bg::model::point<float, 2, bg::cs::cartesian> Point;
typedef bg::model::polygon<Point, false, false> Polygon; // ccw, open polygon

struct poly_with_score{
  poly_with_score(){
  }
  poly_with_score(Polygon poly_, float score_){
    poly = poly_;
    score = score_;
  }
  Polygon poly;
  float score;
};

std::vector<poly_with_score> nonmax_suppresion(const float* data, 
    int num_box, float nms_th);

float poly_iou(Polygon a, Polygon b);

bool should_merge(poly_with_score a, poly_with_score b, float threshold);
