#include "BMP.h"
#include <chrono>
#include <omp.h>
using namespace std;
BMP::BMP() {
    ifstream file_1("core_1.txt");
    ifstream file_2("core_2.txt");
    ifstream file_3("core_3.txt");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            file_1 >> core_1[i][j];
            file_2 >> core_2[i][j];
            file_3 >> core_3[i][j];
        }
    }
    file_1.close();
    file_2.close();
    file_3.close();
};//�����������
BMP::~BMP() {
    for (int i = 0; i < mapInfo.biHeight; i++)
    {
        delete[] MATRIX[i];
    }
    delete[] MATRIX;
    for (int i = 0; i < mapInfo.biHeight; i++)
    {
        delete[] res[i];
    }
    delete[] res;
};
void BMP::loadImg(string name, bool isGrayScale) {
    FILE* img;
    if (fopen_s(&img, name.c_str(), "rb") == 0) {
        cout << "file is open\n";
    }
    else {
        cout << "file is NOT open\n";
    }
    fread(&mapFile, sizeof(mapFile), 1, img);
    fread(&mapInfo, sizeof(mapInfo), 1, img);
    MATRIX = new float* [mapInfo.biHeight];
    res = new float* [mapInfo.biHeight];
    for (int i = 0; i < mapInfo.biHeight; i++)
    {
        res[i] = new float[mapInfo.biWidth];//��������� ���������
    }
    for (int i = 0; i < mapInfo.biHeight; i++)
    {
        MATRIX[i] = new float[mapInfo.biWidth];//�������� ������ ��� ���������
    }
    float grayScale;
    for (int i = 0; i < mapInfo.biHeight; i++)
    {
        for (int j = 0; j < mapInfo.biWidth; j++)
        {
            if (isGrayScale) {
                fread(&rgb_l, sizeof(rgb_l), 1, img);
                MATRIX[i][j] = rgb_l.rgbtRed;
                res[i][j] = MATRIX[i][j];
            }
            else {
                fread(&rgb_l, sizeof(rgb_l), 1, img);
                grayScale = rgb_l.rgbtRed * 0.3 + rgb_l.rgbtGreen * 0.59 + rgb_l.rgbtBlue * 0.11;//������� �������� ����������� � �������� ������ �� ����� ������ �������
                rgb_l.rgbtRed = grayScale;
                rgb_l.rgbtGreen = grayScale;
                rgb_l.rgbtBlue = grayScale;
                MATRIX[i][j] = grayScale;
                res[i][j] = MATRIX[i][j];
            }
        }
    }
    _fcloseall();
};
//����������� ��������� �������� � ���������� ��� � ��� ���������
void BMP::saveImg(string name) {
    FILE* img;
    if (fopen_s(&img, name.c_str(), "wb") == 0) {
        cout << "file is open\n";
    }
    else {
        cout << "file is NOT open\n";
    }
    fwrite(&mapFile, sizeof(mapFile), 1, img);
    fwrite(&mapInfo, sizeof(mapInfo), 1, img);
    for (int i = 0; i < mapInfo.biHeight; i++)
    {
        for (int j = 0; j < mapInfo.biWidth; j++)
        {
            RGBTRIPLE buf;
            buf.rgbtBlue = res[i][j];
            buf.rgbtRed = res[i][j];
            buf.rgbtGreen = res[i][j];
            fwrite(&buf, sizeof(buf), 1, img);
        }
    }
    _fcloseall();
    cout << "file is saved\n";
};
//����� ������� �� �����, �� ���� ���� �� ������� ����� � �������� �������
void BMP::matrixOut() {
    for (int i = 0; i < mapInfo.biHeight; i++) {
        cout << endl;
        for (int j = 0; j < mapInfo.biWidth; j++) {
            cout << res[i][j] << " ";
        }
    }
    cout << endl;
}
//��������������� ������ ��� ���������� � ����
void BMP::matrixOut(string name) {
    ofstream file(name + ".txt");
    for (int i = 0; i < mapInfo.biHeight; i++) {
        file << endl;
        for (int j = 0; j < mapInfo.biWidth; j++) {
            file << res[i][j] << " ";
        }
    }
    file.close();
    cout << "matrix is saved\n";
}
void BMP::doTaskLine(bool parallel) {
    //�������� ������������� ���������� ����������� ���� � ���� �����
    auto  start_time = chrono::steady_clock::now();//��� ������ ������� ���������� �������
    //������ � ����� ��� ������ �������� ������� � ������� � � 3 ������� ��������������� ����� ������� � �������, ������� ���������� ������ �������� ������� �� ��������������� ������ 
    //���� � ��������� �� ���, ����� ������ � ������� � ������ ������� res
    int height = mapInfo.biHeight - 2;
    int width = mapInfo.biWidth - 2;
#pragma omp parallel if (parallel)
    {
#pragma omp for
    for (int i = 2; i < height; i++) {
        for (int j = 2; j < width; j++) {
            
            res[i][j] = multiSumMatrix(i - 2, j - 2, core_1, core_2, core_3);
        }
    }
    }
    auto  end_time = chrono::steady_clock::now();
    auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << elapsed_ns.count() << "ns\n";
    cout << "ready\n";
}
int BMP::multiSumMatrix(int x, int y, int core_1[5][5], int core_2[5][5], int core_3[5][5]) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            sum += MATRIX[x + i][y + j] * (core_1[i][j] + core_2[i][j] + core_3[i][j]);
        }
    }
    //���������� ��� �� ������� 413181975400ns ��� � 10 ��� ������ ���������������� ���������
    return sum;
}
