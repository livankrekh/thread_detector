#ifndef CLASSIFIER_HPP
#define CLASSIFIER_HPP

#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/bgsegm.hpp>

#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

class Classifier 
{
public:

	Classifier(void) = delete;
	Classifier(Classifier const & cp) = delete;
	Classifier(std::string path);
	~Classifier(void);

	Classifier & operator= (Classifier & cp) = default;

	cv::CascadeClassifier & get_classifier(void);
	
	void sortDetected(std::vector<cv::Rect> & tmp);
	void setFirstFrame(cv::Mat & frame);
	void setFirstFrameDetect(int neighbors, cv::Size min_size, cv::Size max_size);
	// @method detect - Detect by Haar cascade
	void detect(cv::Mat & frame, int neighbors, cv::Size min_size, cv::Size max_size);
	// @method detectOther - Detect moving and other objects by background substractor
	bool detectOther(cv::Mat & frame);
	void show(cv::Mat & frame);
	// @method check - Check threads state
	void check(void);
	bool isCreated(void);

private:
	cv::Ptr<cv::BackgroundSubtractor> 	substractor;
	cv::CascadeClassifier 				main_classifier;
	std::vector<cv::Rect> 				detected;
	std::vector<cv::Rect> 				first_detected;
	cv::Mat								first_frame;
	bool				  				created;
	size_t								disabled_count[6];
	size_t								enabled_count[6];
};

#endif
