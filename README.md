# CPPND: Capstone - deepSORT
This is my Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 
The goal is to re-implement the popular object tracking algorithm [deepSORT]([https://arxiv.org/abs/1703.07402](https://arxiv.org/abs/1703.07402)) in C++.

## Dependencies for Running Locally
* cmake >= 3.10
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV == 4.3 (I think all OpenCV 4 version will work)
* Eigen3

## Basic Build Instructions

1. Clone this repo.
2. Install OpenCV 4.3 and Eigen3 libraries
3. Compile: `make build`
4. Run it: `cd build && ./deepSORT [path to test video]`.
