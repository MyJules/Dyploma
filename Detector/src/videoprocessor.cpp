#include "videoprocessor.h"

#include "cvutils.h"

#include <opencv2/core.hpp>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <QVideoFrame>
#include <QElapsedTimer>
#include <QOpenGLFunctions>

#include <QRandomGenerator>
#include <QDateTime>

VideoProcessor::VideoProcessor(QObject *parent)
    : QObject{parent}
{
    m_timer.setInterval(500);
}

QVideoSink *VideoProcessor::videoSink() const
{
    return m_videoSink.get();
}

void VideoProcessor::setVideoSink(QVideoSink *newVideoSink)
{
    if(m_videoSink == newVideoSink)
    {
        return;
    }
    m_videoSink = newVideoSink;
    emit videoSinkChanged();
}

void VideoProcessor::start()
{
    connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &VideoProcessor::onVideoFrameChanged);
}

void VideoProcessor::onVideoFrameChanged(const QVideoFrame &videoFrame) const
{
    QElapsedTimer timer;
    timer.start();

    QVideoFrame frametodraw(videoFrame);

    if (!frametodraw.isValid())
    {
        qDebug() << "Frame is not valid!\n";
        return;
    }

    if(frametodraw.handleType() == QVideoFrame::NoHandle)
    {
        qDebug() << "Processing with mapped image";

        if(!frametodraw.map(QVideoFrame::ReadWrite))
        {
           qDebug() << "Failed to map frame!\n";
           return;
        }

        QImage image;
        QImage::Format imageFormat = QVideoFrameFormat::imageFormatFromPixelFormat(frametodraw.pixelFormat());
        if(imageFormat == QImage::Format_Invalid)
        {
            image = QImage(frametodraw.bits(0), frametodraw.width(), frametodraw.height(), QImage::Format_Grayscale8);
        }else
        {
            image = QImage(frametodraw.bits(0), frametodraw.width(), frametodraw.height(), imageFormat);
        }
        //Process image here

        cv::Mat cvImage = cvutils::QImageToCvMat(image.convertToFormat(QImage::Format_RGB32));

        auto font = cv::FONT_HERSHEY_SIMPLEX;
        cv::putText(cvImage, "Hello OpenCv", {40, 500}, font, 4, {255, 255, 122}, 2, cv::LINE_AA);

        QImage processedImage = cvutils::cvMatToQImage(cvImage);

        QPainter painter(&image);
        painter.drawImage(image.rect(), processedImage);
        painter.end();

        frametodraw.unmap();
    }else
    {
        qDebug() << "Your device is not supported";
        emit videoProcessError();
    }

    m_videoSink->setVideoFrame(frametodraw);
    qDebug() << "Frame process time: " << timer.elapsed();
}
