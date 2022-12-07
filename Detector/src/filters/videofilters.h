#ifndef PROCESSVIDEOSURFACE_H
#define PROCESSVIDEOSURFACE_H

#include <filters/ivideofilter.h>
#include <filters/goodfeaturestotrack.h>

#include <opencv2/core.hpp>

#include <QtQml/qqml.h>

class GoodFeaturesToTrackFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    QML_ELEMENT

public:
    QVideoFilterRunnable *createFilterRunnable()
    {
        GoodFeaturesToTrack *filter = new GoodFeaturesToTrack();
        connect(this, &GoodFeaturesToTrackFilter::updateImageToTrack, this, [filter](){
            filter->onUpdateImageToTrack();
        });
        return filter;
    }

signals:
    void finished(QObject *result);
    void updateImageToTrack();

public slots:
    void onNewImageToTrack()
    {
        emit updateImageToTrack();
    }
};

#endif // PROCESSVIDEOSURFACE_H
