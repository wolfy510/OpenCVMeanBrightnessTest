#include <iostream>
#include <opencv2/opencv.hpp>
#include "HelperFunctions.h"


int main() {
    std::string image_path = "images/raw_image";
    std::string input_format = ".raw";
    std::string full_image_path = image_path + input_format;

    bool show_image = false;
    bool show_histogram = false;
    bool save_image_and_histogram = true;

    int measures = 1;

    cv::Mat converted_image;
    std::vector<cv::ColorConversionCodes> image_formats = {cv::COLOR_RGB2RGBA, cv::COLOR_RGB2HSV, cv::COLOR_RGB2YUV, cv::COLOR_RGB2YCrCb, cv::COLOR_RGB2Lab, cv::COLOR_RGB2Luv, cv::COLOR_RGB2XYZ};
    std::vector<int>channels                            = {                 0,                 2,                 0,                   0,                 0,                 0,                 1};
    //                            width, height,  x0,  y0
    std::vector<int> roi_arguments = {1400, 200, 210, 600};

    cv::Mat image;
    if (".raw" == input_format) {
        ReadRawImage(image, full_image_path,show_image);
    } else {
        image = cv::imread(full_image_path, cv::IMREAD_UNCHANGED);
    }

    double tmp_brightness = 0.0;
    double tmp_duration_ms = 0.0;

    double RGB_brightness_sum = 0.0;
    double HSV_brightness_sum = 0.0;
    double YUV_brightness_sum = 0.0;
    double YCrCb_brightness_sum = 0.0;
    double Lab_brightness_sum = 0.0;
    double Luv_brightness_sum = 0.0;
    double XYZ_brightness_sum = 0.0;

    double RGB_duration_sum_ms = 0.0;
    double HSV_duration_sum_ms = 0.0;
    double YUV_duration_sum_ms = 0.0;
    double YCrCb_duration_sum_ms = 0.0;
    double Lab_duration_sum_ms = 0.0;
    double Luv_duration_sum_ms= 0.0;
    double XYZ_duration_sum_ms = 0.0;

    for (int i = 0; i < measures; i++) {
        for (auto &image_format : image_formats) {
            if (cv::COLOR_RGB2RGBA == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                // RGB is NOT converted to RGBA. This is just used to be able to make a distinction for the RGB image in main.cpp
                // ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2RGBA, channels.at(0), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                RGB_brightness_sum += tmp_brightness;
                RGB_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2RGBA, channels.at(0), "RGB_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2RGBA, channels.at(0), roi_arguments, "RGB_image", show_image);
                }
            } else if (cv::COLOR_RGB2HSV == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2HSV, channels.at(1), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                HSV_brightness_sum += tmp_brightness;
                HSV_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2HSV, channels.at(1), "HSV_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2HSV, channels.at(1), roi_arguments, "HSV_image", show_image);
                }
            } else if (cv::COLOR_RGB2YUV == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2YUV, channels.at(2), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                YUV_brightness_sum += tmp_brightness;
                YUV_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2YUV, channels.at(2), "YUV_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2YUV, channels.at(2), roi_arguments, "YUV_image", show_image);
                }
            } else if (cv::COLOR_RGB2YCrCb == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2YCrCb, channels.at(3), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                YCrCb_brightness_sum += tmp_brightness;
                YCrCb_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2YCrCb, channels.at(3), "YCrCb_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2YCrCb, channels.at(3), roi_arguments, "YCrCb_image", show_image);
                }
            } else if (cv::COLOR_RGB2Lab == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2Lab, channels.at(4), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                Lab_brightness_sum += tmp_brightness;
                Lab_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2Lab, channels.at(4), "Lab_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2Lab, channels.at(4), roi_arguments, "Lab_image", show_image);
                }
            } else if (cv::COLOR_RGB2Luv == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2Luv, channels.at(5), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                Luv_brightness_sum += tmp_brightness;
                Luv_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2Luv, channels.at(5), "Luv_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2Luv, channels.at(5), roi_arguments, "Luv_image", show_image);
                }
            } else if (cv::COLOR_RGB2XYZ == image_format) {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                image.copyTo(converted_image);
                ConvertImage(converted_image, image_format);
                CalculateMeanBrightness(converted_image, cv::COLOR_RGB2XYZ, channels.at(6), roi_arguments, tmp_brightness, tmp_duration_ms);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                XYZ_brightness_sum += tmp_brightness;
                XYZ_duration_sum_ms += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;
                tmp_duration_ms = 0.0;

                if (save_image_and_histogram) {
                    SaveHistogram(converted_image, cv::COLOR_RGB2XYZ, channels.at(6), "XYZ_histogram", show_histogram);
                    SaveImage(converted_image, cv::COLOR_RGB2XYZ, channels.at(6), roi_arguments, "XYZ_image", show_image);
                }
            }
        }
    }

    std::cout << "RGB:\t " << RGB_brightness_sum / measures << ", " << RGB_duration_sum_ms / measures << " ms" << std::endl;
    std::cout << "HSV:\t " << HSV_brightness_sum / measures << ", " << HSV_duration_sum_ms / measures << " ms" << std::endl;
    std::cout << "YUV:\t " << YUV_brightness_sum / measures << ", " << YUV_duration_sum_ms / measures << " ms" << std::endl;
    std::cout << "YCrCb:\t " << YCrCb_brightness_sum / measures << ", " << YCrCb_duration_sum_ms / measures << " ms" << std::endl;
    std::cout << "Lab:\t " << Lab_brightness_sum / measures << ", " << Lab_duration_sum_ms / measures << " ms" << std::endl;
    std::cout << "Luv:\t " << Luv_brightness_sum / measures << ", " << Luv_duration_sum_ms / measures << " ms" << std::endl;
    std::cout << "XYZ:\t " << XYZ_brightness_sum / measures << ", " << XYZ_duration_sum_ms / measures << " ms" << std::endl;
    return 0;
}

