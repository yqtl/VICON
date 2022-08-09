#define LIBCALIB_OPENCV_INTEROP  
#include <fstream>  
#include <iostream>
#include <opencv2/opencv.hpp>  
  
#include "json.hpp"  
using json = nlohmann::json;  
// OpenCV parameters. Demonstrates how libCalib results can be read and used  
// with OpenCV.  
//  
// (c) Calib.io ApS  
  
// Read Calib Camera Calibrator parameters into OpenCV datastructures, suitable  
// for further use with OpenCV function. Uses nlohmann’s single-header "JSON for  
// Modern C++" for JSON import. See https://github.com/nlohmann/json  
bool readCalibParameters(const std::string &filePath,  
                         std::vector<cv::Matx33d> &K,  
                         std::vector<cv::Vec<double, 14>> &k,  
                         std::vector<cv::Vec3d> &rvec,  
                         std::vector<cv::Vec3d> &tvec) {  
  
  std::ifstream fileStream(filePath); 
  nlohmann::json jsonStruct; 
  printf("hello in try");
  try {  
    //fileStream >> jsonStruct;
    //load json file
    //fileStream >> jsonStruct;
    //printf("hello in try");
    //printf("hello in try");
    jsonStruct = json::parse(fileStream);
    //printf("hello in try");
    //printf("%s", jsonStruct["camera_matrix"]["data"].dump().c_str());
    //for (auto i: rvec) {
    //std::cout << i << std::endl; // will print: "a b c"
    //}
  } catch (...) {  
    return false;  
  }  
  
  assert(jsonStruct["Calibration"]["cameras"][0]["model"]["polymorphic_name"] ==  
         "libCalib::CameraModelOpenCV");  
  
  int nCameras = jsonStruct["Calibration"]["cameras"].size();  
  
  if (nCameras < 1) {  
    return false;  
  }  
  
  K.resize(nCameras);  
  k.resize(nCameras);  
  
  rvec.resize(nCameras);  
  tvec.resize(nCameras);  
  
  for (int i = 0; i < nCameras; ++i) {  
    auto intrinsics = jsonStruct["Calibration"]["cameras"][i]["model"]  
                                ["ptr_wrapper"]["data"]["parameters"];  
  
    double f = intrinsics["f"]["val"];  
    double ar = intrinsics["ar"]["val"];  
    double cx = intrinsics["cx"]["val"];  
    double cy = intrinsics["cy"]["val"];  
    double k1 = intrinsics["k1"]["val"];  
    double k2 = intrinsics["k2"]["val"];  
    double k3 = intrinsics["k3"]["val"];  
    double k4 = intrinsics["k4"]["val"];  
    double k5 = intrinsics["k5"]["val"];  
    double k6 = intrinsics["k6"]["val"];  
    double p1 = intrinsics["p1"]["val"];  
    double p2 = intrinsics["p2"]["val"];  
    double s1 = intrinsics["s1"]["val"];  
    double s2 = intrinsics["s2"]["val"];  
    double s3 = intrinsics["s3"]["val"];  
    double s4 = intrinsics["s4"]["val"];  
    double tauX = intrinsics["tauX"]["val"];  
    double tauY = intrinsics["tauY"]["val"];  
  
    K[i] = cv::Matx33d(f, 0.0, cx, 0.0, f * ar, cy, 0.0, 0.0, 1.0);  
    k[i] = cv::Vec<double, 14>(k1, k2, p1, p2, k3, k4, k5, k6, s1, s2, s3, s4,  
                               tauX, tauY);  
  
    auto transform = jsonStruct["Calibration"]["cameras"][i]["transform"];  
  
    auto q = transform["rotation"];  
    double qw = q["w"];  
    double qx = q["x"];  
    double qy = q["y"];  
    double qz = q["z"];  
  
    double angle = 2 * acos(qw);  
    double x = qx / sqrt(1 - qw * qw);  
    double y = qy / sqrt(1 - qw * qw);  
    double z = qz / sqrt(1 - qw * qw);  
  
    if (std::abs(angle) < std::numeric_limits<double>::epsilon()) {  
      rvec[i] = cv::Vec3d(0.0, 0.0, 0.0);  
    } else {  
      rvec[i] = angle * cv::Vec3d(x, y, z);  
    }  
  
    auto t = transform["translation"];  
    tvec[i] = cv::Vec3d(t["x"], t["y"], t["z"]);  
  }  
  
  return true;  
}  
  
int main() {  
  //read json file
  //std::string jsonFilePath = "./calib.json";
  std::string jsonFilePath = "opencv_parameters.json";
  //std::string jsonFilePath = "test.json";
  std::vector<cv::Matx33d> K;  
  std::vector<cv::Vec<double, 14>> k;  
  std::vector<cv::Vec3d> rvec;  
  std::vector<cv::Vec3d> tvec;  
  
  readCalibParameters(jsonFilePath, K, k, rvec, tvec);  
  // std::cout << "Rotation vector : " << rvec << std::endl;
  // std::cout << "Translation vector : " << tvec << std::endl;
  for (auto i: rvec) {
    std::cout << i << std::endl; // will print: "a b c"
    }
  return 0;  
}
   
