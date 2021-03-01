//
// Created by Antonio on 18.02.2021..
//

#ifndef OPENCVMEANBRIGHTNESSTEST_HELPERFUNCTIONS_H
#define OPENCVMEANBRIGHTNESSTEST_HELPERFUNCTIONS_H



#include <cstdio>
#include <chrono>

void SaveImage(cv::Mat& img, cv::ColorConversionCodes color_space, int channel, const std::vector<int>& roi_arguments, const std::string& image_name, bool show_image) {
    if (img.empty()) {
        std::cout << "SaveImage(): Image not found!" << std::endl;
        return;
    }

    cv::Mat image;
    img.copyTo(image);

    if (cv::COLOR_RGB2RGBA == color_space) {
        cv::Mat tmp_image;
        image.copyTo(tmp_image);
    } else {
        std::vector<cv::Mat> channels;
        split(image, channels);
        cv::cvtColor(channels[channel], image, cv::COLOR_GRAY2RGB);
    }
    // ROI arguments
    int width = roi_arguments.at(0);
    int height = roi_arguments.at(1);
    int x0 = roi_arguments.at(2);
    int y0 = roi_arguments.at(3);

    // Top left corner
    cv::Point pt1(x0, y0);
    // Bottom right corner
    cv::Point pt2(x0 + width, y0 + height);

    cv::rectangle(image, pt1, pt2, cv::Scalar(0, 0, 255), 5);

    std::string file_name = image_name + ".jpg";
    if (!cv::imread(file_name, cv::IMREAD_UNCHANGED).empty()) {
        std::remove(file_name.c_str());
    }

    cv::imwrite("images/dump/" + image_name + ".jpg", image);
    if (show_image) {
        if (image.empty()) {
            std::cerr << "SaveImage: Image not found!" << std::endl;
            return;
        }
        cv::imshow(image_name, image);
        cv::waitKey(0);
    }
}

void SaveHistogram(cv::Mat& img, cv::ColorConversionCodes color_space, int channel, const std::string& image_name, bool show_histogram) {
    if (img.empty()) {
        std::cerr << "SaveHistogram(): Image not found!" << std::endl;
        return;
    }

    cv::Mat image;
    img.copyTo(image);

    std::vector<cv::Mat> channels;
    int rgb_channels[] = {0, 1, 2};
    split(image, channels);
    int hist_size = 256;
    float range[] = {0, 256}; // The upper boundary is exclusive
    const float* hist_range = {range};
    bool uniform = true, accumulate = false;
    cv::Mat luminosity_hist;

    if (cv::COLOR_RGB2RGBA == color_space) {
        calcHist(&image, 1, rgb_channels, cv::Mat(), luminosity_hist, 1, &hist_size, &hist_range, uniform, accumulate );
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
             cv::Scalar( 0, 0, 255), 2, 8, 0  );
    }

    std::string file_name = image_name + ".jpg";
    if (!cv::imread(file_name, cv::IMREAD_UNCHANGED).empty()) {
        std::remove(file_name.c_str());
    }
    cv::imwrite("images/dump/" + image_name + ".jpg", hist_image);

    if (show_histogram) {
        if (hist_image.empty()) {
            std::cerr << "SaveHistogram(): Image not found!" << std::endl;
            return;
        }

        cv::imshow(image_name, hist_image);
        cv::waitKey(0);
    }
}

void CalculateMeanBrightness(cv::Mat& img, cv::ColorConversionCodes color_space, int channel, const std::vector<int>& roi_arguments, double& mean_brightness) {
    if (img.empty()) {
        std::cerr << "SaveHistogram(): Image not found!" << std::endl;
        return;
    }

    //cv::Mat image;
    //img.copyTo(image);

    double current_value = 0.0;
    double sum = 0.0;
    mean_brightness = 0.0;

    // ROI arguments
    int width = roi_arguments.at(0);
    int height = roi_arguments.at(1);
    int x0 = roi_arguments.at(2);
    int y0 = roi_arguments.at(3);

    cv::Mat roi_image;
    img(cv::Rect(x0, y0, width, height)).copyTo(roi_image);

    int rows = roi_image.rows;
    int columns = roi_image.cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if (cv::COLOR_RGB2RGBA == color_space) {
                current_value = 0.2126 * roi_image.at<cv::Vec3b>(r, c)[0];
                current_value += 0.7152 * roi_image.at<cv::Vec3b>(r, c)[1];
                current_value += 0.0722 * roi_image.at<cv::Vec3b>(r, c)[2];
                sum += current_value;
            } else {
                current_value = roi_image.at<cv::Vec3b>(r, c)[channel];
                sum += current_value;
            }
        }
    }

    mean_brightness = sum / (rows * columns);
}

void ConvertImage(cv::Mat& img, cv::ColorConversionCodes new_color_space) {
    cv::cvtColor(img, img, new_color_space);
}

void ReadRawImage(cv::Mat& img, const std::string& full_image_path, bool show_image) {
    FILE *fp = nullptr;
    errno_t err;
    char *imagedata = nullptr;
    int image_width = 1920;
    int image_height = 1280;
    int framesize = image_width * image_height;

    if( 0 != (err = fopen_s(&fp, full_image_path.c_str(), "rb"))) {
        std::cerr << "ERROR: " << err << ". Colud not open file!" << std::endl;
        return;
    }

    imagedata = (char*) malloc (sizeof(uchar) * framesize * 2);
    fread(imagedata, sizeof(char) * 2, framesize, fp);
    img.create(image_height, image_width, CV_16U);
    memcpy(img.data, imagedata, framesize * 2);
    free(imagedata);
    fclose(fp);

    if (show_image) {
        if (img.empty()) {
            std::cerr << "ReadRawImage: Image not found!" << std::endl;
            return;
        }
        cv::imshow("RAW Image", img);
        cv::waitKey(0);
    }

    cv::cvtColor(img, img, cv::COLOR_BayerBG2RGB);
    img.convertTo(img, CV_8U, 1 / 256.0);
}

void PrintResults(const std::vector<double>& brightness_sum_list, const std::vector<double>& duration_sum_ms_list, const int& measures) {
    std::cout << "RGB:\t " << brightness_sum_list.at(0) / measures << ", " << duration_sum_ms_list.at(0) / measures << " ms" << std::endl;
    std::cout << "HSV:\t " << brightness_sum_list.at(1) / measures << ", " << duration_sum_ms_list.at(1) / measures << " ms" << std::endl;
    std::cout << "YUV:\t " << brightness_sum_list.at(2) / measures << ", " << duration_sum_ms_list.at(2) / measures << " ms" << std::endl;
    std::cout << "YCrCb:\t " << brightness_sum_list.at(3) / measures << ", " << duration_sum_ms_list.at(3) / measures << " ms" << std::endl;
    std::cout << "Lab:\t " << brightness_sum_list.at(4) / measures << ", " << duration_sum_ms_list.at(4) / measures << " ms" << std::endl;
    std::cout << "Luv:\t " << brightness_sum_list.at(5) / measures << ", " << duration_sum_ms_list.at(5) / measures << " ms" << std::endl;
    std::cout << "XYZ:\t " << brightness_sum_list.at(6) / measures << ", " << duration_sum_ms_list.at(6) / measures << " ms" << std::endl;
}

#endif //OPENCVMEANBRIGHTNESSTEST_HELPERFUNCTIONS_H
