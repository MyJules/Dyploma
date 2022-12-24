#include "videofilters.h"

QVideoFilterRunnable *SIFTFilter::createFilterRunnable()
{
    SIFT *filter = new SIFT();
    connect(this, &SIFTFilter::updateImageToTrack, this, [filter](){
        filter->onUpdateImageToTrack();
    });

    connect(this, &SIFTFilter::resetImageToTrack, this, [filter](){
        filter->onResetImageToTrack();
    });
    return filter;
}

void SIFTFilter::onNewImageToTrack()
{
    emit updateImageToTrack();
}

void SIFTFilter::onResetImageToTrack()
{
    emit resetImageToTrack();
}
