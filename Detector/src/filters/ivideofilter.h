#ifndef IVIDEOFILTER_H
#define IVIDEOFILTER_H

#include <opencv2/core.hpp>

#include <QtMultimedia>

class IVideoFilter : public QVideoFilterRunnable
{
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
    [[nodiscard]] virtual cv::Mat process(const cv::Mat&) = 0;
};

#endif // IVIDEOFILTER_H
