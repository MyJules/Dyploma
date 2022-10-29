import QtQuick 2.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import com.video.process 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Detector")

    Material.theme: Material.Dark
    Material.accent: Material.Orange

    OpenCVFilter {
        id: openCVFilter
    }

    Camera {
          id: camera

          position: Camera.BackFace

          imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

          viewfinder.resolution: "1280x720"

          exposure {
              exposureCompensation: -1.0
              exposureMode: Camera.ExposurePortrait
          }

          flash.mode: Camera.FlashRedEyeReduction

          imageCapture {
              onImageCaptured: {
                  photoPreview.source = preview  // Show the preview in an Image
              }
          }
      }

      VideoOutput {
          source: camera
          anchors.fill: parent
          focus : visible
          autoOrientation: true
          fillMode: VideoOutput.PreserveAspectFit
          filters: [ openCVFilter ]
      }

      Button {
        id: switchCameraButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        padding: 20

        property bool frontFace: false

        text: qsTr("Switch camera")
        onClicked: {
            frontFace = !frontFace
            camera.position = frontFace ? Camera.FrontFace : Camera.BackFace
        }
     }
}
