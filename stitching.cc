
#include "stitching.h"

#include <opencv2/core/version.hpp>
#include "opencv2/stitching.hpp"

Stitching::Stitching() {
  cv::Stitcher stitcher = cv::Stitcher::createDefault(cv::try_use_gpu);

}

const char* Stitching::GetOpenCVVersion() {
  return CV_VERSION;
}
