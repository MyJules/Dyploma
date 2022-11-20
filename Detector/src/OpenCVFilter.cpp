#include "OpenCVFilter.h"

#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <cvutils.h>

QVideoFrame TestFilter::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
//    QElapsedTimer timer;
//    timer.start();

    if (!input->isValid())
    {
        qDebug() << "Frame is not valid!\n";
        return *input;
    }

    if(!input->map(QAbstractVideoBuffer::MapMode::ReadOnly))
    {
       qDebug() << "Failed to map frame!\n";
       return *input;
    }

#ifdef Q_OS_ANDROID
    QImage image = QImage(input->bits(), input->width(), input->height(),
                          QImage::Format_RGB32).copy().mirrored(false, true);

    if(input->pixelFormat() == QVideoFrame::Format_ABGR32)
    {
        image = cvutils::ABGRtoRGB(image);
    }
#else
    QImage image = input->image();
#endif

    if(input->isMapped())
    {
        input->unmap();
    }

    //Process image here
    cv::Mat cvImage = cvutils::QImageToCvMat(image);

    cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2GRAY);

    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(cvImage, corners, 25, 0.01, 10);

    for(int i = 0; i < corners.size(); i++)
    {
        cv::circle(cvImage, corners[i], 4, 255, 1);
    }

    QImage processedImage = cvutils::cvMatToQImage(cvImage);
    QPainter painter(&image);
    painter.drawImage(image.rect(), processedImage);
    painter.end();

//    qDebug() << "Elapsed Time: " << timer.elapsed();

    return image;
}
