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
    Q_PROPERTY(QImage imageToTrack READ imageToTrack WRITE setImageToTrack NOTIFY imageToTrackChanged)
public:
    QVideoFilterRunnable *createFilterRunnable()
    {
        return new GoodFeaturesToTrack(m_imageToTrack);
    }

    QImage imageToTrack()
    {
        if(m_imageToTrack)
            return *m_imageToTrack;
        else
            return QImage();
    }

    void setImageToTrack(const QImage& img)
    {
        m_imageToTrack = new QImage(img);
    }

public slots:
    void imageToTrackChanged(){}

signals:
    void finished(QObject *result);

private:
    QImage *m_imageToTrack = nullptr;
};

#endif // PROCESSVIDEOSURFACE_H
