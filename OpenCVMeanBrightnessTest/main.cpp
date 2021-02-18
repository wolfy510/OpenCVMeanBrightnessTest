#include <iostream>
#include <opencv2/opencv.hpp>
#include "HelperFunctions.h"


int main() {

    cv::Mat image = cv::imread("images/antonio.jpg", cv::IMREAD_COLOR);
    cv::Mat converted_image;
    std::vector<cv::ColorConversionCodes> image_formats = {cv::COLOR_RGB2RGBA, cv::COLOR_RGB2HSV, cv::COLOR_RGB2YUV,
                                                           cv::COLOR_RGB2YCrCb, cv::COLOR_RGB2Lab, cv::COLOR_RGB2Luv,
                                                           cv::COLOR_RGB2XYZ};
    double mean_brightness = 0.0;
    bool show_image = false;
    bool show_histogram = false;

    for (auto &image_format : image_formats) {
        if (cv::COLOR_RGB2RGBA == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            //SaveHistogram(converted_image, cv::COLOR_RGB2RGBA, 0, "RGB_histogram", show_histogram);
            SaveImage(converted_image, "RGB_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2RGBA, 0, 500, 500, 100, 100);
            std::cout << "RGB Mean brightness: " << mean_brightness << std::endl;
        } else if (cv::COLOR_RGB2HSV == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            SaveHistogram(converted_image, cv::COLOR_RGB2HSV, 2, "HSV_histogram", show_histogram);
            SaveImage(converted_image, "HSV_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2HSV, 2, 500, 500, 100, 100);
            std::cout << "HSV Mean brightness: " << mean_brightness << std::endl;
        } else if (cv::COLOR_RGB2YUV == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            SaveHistogram(converted_image, cv::COLOR_RGB2YUV, 0, "YUV_histogram", show_histogram);
            SaveImage(converted_image, "YUV_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2YUV, 0, 500, 500, 100, 100);
            std::cout << "YUV Mean brightness: " << mean_brightness << std::endl;
        } else if (cv::COLOR_RGB2YCrCb == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            SaveHistogram(converted_image, cv::COLOR_RGB2YCrCb, 0, "YCrCb_histogram", show_histogram);
            SaveImage(converted_image, "YCrCb_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2YCrCb, 0, 500, 500, 100, 100);
            std::cout << "YCrCb Mean brightness: " << mean_brightness << std::endl;
        } else if (cv::COLOR_RGB2Lab == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            SaveHistogram(converted_image, cv::COLOR_RGB2Lab, 0, "Lab_histogram", show_histogram);
            SaveImage(converted_image, "Lab_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2Lab, 0, 500, 500, 100, 100);
            std::cout << "Lab Mean brightness: " << mean_brightness << std::endl;
        } else if (cv::COLOR_RGB2Luv == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            SaveHistogram(converted_image, cv::COLOR_RGB2Luv, 0, "Luv_histogram", show_histogram);
            SaveImage(converted_image, "Luv_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2Luv, 0, 500, 500, 100, 100);
            std::cout << "Luv Mean brightness: " << mean_brightness << std::endl;
        } else if (cv::COLOR_RGB2XYZ == image_format) {
            image.copyTo(converted_image);
            ConvertImage(converted_image, image_format);
            SaveHistogram(converted_image, cv::COLOR_RGB2XYZ, 1, "XYZ_histogram", show_histogram);
            SaveImage(converted_image, "XYZ_image", show_image);

            mean_brightness = CalculateMeanBrightness(converted_image, cv::COLOR_RGB2XYZ, 1, 500, 500, 100, 100);
            std::cout << "XYZ Mean brightness: " << mean_brightness << std::endl;
        }
    }
    return 0;
}

