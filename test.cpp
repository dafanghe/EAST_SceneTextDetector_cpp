#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(){
  string filename = "test.jpg";
  cv::Mat image = cv::imread(filename);
  if(!image.data)                              // Check for invalid input
  {
      std::cout <<  "Could not open or find the image " << filename << std::endl ;
      return -1;
  } 
  std::cout<<image.rows<<" "<<image.cols<<std::endl;
}
