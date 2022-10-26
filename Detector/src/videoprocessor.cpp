#include "videoprocessor.h"

#include "cvutils.h"

#include <opencv2/core.hpp>
#include <QImage>
#include <QPainter>
#include <QSize>
#include <QVideoFrame>
#include <QElapsedTimer>
#include <QtConcurrent>

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
#ifdef Q_OS_ANDROID
    m_videoSink->setRhi(nullptr);
#endif
    connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &VideoProcessor::onVideoFrameChanged, Qt::ConnectionType::DirectConnection);
}

void VideoProcessor::onVideoFrameChanged(const QVideoFrame &videoFrame)
{
    if (!static_cast<QVideoFrame>(videoFrame).isValid())
    {
        qDebug() << "Frame is not valid!\n";
        return;
    }

#ifdef Q_OS_ANDROID
    m_videoSink->setRhi(nullptr);
#endif
    if(!static_cast<QVideoFrame>(videoFrame).map(QVideoFrame::ReadOnly))
    {
       qDebug() << "Failed to map frame!\n";
       return;
    }

    QImage image = videoFrame.toImage();

    if(videoFrame.isMapped())
    {
        static_cast<QVideoFrame>(videoFrame).unmap();
    }

    //Process image here
    cv::Mat cvImage = cvutils::QImageToCvMat(image);

    auto font = cv::FONT_HERSHEY_SIMPLEX;
    cv::putText(cvImage, "Hello OpenCv", {40, 500}, font, 4, {255, 255, 122}, 2, cv::LINE_AA);

    QImage processedImage = cvutils::cvMatToQImage(cvImage);

    QPainter painter(&image);
    painter.drawImage(image.rect(), processedImage);
    painter.drawRect(QRect(80,120,200,100));
    painter.end();
}
