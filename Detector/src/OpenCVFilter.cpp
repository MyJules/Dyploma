#include "OpenCVFilter.h"

QVideoFrame TestFilter::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    QElapsedTimer timer;
    timer.start();

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
    QImage image = QImage(input->bits(),input->width(), input->height(),
                          QImage::Format_RGB32).copy().mirrored(false, true);

    // Use if you want proper colors, but conversion works slow
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

    auto font = cv::FONT_HERSHEY_SIMPLEX;
    cv::putText(cvImage, "Hello OpenCv", {40, 500}, font, 4, {0, 0, 0}, 2, cv::LINE_AA);

    //cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2GRAY);

    QImage processedImage = cvutils::cvMatToQImage(cvImage);

    QPainter painter(&image);
    painter.drawImage(image.rect(), processedImage);
    painter.end();

    qDebug() << "Elapsed Time: " << timer.elapsed();

    return image;
}
