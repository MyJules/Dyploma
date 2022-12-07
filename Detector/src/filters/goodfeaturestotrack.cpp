#include "goodfeaturestotrack.h"

#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <cvutils.h>

GoodFeaturesToTrack::GoodFeaturesToTrack()
{
}

cv::Mat GoodFeaturesToTrack::process(const cv::Mat& image)
{
    cv::Mat result = image;
    cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);

    if(m_isNewImageToTrack)
    {
        m_imageToTrack = image;
        m_isNewImageToTrack = false;
    }

    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(result, corners, 25, 0.01, 10);

    for(int i = 0; i < corners.size(); i++)
    {
        cv::circle(result, corners[i], 4, 255, 1);
    }

    return result;
}
