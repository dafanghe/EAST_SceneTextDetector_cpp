#ifndef Scene_Text_Detector_H
#define Scene_Text_Detector_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

//tensorflow
//#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
//#include "tensorflow/cc/ops/const_op.h"
//#include "tensorflow/cc/ops/image_ops.h"
//#include "tensorflow/cc/ops/standard_ops.h"
//#include "tensorflow/core/framework/attr_value.pb.h"
//#include "tensorflow/core/framework/function.pb.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/node_def.pb.h"
#include "tensorflow/core/framework/tensor_shape.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/step_stats.pb.h"
#include "tensorflow/core/graph/algorithm.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/graph/graph_constructor.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/platform.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/reporter.h"

//opencv
#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp"

#include "lanms.h"
#include "utils.h"
#include "text_box.h"

using namespace tensorflow;


class SceneTextDetector{
  public:
    SceneTextDetector(){};

    SceneTextDetector(const std::string frozen_graph_filename);
    //std::string input_layer_, std::string output_layer_string);
    
    bool init(const std::string);
    int run_graph(std::string image_filename);
    int run_graph(const cv::Mat& image);

  private:
    std::vector<TextBox> detect(cv::Mat& score_map, cv::Mat& geometry_map, float, float ,float);

    bool init_graph(const std::string& frozen_graph_filename);
    tensorflow::GraphDef graph_def;
    std::string input_layer;
    std::unique_ptr<tensorflow::Session> session;
    std::vector<string> output_layers;
    float score_map_th;
    float box_th;
    float nms_th;
};

void visualize_det_output(std::vector<Tensor>&);
#endif 
