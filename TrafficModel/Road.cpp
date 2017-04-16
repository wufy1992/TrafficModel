#include "stdafx.h"
#include "Road.h"
#include <iostream>


CRoad::CRoad()
{
	// freopen("road.txt", "r", stdin);

	DWORD error = GetLastError();

	std::cin >> g_l;
	std::cin >> a >> b;
	std::cin >> g_s1 >> g_s2 >> g_s3 >> g_s4;
	std::cin >> g_p1 >> g_p2 >> g_p3 >> g_p4;
	std::cin >> g_N0 >> g_r1 >> g_r2 >> g_r3 >> g_r4;

	std::cin >> n >> c >> Pn;
	std::cin >> Sp >> ci >> Qp >> Qc >> vp >> sp_ >> dp >> S0p >> t >> T >> lc_ >> vc >> l0c >> Lr >> h;

}


CRoad::~CRoad()
{
}
double CRoad::getCarRoadCapacity()
{
	return g_funcTi * g_p *g_l;
}

double CRoad::getWalkRoadCapacity()
{
	return g_funcFi *(1 - g_p) * g_l;
}

double CRoad::getValue(double a, double b)
{
	return a * getCarRoadCapacity() + b * getWalkRoadCapacity();
}

double CRoad::getCarRoadSatisfaction(double n, double c, double Pn)
{
	double pl = int(g_p * g_l);
	double r1_ = pl / 20 + g_r1;

	double Np = g_N0 * g_r1 * g_r2 * g_r3 * g_r4;
	double Np_ = g_N0 * r1_ * g_r2 * g_r3 * g_r4;
	g_Np_ = Np_;

	return (Np_ - Np) * Pn / (n * g_lp);
}


double CRoad::getWalkRoadSatisfaction(double c)
{
	g_lc = 6;
	g_lb = 1.8;
	double fi_ = (g_p1 + g_p2 + c / g_lc + c / g_lb) / ((g_s1 * g_p1 + g_s2 * g_p2) / c + g_s3 + g_s4);
	return fi_;
}

bool CRoad::constraintCondition(double Sp, double ci, double Qp, double Qc, double vp, double sp_, double dp, double S0p, double t, double T, double lc_, double vc, double l0c, double Lr, double h)
{
	// return true;
	double Cp = (1 - g_p) * g_l * T * ci;
	double sp = sp_ + dp * vp * t / 3.6 + S0p;
	double Ep = m_nWalkTraffic * sp * Qp * ci * 4.0 / vp;

	double CR = Lr * T * g_p * g_l;
	double lc = (lc_ + vc * t / 3.6 + l0c) / 10;
	double Ec = m_nCarTraffic  * lc * Qc * ci / vc;

	std::cout << "Ec = " << Ec << " ";
	std::cout << "Ep = " << Ep << " ";
	std::cout << "Cp = " << Cp << " ";
	std::cout << "CR = " << CR << " ";

	return Ep < Cp && Ec < CR;

	

}

void CRoad::setTraffic(double nCarTraffic, double nWalkTraffic)
{
	m_nCarTraffic = nCarTraffic;
	m_nWalkTraffic = nWalkTraffic;
}

void CRoad::optimization()
{
	g_p = 0.00;
	double u = -1.00;
	double ans = 0.00;

	for (int i = 0; i <= 100; i++)
	{
		//cout << "p = " << g_p;
		

		g_funcFi = getWalkRoadSatisfaction(c);
		g_funcTi = getCarRoadSatisfaction(n, c, Pn);

		// cout <<  g_funcTi << " " <<  g_funcFi <<endl;

		// cout << a * g_funcTi * g_p * g_l << " " << b * g_funcFi * (1 - g_p) * g_l;

		std::cout << "p = " << g_p << " ";
		if (!constraintCondition(Sp, ci, Qp, Qc, vp, sp_, dp, S0p, t, T, lc_, vc, l0c, Lr, h))
		{
			g_p += 0.01;
			std::cout << std::endl;
			continue;
		}



		double temp = getValue(a, b);
		std::cout << "  U = " << temp << std::endl;

		// cout << "U = " << temp << "  " << "P = " << g_p <<endl;

		if (temp > u)
		{
			u = temp;
			ans = g_p;
			m_nCarRoadCapacity= getCarRoadCapacity();
			m_nWalkRoadCapacity = getWalkRoadCapacity();
		}
		g_p += 0.01;

	}
	m_nOptimalValue = u;
	std::cout << "P = " << ans << std::endl;
}
