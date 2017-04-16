#pragma once
class CRoad
{
public:
	CRoad();
	~CRoad();

	void setTraffic(double nCarTraffic, double nWalkTraffic);
	void optimization();
	double getValue(double a, double b);

	// ����������
	double m_nCarRoadCapacity;
	// ���е�������
	double m_nWalkRoadCapacity;

private:

	// ��ͨ�ֵ���λ���������
	double getCarRoadSatisfaction(double n, double c, double Pn);
	// ���нֵ���λ���������
	double getWalkRoadSatisfaction(double c);

	bool constraintCondition(double Sp, double ci, double Qp, double Qc, double vp, double sp_, double dp, double S0p, double t, double T, double lc_, double vc, double l0c, double Lr, double h);

	double getCarRoadCapacity();
	double getWalkRoadCapacity();

	/*----------------------------������----------------------------------------*/
	double g_s1, g_s2, g_s3, g_s4;
	double g_p1, g_p2, g_p3, g_p4;

	double g_p, g_l;

	double g_pl_3;

	double g_N0, g_r2, g_r3, g_r4;
	double g_r1 = 0.95;

	// �������
	double g_lp = 3.25;

	double g_funcFi;
	double g_funcTi;
	double g_Np_;
	double g_lc, g_lb;

	// ��ͨ��
	double m_nCarTraffic;
	double m_nWalkTraffic;


	// ���Ž�
	double m_nOptimalValue;

	double n, c, Pn;

	double Sp, ci, Qp, Qc, vp, sp_, dp, S0p, t, T, lc_, vc, l0c, Lr, h;

	double a, b;
};

