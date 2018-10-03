#include "Classifier.hpp"

int main(int argc, char **argv)
{
    cv::VideoCapture    capture;
    cv::Mat             first_frame;
    cv::Mat             frame;
    cv::Mat             moving_mask;
    std::string         window_name = "Thread break Detection";
    Classifier          thread_detector("../cascadeHaar/strings_cascade/cascade.xml");
    Classifier          wheel_detector("../cascadeHaar/wheels_cascade/cascade.xml");

    if (!thread_detector.isCreated() || !wheel_detector.isCreated())
        return -1;

    capture.open(argc > 1 ? argv[1] : 0);
    
    if (!capture.isOpened())
    {
        std::cout << "Error: Can't open video capture" << std::endl;
        return -1;
    }

    for (int i = 0; i < 10; i++)
        capture.read(first_frame);
    thread_detector.setFirstFrame(first_frame);
    thread_detector.setFirstFrameDetect(10, cv::Size(20, 400), cv::Size(40, 600));

    while (capture.read(frame))
    {
        if (frame.empty())
        {
            std::cout << "Error: No captured frame" << std::endl;
            break;
        }

        thread_detector.detect(frame, 10, cv::Size(20, 400), cv::Size(40, 600));
        thread_detector.check();
        thread_detector.show(frame);

        cv::imshow(window_name, frame);

        int c = cv::waitKey(10);
        
        if((char)c == 27)
            break;
    }
    return 0;
}
