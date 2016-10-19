# OpenCV-Facial-Recognition

## Introduction
This program is built upon the framework developed by Ben Hoff.
It works on 2 threads: one thread runs the camera input and processes the allocated cv::mat. Then, it emits the complete data as a signal to QT, which the main thread receives in the appropriate slot and subsequently runs the FisherFaceRecognizer algorithm. I also threw in a dark UI theme to make things more aesthetically pleasing.
## TODO:
* Fix crash on program close.
* Other stuff I can't remember.
