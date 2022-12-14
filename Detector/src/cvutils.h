#ifndef CVUTILS_H
#define CVUTILS_H

#include <QImage>
#include <QVideoFrame>
#include <opencv2/opencv.hpp>

namespace cvutils
{
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData = true);
    QImage ABGRtoRGB(const QImage &image);
}

#endif // CVUTILS_H
