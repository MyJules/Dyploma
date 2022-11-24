#ifndef PROCESSVIDEOSURFACE_H
#define PROCESSVIDEOSURFACE_H

#include <opencv2/core.hpp>

#include <QQuickItem>
#include <QtMultimedia>
#include <QElapsedTimer>

class IVideoFilter : public QVideoFilterRunnable
{
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
    [[nodiscard]] virtual cv::Mat process(const cv::Mat&) = 0;
};

class GoodFeaturesToTrack final : public IVideoFilter
{
public:
    [[nodiscard]] cv::Mat process(const cv::Mat&) override;
};

class OpenCVFilter : public QAbstractVideoFilter
{
public:
    QVideoFilterRunnable *createFilterRunnable() { return new GoodFeaturesToTrack; }

signals:
    void finished(QObject *result);
};

#endif // PROCESSVIDEOSURFACE_H
