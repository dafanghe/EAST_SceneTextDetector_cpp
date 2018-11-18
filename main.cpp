#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include "boost/filesystem.hpp"
#include <fstream>

#include "tensorflow/core/util/command_line_flags.h"
#include "scene_text_detector.h"
#include <opencv2/core.hpp>

struct path_leaf_string
{
  std::string operator()(const boost::filesystem::directory_entry& entry) const
  {
      return entry.path().leaf().string();
  }
};


void read_directory(const std::string& name, std::vector<std::string>& v)
{
  boost::filesystem::path p(name);
  boost::filesystem::directory_iterator start(p);
  boost::filesystem::directory_iterator end;
  std::transform(start, end, std::back_inserter(v), path_leaf_string());
}


int main(int argc, char** argv) {
  std::cout<<"start text detection:"<<std::endl;
  //do text detection
  string graph = "";
  string input_dir = "";
  string output_dir = ""; 
  std::vector<Flag> flag_list = {
      Flag("detector_graph", &graph, "graph file name"),
      Flag("input_dir", &input_dir, "the directory containing the input images."),
      Flag("output_dir", &output_dir,
          "the output directory"),
  };
  string usage = Flags::Usage(argv[0], flag_list);
  const bool parse_result = Flags::Parse(&argc, argv, flag_list);

  if (!parse_result) {
    LOG(ERROR) << usage;
    return -1;
  }

  ::tensorflow::port::InitMain(argv[0], &argc, &argv);
  if (argc > 1) {
    LOG(ERROR) << "Unknown argument " << argv[1] << "\n" << usage;
    return -1;
  }

  SceneTextDetector detector(graph);

  std::vector<std::string> paths;
  read_directory(input_dir, paths);
  for(int i=0; i<paths.size(); i++){
    std::string suffix = paths[i].substr(paths[i].size()-3, 3);
    if(suffix != "jpg" && suffix != "bmp" && suffix != "bmp")
      continue;
    std::string image_filename = input_dir + paths[i];
    std::cout<<"detect text in image: " << image_filename<<std::endl; 
    cv::Mat image = cv::imread(image_filename);
    if(!image.data)                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image " << image_filename << std::endl ;
        continue;
    } 
    std::vector<TextBox> res;
    detector.run_graph(image, res);
    
    std::vector<cv::Scalar> colors={cv::Scalar(0,0,255), cv::Scalar(0,255,0),
      cv::Scalar(255,0,0), cv::Scalar(255,255,0), cv::Scalar(0,255,255), cv::Scalar(255,0,255)};
    for(int i=0; i<res.size(); i++){
      std::vector<cv::Point> points = res[i].get_points(); 
      for(int j=0; j<4; j++){
        cv::line(image, points[j], points[(j+1)%4], colors[j%4], 3); 
      }
    }
    
    std::string outname_prefix = paths[i].substr(0, paths[i].size() - 4);
    std::string output_filename = output_dir + "res_" + outname_prefix + ".txt";
    std::ofstream ofs(output_filename, std::ofstream::out);
    for(int i=0; i<res.size(); i++){
      std::vector<cv::Point> points = res[i].get_points();
      string out;
      for(int j=0; j<4; j++)
        out += std::to_string(points[j].x) + "," + std::to_string(points[j].y)+ ",";
      ofs << out.substr(0, out.size()-1) << std::endl;
    }
    
    //output_filename = output_dir + outname_prefix + ".jpg";
    //cv::imwrite(output_filename, image);
  }

  return 0;
}
