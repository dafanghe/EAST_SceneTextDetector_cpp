# DeepSceneTextReader
This is a c++ project deploying the famous [EAST scene text detector](http://openaccess.thecvf.com/content_cvpr_2017/papers/Zhou_EAST_An_Efficient_CVPR_2017_paper.pdf).
It detects text from natural scene images.
To combine this detector with scene text recognizer for a full pipeline, checkout my another [repo](https://github.com/dafanghe/DeepSceneTextReader).

<p align="center">
  <img src="images/img_9120.jpg" width=1280 height=720>
  <img src="images/img_9132.jpg" width=1280 height=720>
  <img src="images/img_9162.jpg" width=1280 height=720>
  <img src="images/img_9185.jpg" width=1280 height=720>
</p>


# Prerequsites

The project is written in c++ using tensorflow computational framework.
It is tested using tensorflow 1.4. Newer version should be ok too, but not tested.
Please install:

* Tensorflow

* nsync project: https://github.com/google/nsync.git  This is needed for building tensorflow.

* opencv3.3

* protobuf

* eigen

Please check this project on how to build project using tensorflow with cmake:
https://github.com/cjweeks/tensorflow-cmake
It greatly helped the progress of building this project.
When building tensorflow library, please be careful since we need to use opencv. Looks like there is still problem when including tensorflow and opencv together.
It will make opencv unable to read image.
Check out this issue: https://github.com/tensorflow/tensorflow/issues/14267
The answer by allenlavoie solved my problem, so I paste it here:

"In the meantime, as long as you're not using any custom ops you can build libtensorflow_cc.so with bazel build --config=monolithic, which will condense everything together into one shared object (no libtensorflow_framework dependence) and seal off non-TensorFlow symbols. That shared object will have protocol buffer symbols."

# build process
cd build

cmake ..

make

It will create an excutable named **DetectText** in bin folder.

## Usage
Download the pretrained detector model and put it in model/

./DetectText --detector_graph='model/Detector_model.pb' \
   --input_dir=/path/to/test/images/ --output_dir=/path/to/write/output/directory

# Model Description
The detector training is based on the public [repo](https://github.com/argman/EAST).

The python script for converting the trained model to the frozen graph will be provided.
Basically the frozen graph output is all the decoded text boxes(before nonmax suppresion) instead of the score_map and geometry_map.
I implemented a simple nonmax suppresion based on polygon IOU.
Boost::geometry is used for polygon related calculation.

### *Pretrained Models*
A pretrained frozen graph will be provided based on the original code in [repo](https://github.com/argman/EAST).

# Reference and Related Projects
- [EAST](http://openaccess.thecvf.com/content_cvpr_2017/papers/Zhou_EAST_An_Efficient_CVPR_2017_paper.pdf) EAST: an efficient and accurate scene text detector paper.
- [tensorflow-cmake](https://github.com/cjweeks/tensorflow-cmake), Tutorial of Building Project with tensorflow using cmake.

# Contact:

* Dafang He. The Penn State University.  hdfcraig@gmail.com   http://personal.psu.edu/duh188/
