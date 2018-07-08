//
// Created by LGW on 2017/5/11.
//
#pragma once

#include <math.h>
#include <string>
#define M_PI 3.14159265358979323846
#define DEFFAULT_LONTITUDE -9999
using namespace std;


namespace coord {
	/**
	* ����� �ṹ��
	*/
	struct Point  //�����ṹ������
	{
		double x;
		double y;
		double z;
		Point() {}
		Point(double _x, double _y, double _z) :x(_x), y(_y), z(_z){}
	};

	/**
	* ���������
	*/
	struct EllipsoidParam
	{
		string name;
		double a;
		double b;
		double f;
	};

	/**
	* �������ϵö����
	*/
	enum GEODETIC_COORD_SYSTEM
	{
		XIAN80,
		WGS84
	};

	/**
	* ������������������� ���������ϵ���ƣ�a,b,f��
	*/
	static EllipsoidParam EllipsoidParamArray[] =
	{
		{ "XIAN80", 6378140, 6356755.2882, 1 / 298.257 },
		{ "WGS84", 6378137, 6356752.314, 1 / 298.2572236 }
	};
}

using namespace coord;
class Trans
{
private:
	double
		dx,//x����ƽ����(��)
		dy,//y����ƽ����(��)
		dz,//z����ƽ����(��)
		Ox,//x������ת�Ƕ�(��)
		Oy,//y������ת�Ƕ�(��)
		Oz,//z������ת�Ƕ�(��)
		k; //�߶�����(ppm)��������ʵӦ���ñ���m��ʾ
	//����Ҫ˵��һ�£��߶����������ֵ�λ�ı�ʾ��ʽ��һ�ֽ��ԼΪ1����1.0000045����k��ʾ��
	//��һ�־���ppm�ı�ʾ��ʽ����΢��1��һ�㣬��4.5����m��ʾ��k=1+m/1000000
	
	GEODETIC_COORD_SYSTEM coordSystem_origin;//ԭʼ����ϵ
	GEODETIC_COORD_SYSTEM coordSystem_result;//�������ϵ
	
private:
	/**
	 * ���Ƕ�ת��Ϊ����
	 */
	double toRadians(double angdeg);

	/**
	 * ������ת��Ϊ�Ƕ�
	 */
	double toDegrees(double angdeg);


public:
	/**
	 * �߲��� + ԭʼ����ϵ + �������ϵ
	 */
	Trans(double dx, double dy, double dz, double Ox, double Oy, double Oz, double k,
		GEODETIC_COORD_SYSTEM _coordSystem_origin, GEODETIC_COORD_SYSTEM _coordSystem_result);

	~Trans();

	GEODETIC_COORD_SYSTEM getOriginCoordSystem()
	{
		return coordSystem_origin;
	}

	GEODETIC_COORD_SYSTEM getResultCoordSystem()
	{
		return coordSystem_result;
	}

	/**
	 * �������ת�ռ�����
	 * ���룺pt:�������(�ȡ�)  a��b:�ô������ϵ������ĳ����̰���(m)
	 * ������ռ�����(m)
	 */
	Point geodetic2spatial(Point pt,double a ,double b);
	
	/**
	 * �߲������㣨spatial1 -> spatial2��
	 * ���룺point:�ռ�����(m) 
	 * ������ռ�����(m)
	 */
	Point qicanshu(Point point);

	/**
	* �ռ�����ת�������
	* ���룺pt:�ռ�����(m)  a��f:����������ϵ������ĳ�����(m)�ͱ���
	* ������������(�ȡ�)
	*/
	Point spatial2geodetic(Point pt, double a, double f);

	/**
	 * ��˹ͶӰ �������->ƽ������
	 * Lo; //���뾭��(��) ��˹ͶӰʱʹ�� �ṩ�Ļ�������������뾭�߼��㣬û�ṩ��ͨ���������ó�
	 * a��b��f:�������ϵ������ĳ����̰���(m)�ͱ���
	 * ZoneWide:���� 3 or 6
	 */
	Point gaussPrj_geodetic2plane(Point point, double a, double b, double f, int ZoneWide, int Lo = DEFFAULT_LONTITUDE);

	/**
	 * ��˹���� ƽ������->�������
	 * Lo; //���뾭��(��) ��˹ͶӰʱʹ�� �ṩ�Ļ�������������뾭�߼��㣬û�ṩ��ͨ���������ó� 
	 * a��b:����������ϵ������ĳ����̰���(m)
	 * ZoneWide:���� 3 or 6
	 */
	Point gaussInversePrj_plane2geodetic(Point point, double a, double b,int ZoneWide, int Lo = DEFFAULT_LONTITUDE);

	/**
	 * �������ϵ1�µľ�γ������ -> �������ϵ2�µĸ�˹ͶӰƽ������
	 * ���룺point���������ϵ1�µľ�γ�����꣩,*_origin�ֱ�Ϊ�������ϵ1��Ӧ������ĳ����̰��ᣨm��������
	 * ���룺*_result�ֱ�Ϊ�������ϵ2��Ӧ��������ĳ����̰��ᣨm�������ʣ�ZoneWide:���� 3 or 6��Lo; //���뾭��(��) ��˹ͶӰʱʹ�ã���ѡ��
	 * ������������ϵ2�µĸ�˹ͶӰƽ������
	 */
	Point geodetic2plane(Point point, double a_origin, double b_origin, double a_result, double b_result, double f_result, int ZoneWide, int Lo = DEFFAULT_LONTITUDE);

	/**
	* �������ϵ1�µĸ�˹ͶӰƽ������ -> �������ϵ2�µľ�γ������
	* ���룺point���������ϵ1�µĸ�˹ͶӰƽ�����꣩,*_origin�ֱ�Ϊ�������ϵ1��Ӧ������ĳ����̰��ᣨm��������
	* ���룺*_result�ֱ�Ϊ�������ϵ2��Ӧ��������ĳ����̰��ᣨm�������ʣ�ZoneWide:���� 3 or 6��Lo; //���뾭��(��) ��˹ͶӰʱʹ�ã���ѡ��
	* ������������ϵ2�µľ�γ������
	*/
	Point plane2geodetic(Point point, double a_origin, double b_origin, double f_origin, double a_result, double f_result, int ZoneWide, int Lo = DEFFAULT_LONTITUDE);

	/**
	 * �������ϵ1�µľ�γ������ -> �������ϵ2�µĸ�˹ͶӰƽ������
	 * ���룺ZoneWide:���� 3 or 6��Lo; //���뾭��(��) ��˹ͶӰʱʹ�ã���ѡ��
	 * ���������ʹ�ù��캯��ö�����Ͷ�Ӧ�Ĳ���
	 */
	Point geodetic2plane(Point point, int ZoneWide, int Lo = DEFFAULT_LONTITUDE);

	/**
	 * �������ϵ1�µĸ�˹ͶӰƽ������ -> �������ϵ2�µľ�γ������
	 * ���룺ZoneWide:���� 3 or 6��Lo; //���뾭��(��) ��˹ͶӰʱʹ�ã���ѡ��
	 * ���������ʹ�ù��캯��ö�����Ͷ�Ӧ�Ĳ���
	 */
	Point plane2geodetic(Point point, int ZoneWide, int Lo = DEFFAULT_LONTITUDE);
};

