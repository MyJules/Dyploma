#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QVideoSink>
#include <QQuickItem>
#include <QPointer>
#include <QTimer>

class VideoProcessor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
public:
    explicit VideoProcessor(QObject *parent = nullptr);

    QVideoSink* videoSink() const;
    void setVideoSink(QVideoSink* newVideoSink);
    Q_INVOKABLE void start();

signals:
    void videoSinkChanged();
    void videoProcessError() const;

private:
    void onVideoFrameChanged(const QVideoFrame& videoFrame) const;

    QPointer<QVideoSink> m_videoSink;
    QTimer m_timer;

};

#endif // VIDEOPROCESSOR_H
