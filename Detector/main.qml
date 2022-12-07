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

    GoodFeaturesToTrackFilter {
        id: goodFeaturesToTrackFilter
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: 0

        onCurrentIndexChanged: {
            handleCameraSwitch()
            handleFilterDetectorSwitch()
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
                          //preview
                          imageToTrack.source = preview
                          goodFeaturesToTrackFilter.onNewImageToTrack();
                      }
                  }
              }

              VideoOutput {
                  id: videoOutput
                  source: camera
                  anchors.fill: parent
                  focus : visible
                  autoOrientation: true
                  fillMode: VideoOutput.PreserveAspectFit
                  filters: [ goodFeaturesToTrackFilter ]
              }

              Text {
                  id: detectorText
                  text:qsTr("Detector")
                  font.pointSize: 20
                  anchors.top: parent.top
                  anchors.right: parent.right
                  anchors.topMargin: 20
                  anchors.rightMargin: 40
              }

              Image {
                  id: imageToTrack
                  source: "image:"

                  fillMode: Image.PreserveAspectFit

                  width: 240
                  height: 120

                  anchors.top: parent.top
                  anchors.left: parent.left
                  anchors.bottomMargin: 10
                  anchors.rightMargin: 10
                  anchors.topMargin: 10
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
                        console.log("Capture image to track")
                        camera.imageCapture.capture();
                     }
                  }

                  Button {
                    id: resetButton

                    text: qsTr("Reset")
                    onClicked: {
                        console.log("Reset image to track")
                        imageToTrack.source = "image:"
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
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 20
                anchors.rightMargin: 40

            }

            Row {
                id: row
                spacing: 80
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                Tumbler {
                    id: filterTumbler
                    scale: 1.5
                    model: ["GFTT", "Harris"]
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

    function handleCameraSwitch() {
        if(swipeView.currentIndex === 0){
            camera.viewfinder.resolution = resolutionTumbler.model[resolutionTumbler.currentIndex]
            camera.start();
        }else{
            camera.stop();
        }
    }

    function handleFilterDetectorSwitch() {
        if(swipeView.currentIndex === 1) return

        let currentFilter = filterTumbler.model[filterTumbler.currentIndex]
        switch (currentFilter) {
            case 'Harris':
                console.log("Handle harris")
                videoOutput.filters = []

            break;

            case 'GFTT':
                console.log("Handle GFTT")
                videoOutput.filters = [goodFeaturesToTrackFilter]

            break;

            default:
                console.log("Error: unhandled tumbler state")
            break;
        }
    }
}
