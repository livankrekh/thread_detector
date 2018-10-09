# Thread broke detector

Simple Computer vision project (good example for using openCV 3 on C++ style)

This project detects all threads in textile machine (and detects cases, when threads are broken or other object overlap threads)

## General instructions

For installing and compiling project you need to install:
* openCV with extra contrib modules ([Compile and install tutorial for Ubuntu](https://www.learnopencv.com/install-opencv3-on-ubuntu/))
* g++ with c++11/14 standart supporting

## Code instructions

This project uses next algorithms:
* `Haar's cascade` for detecting threads and wheels on textile machine (dataset for training in `./cascadeHaar` directory)
```cpp
void detect(cv::Mat & frame, int neighbors, cv::Size min_size, cv::Size max_size);

cv::Mat & frame   - captured frame from video or web-cam (and transformed to grey binary image)
int neighbors     - normal overlaping coefficient (for `cv::CascadeClassifier::detectMultiScale` openCV method)
cv::Size min_size - minimal rectangle size for detecting (for `cv::CascadeClassifier::detectMultiScale` openCV method)
cv::Size max_size - maximal rectangle size for detecting (for `cv::CascadeClassifier::detectMultiScale` openCV method)
```

* `BackgroundSubstractorMOG` for detecting other object, that overlaps threads and textile machine (uses `cv::BackgroundSubtractor` from OpenCV contrib modules)
```cpp
bool detectOther(cv::Mat & frame);

cv::Mat & frame - captured frame tranformed to binary image
returns true, when overlapped object detected
```
