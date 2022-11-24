#include "ivideofilter.h"

#include <cvutils.h>

QVideoFrame IVideoFilter::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
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

    auto processed = process(cvImage);

    QImage processedImage = cvutils::cvMatToQImage(processed);
    QPainter painter(&image);
    painter.drawImage(image.rect(), processedImage);
    painter.end();

    qDebug() << "Elapsed Time: " << timer.elapsed();

    return image;
}
