import QtQuick 2.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import com.video.process 1.0

ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Detector")

    Material.theme: Material.Light
    Material.accent: Material.Orange

    OpenCVFilter {
        id: openCVFilter
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: 0

        onCurrentIndexChanged: {
            if(swipeView.currentIndex === 0){
                camera.viewfinder.resolution = resolutionTumbler.model[resolutionTumbler.currentIndex]
                camera.start();
            }else{
                camera.stop();
            }
        }

        Item {
            id: firstPage

            Camera {
                  id: camera

                  position: Camera.BackFace

                  imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

                  viewfinder.resolution: "640x480"

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

              Text {
                  id: detectorText
                  text:qsTr("Detector")
                  font.pointSize: 20
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.topMargin: 20

              }


              Row {
                  id: column
                  anchors.bottom: parent.bottom
                  anchors.right: parent.right
                  spacing: 4
                  anchors.bottomMargin: 15
                  anchors.rightMargin: 15

                  Button {
                    id: switchCameraButton

                    property bool frontFace: false

                    text: qsTr("Switch camera")
                    onClicked: {
                        frontFace = !frontFace
                        camera.position = frontFace ? Camera.FrontFace : Camera.BackFace
                     }
                  }

                  Button {
                    id: trackButton

                    text: qsTr("Track")
                    onClicked: {
                        console.log("Start tracking")
                     }
                  }
             }
        }

        Item {
            id: secondPage

            Text {
                id: optionText
                text:qsTr("Options")
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20

            }

            Row {
                id: row
                spacing: 80
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                Tumbler {
                    id: trackTumbler
                    scale: 1.5
                    model: ["BRIEF", "ORB"]
                    wrap: true
                    font.pointSize: 8
                }
                Tumbler {
                    id: filterTumbler
                    scale: 1.5
                    model: ["Harris", "GFTT"]
                    wrap: true
                    font.pointSize: 8
                }
                Tumbler {
                    id: resolutionTumbler
                    scale: 1.5
                    model: ["640x480", "1280x720"]
                    wrap: true
                    font.pointSize: 8
                }
            }
        }
    }

    PageIndicator {
        id: indicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
