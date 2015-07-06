#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <vector>

std::vector<cv::Rect> findFaces(cv::Mat *m, unsigned int min, unsigned int max);
