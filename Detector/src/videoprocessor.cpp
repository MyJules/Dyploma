#include "videoprocessor.h"

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
        qDebug() << "No Handle";

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
        painter.setPen(Qt::red);
        painter.drawText(image.rect(), Qt::AlignCenter, QDateTime::currentDateTime().toString());
        painter.end();

        frametodraw.unmap();
    }else
    {
        qDebug() << "Normal processing";

        //        QImage image( videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32 );
        //        GLuint textureId = static_cast<GLuint>( videoFrame.videoBuffer()->handle().toInt() );
        //        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        //        QOpenGLFunctions* f = ctx->functions();
        //        GLuint fbo;
        //        f->glGenFramebuffers( 1, &fbo );
        //        GLint prevFbo;
        //        f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFbo );
        //        f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
        //        f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
        //        f->glReadPixels( 0, 0,  videoFrame.width(),  videoFrame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
        //        f->glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );
    }

    m_videoSink->setVideoFrame(frametodraw);
    qDebug() << "Frame process time: " << timer.elapsed();
}
