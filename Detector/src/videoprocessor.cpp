#include "videoprocessor.h"

#include <QImage>
#include <QPainter>
#include <QSize>
#include <QVideoFrame>
#include <QElapsedTimer>

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
    if(!frametodraw.map(QVideoFrame::ReadOnly))
    {
       qDebug() << "Failed to map frame!\n";
       return;
    }

    QImage image(frametodraw.bits(0), frametodraw.width(),frametodraw.height(), QImage::Format_Grayscale8);

    QPainter painter(&image);
    QFont font = painter.font();
    font.setPixelSize(32);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(image.rect(), Qt::AlignCenter, QDateTime::currentDateTime().toString());
    painter.end();

    frametodraw.unmap();
    m_videoSink->setVideoFrame(frametodraw);
    qDebug() << "Frame process time: " << timer.elapsed();
}
