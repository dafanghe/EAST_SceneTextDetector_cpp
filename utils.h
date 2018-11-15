#ifndef Scene_Text_Utils_H
#define Scene_Text_Utils_H

#include "tensorflow/core/framework/tensor_shape.pb.h"
#include "tensorflow/core/framework/tensor.h"

//opencv
#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp"

#include <iostream>

void resize_image_max_len(const cv::Mat& image, cv::Mat& resized_image, float& ratio_h, float& ratio_w, int max_side_len=800);

void resize_image_fix_height(const cv::Mat& image, cv::Mat& resized_image, float& ratio, int fixed_height=32);

void pad_image_width(const cv::Mat& image, cv::Mat& padded_image, int target_width=128);

tensorflow::Tensor cv_mat_to_tensor(const cv::Mat& image);

cv::Mat tensor_to_cv_mat(const tensorflow::Tensor tensor);
#endif 
