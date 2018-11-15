#include "scene_text_detector.h"

void visualize_det_output(std::vector<Tensor>& outputs){
  std::cout<<outputs[0].DebugString()<<std::endl;

  int dim = outputs[0].dims();
  std::cout<<"num dim: "<<dim<<" "<<outputs[0].NumElements()<<std::endl;
  
  auto out_score_mat = outputs[0].matrix<float>();
  std::cout<<out_score_mat.NumDimensions<<std::endl;

  const Eigen::Tensor<float, out_score_mat.NumDimensions>::Dimensions& out_score_dim = out_score_mat.dimensions();
  LOG(INFO)<<"score dimensions: "<<out_score_mat.NumDimensions<<" "<<out_score_dim[0];

  //tensor to cv mat
  //cv::Mat vis_score = tensor_to_cv_mat(outputs[0]);
  //std::cout<<vis_score.rows<<" "<<vis_score.cols<<std::endl;
  //double min_v, max_v;
  //cv::minMaxLoc(vis_score, &min_v, &max_v);

}

SceneTextDetector::SceneTextDetector(const std::string frozen_graph_filename){
  this->init(frozen_graph_filename);
}

bool SceneTextDetector::init(const std::string frozen_graph_filename){
  init_graph(frozen_graph_filename); 
  input_layer = "input_images:0";
  output_layers = str_util::Split("text_boxes:0", ',');
  nms_th = 0.2;
}

int SceneTextDetector::run_graph(const cv::Mat& image){
  cv::Mat resized_image;
  float ratio_h=0, ratio_w=0;
  resize_image_max_len(image, resized_image, ratio_h, ratio_w);
  
  auto input_tensor = cv_mat_to_tensor(resized_image);

  std::vector<Tensor> outputs;
  Status run_status = this->session->Run({{this->input_layer, input_tensor}},
                                   this->output_layers, {}, &outputs);
  if (!run_status.ok()) {
    LOG(ERROR) << "Running model failed: " << run_status;
    return -1;
  }
  LOG(INFO) <<"number of output:"<<outputs.size();

  //auto out_tex_boxes = outputs[0].flat_outer_dims<float>();
  //visualize_det_output(outputs);
  //std::cout<<outputs[0].DebugString()<<std::endl;
  //auto out_text_boxes = outputs[0];
  auto detection_boxes = outputs[0].tensor<float, 2>();
  int num_box = detection_boxes.dimension(0);
  for(int i=0; i<num_box; i++){
    std::cout<<i<<" ";
    for(int j=0; j<9; j++)
      std::cout<<detection_boxes(i, j)<<" ";
    std::cout<<std::endl;
  }

  float* data = detection_boxes.data();
  for(int i=0; i<20; i++) std::cout<<data[i]<<" ";
}


std::vector<TextBox> SceneTextDetector::detect(cv::Mat& score_map, cv::Mat& geometry_map, float score_map_thresh, float box_thresh, float nms_thresh){
  std::vector<TextBox> res;

  return res;
}


bool SceneTextDetector::init_graph(const std::string& frozen_graph_filename){
  if (!ReadBinaryProto(tensorflow::Env::Default(), frozen_graph_filename, &graph_def).ok()) {
    LOG(ERROR) << "Read proto";
    return -1;
  } 
  
  tensorflow::SessionOptions sess_opt;
  sess_opt.config.mutable_gpu_options()->set_allow_growth(true);
  (&session)->reset(tensorflow::NewSession(sess_opt));
  if (!session->Create(graph_def).ok()) {
    LOG(ERROR) << "Create graph";
    return -1;
  }
}
