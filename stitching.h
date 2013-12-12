
#ifndef STITCHING_H_
#define STITCHING_H_

#include <opencv2/core/core.hpp>
#include <opencv2/core/version.hpp>

class Stitching{
 public:
  Stitching ();
  virtual ~Stitching() {}

  const char* GetOpenCVVersion();

 private:
  cv::Mat input_img_[3];
};


#endif  // STITCHING_H_
