#include <iostream>
#include <deque>

#include <boost/version.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

namespace bg = boost::geometry;


int main() {
  typedef bg::model::point<float, 2, bg::cs::cartesian> point;
  typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon

  std::cout << "Boost version: " 
          << BOOST_VERSION / 100000
          << "."
          << BOOST_VERSION / 100 % 1000
          << "."
          << BOOST_VERSION % 100 
          << std::endl;

  polygon p;
  p.outer().push_back(point(0.0, 0.0));
  p.outer().push_back(point(2.0, 0.0));
  p.outer().push_back(point(2.0, 2.0));
  p.outer().push_back(point(0.0, 2.0));
  
  polygon p2;
  p2.outer().push_back(point(1.0, 1.0));
  p2.outer().push_back(point(3.0, 1.0));
  p2.outer().push_back(point(3.0, 3.0));
  p2.outer().push_back(point(1.0, 3.0));
  
  for(int j=0; j<4; j++)
    std::cout<<p2.outer()[j].get<0>()<<" "<<p2.outer()[j].get<1>()<<std::endl;

  std::deque<polygon> output;
  boost::geometry::intersection(p, p2, output);
  for(int i=0; i<output.size(); i++)
    std::cout<<boost::geometry::area(output[i])<<std::endl;
  return 0;
}

