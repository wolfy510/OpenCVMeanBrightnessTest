//
// Created by Antonio on 18.02.2021..
//

#ifndef OPENCVMEANBRIGHTNESSTEST_HELPERFUNCTIONS_H
#define OPENCVMEANBRIGHTNESSTEST_HELPERFUNCTIONS_H

#include <cstdio>

void DefineRegionOfInterest() {

}

void SaveImage(cv::Mat& image, const std::string& image_name, bool show_image) {

    std::string file_name = image_name + ".jpg";
    if (!cv::imread(file_name, cv::IMREAD_ANYCOLOR).empty()) {
        std::remove(file_name.c_str());
    }
    cv::imwrite("images/" + image_name + ".jpg", image);

    if (show_image) {
        if (image.empty()) {
            std::cerr << "SaveImage: Image not found!" << std::endl;
            return;
        }
        cv::imshow(image_name, image);
        cv::waitKey(0);
    }
}

void SaveHistogram(cv::Mat& image, cv::ColorConversionCodes color_space, int channel, const std::string& image_name, bool show_histogram) {
    if (image.empty()) {
        std::cerr << "SaveHistogram(): Image not found!" << std::endl;
        return;
    }

    std::vector<cv::Mat> channels;
    split(image, channels);
    int hist_size = 256;
    float range[] = {0, 256}; //the upper boundary is exclusive
    const float *hist_range = {range};
    bool uniform = true, accumulate = false;
    cv::Mat luminosity_hist;

    if (cv::COLOR_RGB2RGBA == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
        //calcHist(&channels[0], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
        //calcHist(&channels[1], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
        //calcHist(&channels[2], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    } else if (cv::COLOR_RGB2HSV == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    } else if (cv::COLOR_RGB2YUV == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    } else if (cv::COLOR_RGB2YCrCb == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    } else if (cv::COLOR_RGB2Lab == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    } else if (cv::COLOR_RGB2Luv == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    } else if (cv::COLOR_RGB2XYZ == color_space) {
        calcHist(&channels[channel], 1, 0, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
    }

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/hist_size );
    cv::Mat hist_image(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0) );

    normalize(luminosity_hist, luminosity_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    for( int i = 1; i < hist_size; i++ ) {
        line(hist_image, cv::Point(bin_w * (i - 1), hist_h - cvRound(luminosity_hist.at<float>(i - 1)) ),
             cv::Point( bin_w*(i), hist_h - cvRound(luminosity_hist.at<float>(i)) ),
             cv::Scalar( 255, 0, 0), 2, 8, 0  );
    }

    std::string file_name = image_name + ".jpg";
    if (!cv::imread(file_name, cv::IMREAD_ANYCOLOR).empty()) {
        std::remove(file_name.c_str());
    }
    cv::imwrite("images/" + image_name + ".jpg", hist_image);

    if (show_histogram) {
        if (hist_image.empty()) {
            std::cerr << "SaveHistogram(): Image not found!" << std::endl;
            return;
        }

        cv::imshow(image_name, hist_image);
        cv::waitKey(0);
    }
}

double CalculateMeanBrightness(cv::Mat& image, cv::ColorConversionCodes color_space, int channel, const int roi_width, const int roi_height, const int x0, const int y0) {
    double current_value = 0.0;
    double sum = 0.0;
    double mean_brightness = 0.0;

    int rows = image.rows;
    int columns = image.cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if (cv::COLOR_RGB2RGBA == color_space) {
                current_value = 0.2126 * image.at<cv::Vec3b>(r, c)[0];
                current_value += 0.7152 * image.at<cv::Vec3b>(r, c)[1];
                current_value += 0.0722 * image.at<cv::Vec3b>(r, c)[2];
                sum += current_value;
            } else {
                current_value = image.at<cv::Vec3b>(r, c)[channel];
                sum += current_value;
            }
        }
    }

    mean_brightness = sum / (rows * columns);

    return mean_brightness;
}

void ConvertImage(cv::Mat& image, cv::ColorConversionCodes new_color_space) {
    if (cv::COLOR_RGB2RGBA == new_color_space) {
        return;
    } else {
        cv::cvtColor(image, image, new_color_space);
    }

}



#endif //OPENCVMEANBRIGHTNESSTEST_HELPERFUNCTIONS_H
