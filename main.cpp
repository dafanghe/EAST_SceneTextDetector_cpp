#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "tensorflow/core/util/command_line_flags.h"
#include "scene_text_detector.h"
#include <opencv2/core.hpp>


int main(int argc, char** argv) {
  std::cout<<"start text detection:"<<std::endl;
  //do text detection
  string graph = "";
  string filename = "";
  string output_filename = ""; 
  std::vector<Flag> flag_list = {
      Flag("detector_graph", &graph, "graph file name"),
      Flag("filename", &filename, "the filename to be tested."),
      Flag("output_filename", &output_filename,
          "the output filename"),
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

  cv::Mat image = cv::imread(filename);
  if(!image.data)                              // Check for invalid input
  {
      std::cout <<  "Could not open or find the image " << filename << std::endl ;
      return -1;
  } 
  detector.run_graph(image);
  return 0;
}
