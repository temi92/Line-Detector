# Line-Detector

## Overview
The objective of this project was to design and develop an algorithm for line detection. 
An image of a tennis court was used as a test case where the aim is to detect the tennis court lines extract them and categorise them.

The following subsection will explain the pipeline of the algorithm step by step
### Algorithm
The pipeline for the proposed algorithm is shown below
![pipeline](hawkeye.png)

* The original input image is shown below:
![raw_img](raw_img.png)

* The first step would be thresholding the image to obtain a binary mask
![binary_mask](MaskedImg.png)

* The binary mask above as you can see is noisy, application of erosion techniques with horizontal and vertical kernels is done to segment horizontal and vertical lines

* Horinzontal Lines <br>

![horizontal](MaskedImgH.png)

* Vertical Lines
![verical](MaskedImgV.png)