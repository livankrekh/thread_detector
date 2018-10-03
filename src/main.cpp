#include "Classifier.hpp"

bool trackChanged = false;

void onMoved(int, void*) { trackChanged = true; }

int main(int argc, char **argv)
{
    cv::VideoCapture    capture;
    cv::Mat             first_frame;
    cv::Mat             frame;
    cv::Mat             moving_mask;
    std::string         window_name = "Thread break Detection";
    Classifier          thread_detector("cascadeHaar/strings_cascade/cascade.xml");
    Classifier          wheel_detector("cascadeHaar/wheels_cascade/cascade.xml");
    bool                hasOther = false;
    int                 value = 0;

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
    thread_detector.detectOther(first_frame);
    cv::namedWindow(window_name, 0);
    cv::createTrackbar("Video scrolling", window_name, &value, capture.get(cv::CAP_PROP_FRAME_COUNT), onMoved);

    while (capture.read(frame))
    {
        if (trackChanged)
        {
            capture.set(cv::CAP_PROP_POS_FRAMES, value);
            trackChanged = false;
        }

        if (frame.empty())
        {
            std::cout << "Error: No captured frame" << std::endl;
            break;
        }

        if (!thread_detector.detectOther(frame))
        {
            thread_detector.detect(frame, 10, cv::Size(20, 400), cv::Size(40, 600));
            thread_detector.check();
            thread_detector.show(frame);
            hasOther = false;
        }
        else
        {
            if (!hasOther)
            {
                std::time_t result = std::time(nullptr);
                std::cout << std::asctime(std::localtime(&result))
                            << "Camera has been overlaped by undefined object! Cannot detect threads state :("
                            << std::endl << std::endl;
            }
            hasOther = true;
        }

        cv::imshow(window_name, frame);

        int c = cv::waitKey(10);
        
        if((char)c == 27)
            break;
    }
    return 0;
}
