#include <iostream>
#include <opencv2/opencv.hpp>
#include "HelperFunctions.h"


int main() {
    std::string image_path = "images/road_image";
    std::string input_format = ".raw";
    std::string full_image_path = image_path + input_format;

    bool show_raw_image = false;
    bool show_all_images = false;
    bool show_all_histograms = false;
    bool save_images_and_histograms = false;
    bool measure_only_step_4 = false;
    int measures = 1000;

    cv::Mat converted_image;
    std::vector<std::string> color_space_strings = {"RGB", "HSV", "YUV", "YCrCb", "Lab", "Luv", "XYZ", "GS"};
    std::vector<cv::ColorConversionCodes> image_formats = {cv::COLOR_RGB2RGBA, cv::COLOR_RGB2HSV, cv::COLOR_RGB2YUV, cv::COLOR_RGB2YCrCb, cv::COLOR_RGB2Lab, cv::COLOR_RGB2Luv, cv::COLOR_RGB2XYZ, cv::COLOR_RGB2GRAY};
    std::vector<int>channels                            = {                 0,                 2,                 0,                   0,                 0,                 0,                 1,                  0};
    //                               width,    height,      x0,        y0
    //                         ROI:   1100,       200,     310,        600
    //               Effective FOV:   1400,       900,     210,        10
    std::vector<int> roi_arguments = {1100,       200,     310,        600};
    // Step 1 - read image
    cv::Mat image;
    if (".raw" == input_format) {
        ReadRawImage(image, full_image_path, show_raw_image);
    } else {
        image = cv::imread(full_image_path, cv::IMREAD_UNCHANGED);
    }
    double tmp_brightness = 0.0;

    std::vector<double> brightness_sum_list = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::vector<double> duration_sum_ms_list = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    for (int i = 0; i < measures; i++) {
        if (i % 10 == 0) {
            std::cout << "Progress: " << i << std::endl;
        }
        int j = 0;
        for (auto &image_format : image_formats) {
            if (measure_only_step_4) {
                // Step 2 - copy image
                image.copyTo(converted_image);
                if (cv::COLOR_RGB2RGBA == image_format) {
                    // RGB is NOT converted to RGBA. This is just used to be able to make a distinction for the RGB image in this test
                } else {
                    // Step 3 - convert image
                    ConvertImage(converted_image, image_format);
                }
                // Step 4 - calculate mean brightness of image
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                CalculateMeanBrightness(converted_image, image_format, channels.at(j), roi_arguments, tmp_brightness);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                brightness_sum_list.at(j) += tmp_brightness;
                duration_sum_ms_list.at(j) += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;

                if (save_images_and_histograms) {
                    SaveHistogram(converted_image, image_format, channels.at(j), color_space_strings.at(j) + "_histogram", show_all_histograms);
                    SaveImage(converted_image, image_format, channels.at(j), roi_arguments, color_space_strings.at(j) + "_image", show_all_images);
                }
            } else {
                auto start_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                // Step 2 - copy image
                image.copyTo(converted_image);
                if (cv::COLOR_RGB2RGBA == image_format) {
                    // RGB is NOT converted to RGBA. This is just used to be able to make a distinction for the RGB image in this test
                } else {
                    // Step 3 - convert image
                    ConvertImage(converted_image, image_format);
                }
                // Step 4 - calculate mean brightness of image
                CalculateMeanBrightness(converted_image, image_format, channels.at(j), roi_arguments, tmp_brightness);
                auto end_timestamp_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                brightness_sum_list.at(j) += tmp_brightness;
                duration_sum_ms_list.at(j) += static_cast<double>(end_timestamp_ms - start_timestamp_ms) / 1000000.0;
                tmp_brightness = 0.0;

                if (save_images_and_histograms) {
                    SaveHistogram(converted_image, image_format, channels.at(j), color_space_strings.at(j) + "_histogram", show_all_histograms);
                    SaveImage(converted_image, image_format, channels.at(j), roi_arguments, color_space_strings.at(j) + "_image", show_all_images);
                }
            }
            j++;
        }
    }
    PrintResults(brightness_sum_list, duration_sum_ms_list, measures);
    return 0;
}

