#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

// Load a grayscale image
cv::Mat loadImage(const std::string& imageFile) {
    cv::Mat img = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Couldn't open image " << imageFile << ".\n";
        exit(-1);
    }
    return img;
}

// Generate grayscale noise frames
std::vector<cv::Mat> generateNoiseFrames(int width, int height, int numFrames) {
    std::vector<cv::Mat> noiseFrames;
    for (int i = 0; i < numFrames; ++i) {
        cv::Mat noise(height, width, CV_8UC1);
        cv::randu(noise, cv::Scalar(0), cv::Scalar(255));
        noiseFrames.push_back(noise);
    }
    return noiseFrames;
}

int main() {
    std::string imageFile1 = "/home/jim/Desktop/PiTests/images/image.jpg"; // Path to your first image file
    std::string imageFile2 = "/home/jim/Desktop/PiTests/images/image2.jpg"; // Path to your second image file

    cv::Mat img1 = loadImage(imageFile1);
    cv::Mat img2 = loadImage(imageFile2);

    const int numNoiseFrames = 30;
    std::vector<cv::Mat> noiseFrames = generateNoiseFrames(img1.cols, img1.rows, numNoiseFrames);

    cv::namedWindow("Blended Image Playback", cv::WINDOW_AUTOSIZE);
    int noiseFrameIndex = 0;

    while (true) {
        auto loopStartTime = std::chrono::steady_clock::now();

        // Use pre-generated noise frame
        cv::Mat noise = noiseFrames[noiseFrameIndex];
        noiseFrameIndex = (noiseFrameIndex + 1) % numNoiseFrames;

        // Blend image1, image2, and noise (1/3 each)
        cv::Mat blendedImg1, blendedImg2, finalBlendedImg;
        cv::addWeighted(img1, 1.0/3.0, noise, 1.0/3.0, 0.0, blendedImg1);
        cv::addWeighted(blendedImg1, 2.0/3.0, img2, 1.0/3.0, 0.0, finalBlendedImg);

        cv::imshow("Blended Image Playback", finalBlendedImg);

        // Check for escape key press
        int key = cv::waitKey(1);
        if (key == 27) { // ASCII code for the escape key
            break;
        }

        auto loopEndTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = loopEndTime - loopStartTime;
        std::cout << "Loop duration: " << elapsed_seconds.count() << "s\n";
    }

    return 0;
}
