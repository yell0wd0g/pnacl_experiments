
#include "stitching.h"

#include <opencv2/calib3d/calib3d.hpp>  // CV_RANSAC

namespace {
const int kAllowedNumberOfImages = 2;
}

Stitching::Stitching(int num_images)
  :  num_images_(num_images) {
}

bool Stitching::InitialiseOpenCV(int width, int height) {
  if (num_images_ != kAllowedNumberOfImages)
    return false;

  image_size_ = cv::Size(width, height);

  for (int i = 0; i < num_images_; ++i) {
    input_img_.push_back(new cv::Mat(height, width, CV_8UC1));
  }

  detector_ = cv::FeatureDetector::create("FAST");
  if (!detector_)
    last_error_ += "Creating feature detector failed. ";

  extractor_ = cv::DescriptorExtractor::create("FREAK");
  if (!extractor_)
    last_error_ += "Creating feature descriptor extractor failed. ";

  matcher_ = cv::DescriptorMatcher::create("FlannBased");
  if (!matcher_)
    last_error_ += "Creating feature matcher failed. ";

  descriptors_[0] = new cv::Mat();
  descriptors_[1] = new cv::Mat();

  keypoints_[0].clear();
  keypoints_[1].clear();

  return (detector_ && extractor_ && matcher_);
}

bool Stitching::CalculateHomography() {
  bool ret = true;
  last_error_.clear();

  double t_0 = (double)cv::getTickCount();

  // Extract keypoints from image. This is expensive compared to the other ops.
  detector_->detect(*input_img_[0], keypoints_[0]);
  detector_->detect(*input_img_[1], keypoints_[1]);

  // Now let's compute the descriptors.
  extractor_->compute(*input_img_[0], keypoints_[0], *descriptors_[0]);
  extractor_->compute(*input_img_[1], keypoints_[1], *descriptors_[1]);

  // Let's match the descriptors.
  matches_.clear();
  matcher_->match(*descriptors_[0], *descriptors_[1], matches_);

  // Quick calculation of max and min distances between keypoints
  double max_dist = 0; double min_dist = 100;
  for( int i = 0; i < descriptors_[0]->rows; i++ ) {
    double dist = matches_[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  // Use only "good" matches (i.e. whose distance is less than 3*min_dist )
  cv::vector<cv::DMatch> new_good_matches;
  for( int i = 0; i < descriptors_[0]->rows; i++ ) {
    if( matches_[i].distance < 2*min_dist ) {
      new_good_matches.push_back(matches_[i]);
    }
  }
  if (new_good_matches.size() > 10) good_matches_ = new_good_matches;

  // Redistribute feature points according to selected matches.
  std::vector<cv::Point2f> obj;
  std::vector<cv::Point2f> scene;
  for( unsigned int i = 0; i < good_matches_.size(); i++ ) {
    obj.push_back(keypoints_[0][good_matches_[i].queryIdx].pt);
    scene.push_back(keypoints_[1][good_matches_[i].trainIdx].pt);
  }

  // Find the Homography Matrix, if we have enough points.
  if (good_matches_.size() > 10) {
    homography_ = findHomography(obj, scene, CV_RANSAC);
  } else {
    ret = false;
    last_error_ = "Not enough features for homography calculation. ";
  }

  // Check how long did it take.
  if (cv::getTickFrequency() > 1.0)
    double t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();

  return ret;
}
