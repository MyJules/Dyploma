#ifndef GOODFEATURESTOTRACK_H
#define GOODFEATURESTOTRACK_H

#include <filters/ivideofilter.h>

#include <optional>

class GoodFeaturesToTrack final : public IVideoFilter
{
public:
    explicit GoodFeaturesToTrack();
    ~GoodFeaturesToTrack();
    [[nodiscard]] virtual cv::Mat process(const cv::Mat&);

public slots:
    void onUpdateImageToTrack();
    void onResetImageToTrack();

private:
    bool m_isNewImageToTrack = false;
    std::optional<cv::Mat> m_imageToTrack;
};

#endif // GOODFEATURESTOTRACK_H
