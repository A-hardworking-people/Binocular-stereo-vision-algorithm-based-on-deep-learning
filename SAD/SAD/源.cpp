#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iomanip>


using namespace std;
using namespace cv;


class SAD
{
public:
	SAD(int _winSize, int _maxDis) :winSize(_winSize), maxDis(_maxDis) {}
	Mat compSAD(Mat&L, Mat&R);//����SAD
private:
	int winSize;//����˳ߴ�
	int maxDis;//����Ӳ���Ӳ�������Χ
};

Mat SAD::compSAD(Mat&L, Mat&R)
{
	int Height = L.rows;
	int Width = L.cols;
	Mat Kernel_L(Size(winSize, winSize), CV_8U, Scalar::all(0));//CV_8U:��ʾÿ�����ض�Ӧ1�ֽڣ��޷��ţ���ΧΪ0~255��ֵ�����ó�ʼ����ֵȫΪ��
	Mat Kernel_R(Size(winSize, winSize), CV_8U, Scalar::all(0));
	Mat Disparity(Height, Width, CV_8U, Scalar(0));

	for (int i = 0; i < Width - winSize; ++i)
	{
		for (int j = 0; j < Height - winSize; ++j)
		{
			Kernel_L = L(Rect(i, j, winSize, winSize));//����ͼ��ȡ�߳�ΪwinSize�ľ����
			Mat sad_Vec(1, maxDis, CV_32F, Scalar(0));//�������ֵ�����������ڴ�ž���ֵ��

			for (int k = 0; k < maxDis; ++k) {
				int x = i - k;
				if (x >= 0)
				{
					Kernel_R = R(Rect(x, j, winSize, winSize));//����ͼ��ȡ�߳�ΪwinSize�ľ����
					Mat Dif;
					absdiff(Kernel_L, Kernel_R, Dif);//������˶�ӦԪ�������ȡ����ֵ���������γɵľ���Ԫ��ÿ����ӣ��������ֵ��Dif������
					Scalar ADD = sum(Dif);//���������е�����Ԫ��ֵ��ӣ������ֵ��ADD
					//���������Ǵ��۾���d�����һ����

					float a = ADD[0];
					sad_Vec.at<float>(k) = a;//������ֵ�͸�ֵ������ֵ�������еĵ�k��Ԫ��
				}
			}
			Point minLoc;//����point����
			minMaxLoc(sad_Vec, NULL, NULL, &minLoc, NULL);
			//�Ӳ�����ˣ���sad_Vec�ж�Ӧ����ֵ��Сֵ��������ֵ��minLoc

			int loc = minLoc.x;//��ȡ�����еĺ����꣬������ֵ����Сʱ���������������Ӳ�k
			Disparity.at<char>(j, i) = loc * 15;//Ϊ��ʹ�Ӳ�ͼ�Աȶȸ��󣬸����ԣ���ÿ���Ӳ�Ԫ������15��

			//double rate = double(i) / (Width);//��¼��ɶ�
			//cout << "�����" << setprecision(2) << rate * 100 << "%" << endl;//������ڻ���ʾ����ɵĽ���
		}
	}
	return Disparity;
}

int main()
{
	Mat left = imread("C:\\Users\\PC\\Desktop\\art-2005\\view1.png", CV_LOAD_IMAGE_GRAYSCALE);//��ȡͼƬ����ת��Ϊ�Ҷ�ͼ��
	Mat right = imread("C:\\Users\\PC\\Desktop\\art-2005\\view5.png", CV_LOAD_IMAGE_GRAYSCALE);
	//-------ͼ����ʾ-----------
	namedWindow("leftimag");
	imshow("leftimag", left);

	namedWindow("rightimag");
	imshow("rightimag", right);
	//--------��SAD��ȡ�Ӳ�ͼ-----
	Mat Disparity;
	//Mat Disparity1;
	SAD mySAD(7, 34); //ԭ����,cones:7, 35  teddy-2003:7, 30
	Disparity = mySAD.compSAD(left, right);
	//medianBlur(Disparity, Disparity1,3);//���۾ۺϺ���
	
	//-------�����ʾ------
	namedWindow("Disparity");
	imshow("Disparity", Disparity);

	//-------�����Ӳ�ͼ------
	imwrite("C:\\Users\\PC\\Desktop\\art-2005\\SAD.png", Disparity);

	waitKey(0);
	return 0;
}