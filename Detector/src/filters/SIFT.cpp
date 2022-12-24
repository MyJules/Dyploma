#include "SIFT.h"

#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <cvutils.h>

SIFT::SIFT()
{
}

SIFT::~SIFT()
{
}

cv::Mat SIFT::process(const cv::Mat& image)
{
    using namespace cv;

    if(m_isNewImageToTrack)
    {
        m_imageToTrack = image;
        m_isNewImageToTrack = false;
    }

    if(!m_imageToTrack) return image;

    cv::Mat result = image;
    cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 400;
    Ptr<cv::SIFT> detector = cv::SIFT::create( minHessian );
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute( *m_imageToTrack, noArray(), keypoints_object, descriptors_object );
    detector->detectAndCompute( image, noArray(), keypoints_scene, descriptors_scene );

      //-- Step 2: Matching descriptor vectors with a FLANN based matcher
     // Since SURF is a floating-point descriptor NORM_L2 is used
     Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
     std::vector< std::vector<DMatch> > knn_matches;
     matcher->knnMatch( descriptors_object, descriptors_scene, knn_matches, 2 );

     //-- Filter matches using the Lowe's ratio test
     const float ratio_thresh = 0.75f;
     std::vector<DMatch> good_matches;
     for (size_t i = 0; i < knn_matches.size(); i++)
     {
         if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
         {
             good_matches.push_back(knn_matches[i][0]);
         }
     }
     //-- Draw matches
     Mat img_matches;
     //-- Localize the object
     std::vector<Point2f> obj;
     std::vector<Point2f> scene;
     for( size_t i = 0; i < good_matches.size(); i++ )
     {
         //-- Get the keypoints from the good matches
         obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
         scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
     }

     if(obj.size() < 4 && scene.size() < 4) return result;

     Mat H = findHomography( obj, scene, RANSAC );
     //-- Get the corners from the image_1 ( the object to be "detected" )
     std::vector<Point2f> obj_corners(4);
     obj_corners[0] = Point2f(0, 0);
     obj_corners[1] = Point2f( (float)m_imageToTrack->cols, 0 );
     obj_corners[2] = Point2f( (float)m_imageToTrack->cols, (float)m_imageToTrack->rows );
     obj_corners[3] = Point2f( 0, (float)m_imageToTrack->rows );
     std::vector<Point2f> scene_corners(4);
     perspectiveTransform( obj_corners, scene_corners, H);

     //-- Draw lines between the corners (the mapped object in the scene - image_2 )
     line( result, scene_corners[0] + Point2f(0, 0),
                   scene_corners[1] + Point2f(0, 0), Scalar(0, 255, 0), 4 );
     line( result, scene_corners[1] + Point2f(0, 0),
                   scene_corners[2] + Point2f(0, 0), Scalar( 0, 255, 0), 4 );
     line( result, scene_corners[2] + Point2f(0, 0),
                   scene_corners[3] + Point2f(0, 0), Scalar( 0, 255, 0), 4 );
     line( result, scene_corners[3] + Point2f(0, 0),
                   scene_corners[0] + Point2f(0, 0), Scalar( 0, 255, 0), 4 );

    return result;
}

void SIFT::onUpdateImageToTrack()
{
    m_isNewImageToTrack = true;
    qDebug() << "Update";
}

void SIFT::onResetImageToTrack()
{
    m_imageToTrack = std::nullopt;
    qDebug() << "Reset";
}
