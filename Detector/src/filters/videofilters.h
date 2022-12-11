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
    QVideoFilterRunnable *createFilterRunnable() override;

signals:
    void finished(QObject *result);
    void updateImageToTrack();
    void resetImageToTrack();

public slots:
    void onNewImageToTrack();
    void onResetImageToTrack();
};

#endif // PROCESSVIDEOSURFACE_H
