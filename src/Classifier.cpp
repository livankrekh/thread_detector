#include "Classifier.hpp"

static bool compare_rect(cv::Rect rect1, cv::Rect rect2)
{
	return rect1.x < rect2.x;
}

Classifier::Classifier(std::string const path)
{
	this->detected = std::vector<cv::Rect>();
	this->first_detected = std::vector<cv::Rect>();
	this->created = true;
	this->first_frame = cv::Mat();

	if (!this->main_classifier.load(path))
    {
        std::cout << "Error: Can't load cascade from path: " << path << std::endl;
        this->created = false;
    }

    for (int i = 0; i < 6; i++)
    {
    	this->disabled_count[i] = 0;
    	this->enabled_count[i] = 0;
    }
}

Classifier::~Classifier(void)
{
}

cv::CascadeClassifier & Classifier::get_classifier(void)
{
	return this->main_classifier;
}

void Classifier::sortDetected(std::vector<cv::Rect> & tmp)
{
	std::sort(tmp.begin(), tmp.end(), compare_rect);
}

bool Classifier::isCreated(void)
{
	return this->created;
}

void Classifier::setFirstFrame(cv::Mat & frame)
{
	if (frame.empty())
		return ;

	cv::cvtColor(frame, this->first_frame, cv::COLOR_BGR2GRAY);
}

void Classifier::setFirstFrameDetect(int neighbors, cv::Size min_size, cv::Size max_size)
{
	cv::Mat copy;
	int		size;

	if (first_frame.empty())
		return ;

	cv::equalizeHist(this->first_frame, copy);
	this->main_classifier.detectMultiScale(this->first_frame, this->first_detected, 1.1, neighbors, 0, min_size, max_size);
	size = first_detected.size();

	for (int i = 0; i < size; i++)
	{
		first_detected.push_back(cv::Rect(first_detected[i]));
	}

	cv::groupRectangles(this->first_detected, 1, 0.6);
	this->sortDetected(this->first_detected);
}

void Classifier::detect(cv::Mat & frame, int neighbors, cv::Size min_size, cv::Size max_size)
{
	cv::Mat 	frame_gray;
	int 		size;

    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);

    this->main_classifier.detectMultiScale(frame_gray, this->detected, 1.1, neighbors, 0, min_size, max_size);
    size = detected.size();

    for(int i = 0; i < size; i++)
	{
    	detected.push_back(cv::Rect(detected[i]));
	}

    cv::groupRectangles(this->detected, 1, 0.6);
    this->sortDetected(this->detected);
}

void Classifier::check(void)
{
	std::vector<cv::Rect> 	copy;

	if (detected.empty() || first_frame.empty())
		return ;

	for (unsigned int i = 0; i < first_detected.size(); i++)
	{
		copy = this->detected;

		copy.push_back(first_detected[i]);
		cv::groupRectangles(copy, 1, 0.6);
		if (copy.size() == 0)
		{
			if (this->disabled_count[i] < 20)
			{
				this->disabled_count[i]++;
			}
			else if (this->disabled_count[i] == 20)
			{
				std::time_t result = std::time(nullptr);
    			std::cout << std::asctime(std::localtime(&result))
							<< "Thread #" << i + 1 << " is disabled!"
							<< std::endl << std::endl;
				this->disabled_count[i]++;
				this->enabled_count[i] = 0;
			}
		}
		else
		{
			if (this->disabled_count[i] >= 20)
			{
				if (this->enabled_count[i] < 10)
				{
					this->enabled_count[i]++;
				}
				else if (this->enabled_count[i] == 10)
				{
					std::time_t result = std::time(nullptr);
    				std::cout << std::asctime(std::localtime(&result))
								<< "Thread #" << i + 1 << " is enabled!"
								<< std::endl << std::endl;

					this->disabled_count[i] = 0;
					this->enabled_count[i]++;
				}
			}
			else if (this->disabled_count[i] > 0)
				this->disabled_count[i] = 0;
		}
	}

}

void Classifier::show(cv::Mat & frame)
{
	if (detected.empty())
		return ;

	for (auto edge : this->detected)
    {
        cv::rectangle(frame, edge, cv::Scalar(0, 128, 0), 3);
    }
}
