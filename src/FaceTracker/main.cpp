#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "DroneController.h"

int maxRectArea(std::vector<cv::Rect>& rects) {
    if (rects.empty()) return -1;
    int max_area = 0;
    int max_index = 0;
    for(int i = 0; i < rects.size(); i++) {
        if(rects[i].area() > max_area) {
            max_area = rects[i].area();
            max_index = i;
        }
    }
    return max_index;
}

int main() {
    // Import the necessary libraries.
    cv::CascadeClassifier face_cascade("../haarcascade_frontalface_alt.xml");

    // Initialize control of the camera
    DroneController controller("/dev/tty.usbmodem3485187B55B02", 115200);
    int pan_angle = 0;
    int tilt_angle = 0;

    // Open the video file (change "path_to_video" to your video file's path)
    cv::VideoCapture cap(0);

    // Check if the video opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error opening video file" << std::endl;
        return -1;
    }

    // Get the frames' width and height from the video
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int mid_x = width / 2;
    int mid_y = height / 2;

    // Create a window to display the video
    cv::namedWindow("Video Preview", cv::WINDOW_NORMAL);
    cv::resizeWindow("Video Preview", width, height);

    // Loop to read frames from the video and display them in the window
    while (true) {
        cv::Mat frame;
        // Read a frame from the video
        if (!cap.read(frame)) {
            // If the frame cannot be read (end of video), break the loop
            break;
        }

        // Detect faces in the frame using the CascadeClassifier object.
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        // Draw a rectangle around the largest detected face
        int face_index = maxRectArea(faces);
        if (face_index != -1) {
            cv::Rect face = faces[face_index];
            cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);

            // update pan/tilt position
            int x = face.x + (face.width / 2);
            int y = face.y + (face.height / 2);
            if (abs(mid_x - x) > 250) {
                pan_angle += (mid_x < x) ? -1 : 1;
                controller.setPanAngle(pan_angle);
            }
            if (abs(mid_y - y) > 250) {
                tilt_angle += (mid_y < y) ? -1 : 1;
                controller.setTiltAngle(tilt_angle);
            }
        }

        // Display the frame in the window
        cv::imshow("Video Preview", frame);

        // Exit the loop if the 'Esc' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Release the video capture object and close the window
    cap.release();
    cv::destroyAllWindows();

    return 0;
}