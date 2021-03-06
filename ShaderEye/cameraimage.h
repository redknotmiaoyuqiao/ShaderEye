﻿#ifndef CAMERAIMAGE_H
#define CAMERAIMAGE_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QCamera>


#include <QFile>
class CameraImage:public QAbstractVideoSurface
{
     Q_OBJECT
public:
    /*
     *  支持的video frame  像素格式
     */
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    explicit CameraImage(QObject *parent = 0);
    /*
     *  从present 通知
     */
    void setVideoFrame(const QVideoFrame &frame);
    /*
     *  设置视频源
     */
    void setSource(QCamera *pCamera);
    /*
     *  override QAbstractVideoSurface 中的绘制frame方法
     */
    bool present(const QVideoFrame &frame) override;


signals:
    //视频帧捕获信号
    void CaptureFrame(const QVideoFrame &frame);
private:
    // todo
};

#endif // CAMERAIMAGE_H
