#include "polys.h"

std::vector<poly_with_score> nonmax_suppresion(const float* data, 
    int num_box, float nms_th){
  std::vector<poly_with_score> input_polys;
  if(num_box == 0)
    return input_polys;
  //std::cout<<"num box: "<<num_box<<std::endl;
  for(size_t i=0; i<num_box; i++){
    auto p = data + i * 9;
    //for(int j=0; j<9; j++)
    //  std::cout<<p[j]<<" ";
    //std::cout<<std::endl;
    Polygon poly;
    for(int j=0; j<4; j++){
      poly.outer().push_back(Point(p[2*j], p[2*j+1]));
    }
    input_polys.push_back(poly_with_score(poly, p[8]));
  }
  
  std::vector<size_t> indices(num_box);
	std::iota(std::begin(indices), std::end(indices), 0);
	std::sort(std::begin(indices), std::end(indices),
      [&](size_t i, size_t j) { return input_polys[i].score > input_polys[j].score; });
		
  std::vector<size_t> keep;
	while (indices.size()) {
		size_t p = 0, cur = indices[0];
		keep.emplace_back(cur);
		for (size_t i = 1; i < indices.size(); i ++) {
			if (!should_merge(input_polys[cur],
            input_polys[indices[i]], nms_th)) {
				indices[p ++] = indices[i];
			}
		}
		indices.resize(p);
	}
 
  std::vector<poly_with_score> ret;
	for (auto &&i: keep) {
		ret.emplace_back(input_polys[i]);
	}
	return ret;
}

float poly_iou(Polygon a, Polygon b){
  std::deque<Polygon> output;
  boost::geometry::intersection(a, b, output);
  if(output.size() == 0)
    return 0;
  return boost::geometry::area(output[0]);
}

bool should_merge(poly_with_score a, poly_with_score b, float threshold){
  float inter_area = poly_iou(a.poly, b.poly); 
  float area1 = boost::geometry::area(a.poly);
  float area2 = boost::geometry::area(b.poly);
  float iou_score = inter_area / (area1 + area2 - inter_area);
  return (iou_score > threshold);
}



