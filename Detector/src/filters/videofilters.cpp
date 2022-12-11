#include "videofilters.h"

QVideoFilterRunnable *GoodFeaturesToTrackFilter::createFilterRunnable()
{
    GoodFeaturesToTrack *filter = new GoodFeaturesToTrack();
    connect(this, &GoodFeaturesToTrackFilter::updateImageToTrack, this, [filter](){
        filter->onUpdateImageToTrack();
    });

    connect(this, &GoodFeaturesToTrackFilter::resetImageToTrack, this, [filter](){
        filter->onResetImageToTrack();
    });
    return filter;
}

void GoodFeaturesToTrackFilter::onNewImageToTrack()
{
    emit updateImageToTrack();
}

void GoodFeaturesToTrackFilter::onResetImageToTrack()
{
    emit resetImageToTrack();
}
