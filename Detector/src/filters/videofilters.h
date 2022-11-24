#ifndef PROCESSVIDEOSURFACE_H
#define PROCESSVIDEOSURFACE_H

#include <filters/ivideofilter.h>
#include <filters/goodfeaturestotrack.h>

#include <opencv2/core.hpp>

class GoodFeaturesToTrackFilter : public QAbstractVideoFilter
{
public:
    QVideoFilterRunnable *createFilterRunnable() { return new GoodFeaturesToTrack; }

signals:
    void finished(QObject *result);
};

#endif // PROCESSVIDEOSURFACE_H
