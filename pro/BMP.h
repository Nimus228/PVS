#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "Windows.h"
using namespace std;
class BMP
{
public:
    BITMAPFILEHEADER mapFile;
    BITMAPINFOHEADER mapInfo;
    RGBTRIPLE rgb_l;
    float** MATRIX;
    float** res;
    int core_1[5][5], core_2[5][5], core_3[5][5];
    BMP();
    ~BMP();
    void loadImg(string name, bool isGrayScale);
    void saveImg(string name);
    void matrixOut(); 
    void matrixOut(string name);
    void doTaskLine(bool parallel);
    int multiSumMatrix(int i, int j, int core_1[5][5], int core_2[5][5], int core_3[5][5]);
};
