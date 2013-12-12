
#ifndef STITCHING_H_
#define STITCHING_H_

#include <opencv2/core/core.hpp>
#include <opencv2/core/version.hpp>
#include <opencv2/features2d/features2d.hpp>

class Stitching{
 public:
  explicit Stitching (int num_images);
  virtual ~Stitching() {}

  bool InitialiseOpenCV(int width, int height);

  // Calculate homography based on two input images. Once is calculated OK, the
  // homography can be retrieved using homography(). InitialiseOpenCV() must
  // have been called and succeeded previously.
  bool CalculateHomography();

  const char* GetOpenCVVersion() const { return CV_VERSION; }
  const cv::Mat&  homography() const { return homography_; }
  const std::string& last_error() const { return last_error_; }

 private:
  // Only 2 images supported ATM, checked in InitialiseOpenCV()
  int num_images_;
  std::vector<cv::Mat*> input_img_;
  cv::Size image_size_;

  cv::Ptr<cv::FeatureDetector> detector_;
  cv::Ptr<cv::DescriptorExtractor> extractor_;
  cv::Ptr<cv::DescriptorMatcher> matcher_;

  cv::vector<cv::KeyPoint> keypoints_[2];
  cv::Ptr<cv::Mat> descriptors_[2];

  cv::vector<cv::DMatch> matches_;
  std::vector<cv::DMatch> good_matches_;

  cv::Mat homography_;

  std::string last_error_;
};


#endif  // STITCHING_H_
