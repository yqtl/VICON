#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(void)
{
    printf("%s\r\n", CV_VERSION);
    printf("%u.%u.%u\r\n", CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
    printf("hello");
}