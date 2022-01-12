#ifndef VPXDECODER_H
#define VPXDECODER_H

#include <QOpenGLWidget>

class VPxDecoder : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit VPxDecoder(QWidget *parent = nullptr);
    ~VPxDecoder() {}
signals:

public slots:
};

#endif // VPXDECODER_H
