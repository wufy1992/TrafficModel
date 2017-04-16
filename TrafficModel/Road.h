#pragma once
class CRoad
{
public:
	CRoad();
	~CRoad();

	void setTraffic(double nCarTraffic, double nWalkTraffic);
	void optimization();
	double getValue(double a, double b);

	// 车道的容量
	double m_nCarRoadCapacity;
	// 步行道的容量
	double m_nWalkRoadCapacity;

private:

	// 交通街道单位截面满意度
	double getCarRoadSatisfaction(double n, double c, double Pn);
	// 步行街道单位截面满意度
	double getWalkRoadSatisfaction(double c);

	bool constraintCondition(double Sp, double ci, double Qp, double Qc, double vp, double sp_, double dp, double S0p, double t, double T, double lc_, double vc, double l0c, double Lr, double h);

	double getCarRoadCapacity();
	double getWalkRoadCapacity();

	/*----------------------------数据区----------------------------------------*/
	double g_s1, g_s2, g_s3, g_s4;
	double g_p1, g_p2, g_p3, g_p4;

	double g_p, g_l;

	double g_pl_3;

	double g_N0, g_r2, g_r3, g_r4;
	double g_r1 = 0.95;

	// 车道宽度
	double g_lp = 3.25;

	double g_funcFi;
	double g_funcTi;
	double g_Np_;
	double g_lc, g_lb;

	// 交通量
	double m_nCarTraffic;
	double m_nWalkTraffic;


	// 最优解
	double m_nOptimalValue;

	double n, c, Pn;

	double Sp, ci, Qp, Qc, vp, sp_, dp, S0p, t, T, lc_, vc, l0c, Lr, h;

	double a, b;
};

