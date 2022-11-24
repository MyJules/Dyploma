#ifndef GOODFEATURESTOTRACK_H
#define GOODFEATURESTOTRACK_H

#include <filters/ivideofilter.h>

class GoodFeaturesToTrack final : public IVideoFilter
{
public:
    [[nodiscard]] virtual cv::Mat process(const cv::Mat&);
};

#endif // GOODFEATURESTOTRACK_H
