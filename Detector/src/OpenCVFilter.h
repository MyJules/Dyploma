#ifndef PROCESSVIDEOSURFACE_H
#define PROCESSVIDEOSURFACE_H

#include <QQuickItem>
#include <QtMultimedia>
#include <QElapsedTimer>

class TestFilter : public QVideoFilterRunnable
{
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};

class OpenCVFilter : public QAbstractVideoFilter
{
public:
    QVideoFilterRunnable *createFilterRunnable() { return new TestFilter; }

signals:
    void finished(QObject *result);
};

#endif // PROCESSVIDEOSURFACE_H
