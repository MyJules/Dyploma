import QtQuick 2.4
import QtMultimedia
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

import Detector

ApplicationWindow {
    visible: true
    id: root
    width: 600
    height: 400
    title: "Detector"

    Material.theme: Material.Dark
    Material.accent: Material.Indigo

    VideoProcessor{
       id: videoProcessor
       videoSink: videoOutput.videoSink
    }

    CaptureSession {
        id: captureSession
        camera: camera
        videoOutput: videoOutput
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

    MediaDevices {
        id: mediaDevices
    }

    Camera {
        id: camera
        cameraDevice: mediaDevices.defaultVideoInput
        active: true
    }

    Component.onCompleted: videoProcessor.start()
}
