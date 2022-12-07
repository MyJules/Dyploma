#ifndef GOODFEATURESTOTRACK_H
#define GOODFEATURESTOTRACK_H

#include <filters/ivideofilter.h>

class GoodFeaturesToTrack final : public IVideoFilter
{
public:
    explicit GoodFeaturesToTrack(QImage* image);
    [[nodiscard]] virtual cv::Mat process(const cv::Mat&);

private:
    QImage *m_imageToTrack;
};

#endif // GOODFEATURESTOTRACK_H
