#include "stdafx.h"
#include "People.h"
#include <map>
#include <mutex>
#include <cmath>
#include <string>
#include <algorithm>


static const double eps = 1e-7;

// 记录NormalStats已经算过的值，提高效率
std::map<std::string, double> sMapVis;
static std::mutex sMapMutex;

CPeople::CPeople()
{

}

CPeople::CPeople(double nStartUseCarProbability, double nCarRoadCapacity, double nWalkRoadCapacity, double nCarFreeflowUtility, double nCarStandardUtility, double nWalkFreeflowUtility, double nWalkStandardUtility)
	:m_nUseCarProbability(nStartUseCarProbability),
	m_nCarRoadCapacity(nCarRoadCapacity),
	m_nWalkRoadCapacity(nWalkRoadCapacity),
	m_nCarFreeflowUtility(nCarFreeflowUtility),
	m_nCarStandardUtility(nCarStandardUtility),
	m_nWalkFreeflowUtility(nWalkFreeflowUtility),
	m_nWalkStandardUtility(nWalkStandardUtility)
{
	init();
}


CPeople::~CPeople()
{
}


void CPeople::correctUserCarProbability(int nLastCarNumber, int nLastWalkNumber)
{
	m_nLastCarNumber = nLastCarNumber;
	m_nLastWalkNumber = nLastWalkNumber;
	if (m_bUseCar)
	{
		double carRoadUtility = getCarRoadUtility();
		m_vecUseCarProfit.push_back(carRoadUtility);
		m_nUseCarProfitMean = (m_nUseCarProfitMean * (carRoadUtility - 1) + carRoadUtility) / m_nUseCarTimes;

		double ret = 0;
		for (size_t i = 0; i < m_vecUseCarProfit.size(); i++)
		{
			ret += (m_vecUseCarProfit[i] - m_nUseCarProfitMean) * (m_vecUseCarProfit[i] - m_nUseCarProfitMean);
		}
		m_nUseCarProfitVariance = ret / m_vecUseCarProfit.size();
	}
	else
	{
		double walkRoadUtility = getWalkRoadUtility();
		m_vecWalkProfit.push_back(walkRoadUtility);
		m_nWalkProfitMean = (m_nWalkProfitMean * (m_nWalkTimes - 1) + walkRoadUtility) / m_nWalkTimes;

		double ret = 0;
		for (size_t i = 0; i < m_vecWalkProfit.size(); i++)
		{
			ret += (m_vecWalkProfit[i] - m_nWalkProfitMean) * (m_vecWalkProfit[i] - m_nWalkProfitMean);
		}
		m_nWalkProfitVariance = ret / m_vecWalkProfit.size();


		m_nUseCarProfitVariance = std::max(0.1, m_nUseCarProfitVariance);
		m_nWalkProfitVariance = std::max(0.1, m_nWalkProfitVariance);
	}

	if (m_nUseCarProfitVariance > eps && m_nWalkProfitVariance > eps && m_nUseCarProfitMean > eps && m_nWalkProfitMean > eps)
	{
		double minVCar = m_nUseCarProfitMean - 3.00 * sqrt(m_nUseCarProfitVariance);
		double maxVCar = m_nUseCarProfitMean + 3.00 * sqrt(m_nUseCarProfitVariance);
		double minVWalk = m_nWalkProfitMean - 3.00 * sqrt(m_nWalkProfitVariance);
		double maxVWalk = m_nWalkProfitMean + 3.00 * sqrt(m_nWalkProfitVariance);

		double P1Car = NormalStats(minVCar, m_nCarFreeflowUtility, m_nUseCarProfitMean, m_nUseCarProfitVariance);
		double P2Car = NormalStats(m_nCarFreeflowUtility, m_nCarStandardUtility, m_nUseCarProfitMean, m_nUseCarProfitVariance);
		double P3Car = NormalStats(m_nCarStandardUtility, maxVCar, m_nUseCarProfitMean, m_nUseCarProfitVariance);

		double P1Walk = NormalStats(minVWalk, m_nWalkFreeflowUtility, m_nWalkProfitMean, m_nWalkProfitVariance);
		double P2Walk = NormalStats(m_nWalkFreeflowUtility, m_nWalkStandardUtility, m_nWalkProfitMean, m_nWalkProfitVariance);
		double P3Walk = NormalStats(m_nWalkStandardUtility, maxVWalk, m_nWalkProfitMean, m_nWalkProfitVariance);

		double Vk1Car = 0.00;
		double Vk2Car = 0.00;
		double Vk3Car = 0.00;
		if (P1Car > 0.00) {
			Vk1Car = getValueWithProbability(minVCar, m_nCarFreeflowUtility, m_nUseCarProfitMean, m_nUseCarProfitVariance) / P1Car;
		}
		if (P2Car > 0.00) {
			Vk2Car = getValueWithProbability(m_nCarFreeflowUtility, m_nCarStandardUtility, m_nUseCarProfitMean, m_nUseCarProfitVariance) / P2Car;
		}
		if (P3Car > 0.00) {
			Vk3Car = getValueWithProbability(m_nCarStandardUtility, maxVCar, m_nUseCarProfitMean, m_nUseCarProfitVariance) / P3Car;
		}

		double Vk1Walk = 0.00;
		double Vk2Walk = 0.00;
		double Vk3Walk = 0.00;

		if (P1Walk > 0.00) {
			Vk1Walk = getValueWithProbability(minVWalk, m_nWalkFreeflowUtility, m_nWalkProfitMean, m_nWalkProfitVariance, false) / P1Walk;
		}
		if (P2Walk > 0.00) {
			Vk2Walk = getValueWithProbability(m_nWalkFreeflowUtility, m_nWalkStandardUtility, m_nWalkProfitMean, m_nWalkProfitVariance, false) / P2Walk;
		}
		if (P3Walk > 0.00) {
			Vk3Walk = getValueWithProbability(m_nWalkStandardUtility, maxVWalk, m_nWalkProfitMean, m_nWalkProfitVariance, false) / P3Walk;
		}

		double PECar1 = Vk1Car * getWeights(P1Car, 0.69);
		double PECar2 = Vk2Car * getWeights(P2Car, 0.61);
		double PECar3 = Vk3Car * getWeights(P3Car, 0.69);

		double PEWalk1 = Vk1Walk * getWeights(P1Walk, 0.69);
		double PEWalk2 = Vk2Walk * getWeights(P2Walk, 0.61);
		double PEWalk3 = Vk3Walk * getWeights(P3Walk, 0.69);

		double PECar = PECar1 + PECar2 + PECar3;
		double PEWalk = PEWalk1 + PEWalk2 + PEWalk3;

		m_nUseCarProbability = exp(PECar) / (exp(PECar) + exp(PEWalk));
	}
}

bool CPeople::isUserCar()
{
	// 根据出行概率，用随机函数判断当前这人是否开车
	double randD = 0.01 * (rand() % 100);
	m_bUseCar = randD < m_nUseCarProbability;

	if (m_bUseCar)
		m_nUseCarTimes++;
	else
		m_nWalkTimes++;

	return m_bUseCar;
}


void CPeople::init()
{
	m_nUseCarTimes = 0;
	m_nWalkTimes = 0;

	m_vecUseCarProfit.clear();
	m_vecWalkProfit.clear();

	m_nUseCarProfitMean = 0.00;
	m_nUseCarProfitVariance = 0.00;

	m_nWalkProfitMean = 0.00;
	m_nWalkProfitVariance = 0.00;
}

void CPeople::setRoadCapacity(double nCarRoadCapacity, double nWalkRoadCapacity)
{
	m_nCarRoadCapacity = nCarRoadCapacity;
	m_nWalkRoadCapacity = nWalkRoadCapacity;

	// 路的属性变了，之前的记录就有参考意义了
	init();
}

double CPeople::getLastDrivingTime()
{
	return (1 + 0.15 * (m_nLastCarNumber / m_nCarRoadCapacity) * (m_nLastCarNumber / m_nCarRoadCapacity)  * (m_nLastCarNumber / m_nCarRoadCapacity)  * (m_nLastCarNumber / m_nCarRoadCapacity)) / 11.00;
}

double CPeople::getCarRoadUtility()
{
	int lastDrivingTime = getLastDrivingTime();

	double comfort = m_nCarRoadCapacity / m_nLastCarNumber * 2.67;
	double fee = (2 * (comfort - lastDrivingTime) - (comfort - 0.20)) / (comfort - lastDrivingTime);

	return -0.154 - 0.202 * fee + 0.612 *comfort;
}

double CPeople::getWalkRoadUtility()
{
	return -0.154 / 1.5 + 0.612  *  m_nWalkRoadCapacity / m_nLastWalkNumber * 4.67;
}

double CPeople::getValue(double utility, bool bIsCar)
{
	double nFreeflowUtility = m_nCarFreeflowUtility;
	double nStandardUtility = m_nCarStandardUtility;


	if (!bIsCar)
	{
		nFreeflowUtility = m_nWalkFreeflowUtility;
		nStandardUtility = m_nWalkStandardUtility;
	}
	if (utility < nFreeflowUtility)
	{
		return -2.25 * pow((nFreeflowUtility - utility), 0.88);
	}
	else if (utility <= nStandardUtility)
	{
		return pow((utility - nFreeflowUtility), 0.88);
	}
	else
	{
		return pow((nStandardUtility - nFreeflowUtility), 0.88);
	}
}

double CPeople::getWeights(double P, double r)
{
	return pow(P, r) / pow(pow(P, r) + pow(1 - P, r), 1 / r);
}


double CPeople::NormalExp(double utility, double expect, double variance)
{
	return exp(-(utility - expect) * (utility - expect) / (2 * variance));
}

double CPeople::NormalStats(double utilityMin, double utilityMax, double expect, double variance)
{
	if (utilityMin >= utilityMax)
		return 0.00;

	double s1 = 0;
	double s2 = (utilityMax - utilityMin) * (NormalExp(utilityMin, expect, variance) + NormalExp(utilityMax, expect, variance)) / 2;

	if (s2 < eps)
		return 0.00;

	std::string strKey = std::to_string(utilityMin) + std::to_string(utilityMax) + std::to_string(expect) + std::to_string(variance);

	{
		std::lock_guard<std::mutex> lck(sMapMutex);
		std::map<std::string, double > ::iterator iter = sMapVis.find(strKey);
		if (iter != sMapVis.end())
			return iter->second;
	}


	for (int i = 1; fabs(s1 - s2) > eps; i *= 2)
	{
		double h = (utilityMax - utilityMin) / (double)i;
		s1 = s2;
		s2 = 0;
		for (int j = 0; j < i; j++)
		{
			s2 += h * NormalExp(utilityMin + ((double)j + 0.5) * h, expect, variance);
		}
		s2 = (s1 + s2) / 2;
	}
	int ans = s2 * sqrt(1 / (8 * atan(1.0) * variance));

	{
		std::lock_guard<std::mutex> lck(sMapMutex);
		sMapVis.insert(make_pair(strKey, ans));
	}

	return ans;
}


double CPeople::getValueWithProbability(double utilityMin, double utilityMax, double expect, double variance, bool bIsCar)
{

	if (utilityMin >= utilityMax)
		return 0.00;

	double steps = (utilityMax - utilityMin) / 100;

	double sum = 0.0;
	double start = utilityMin;
	double end = utilityMin + steps;

	for (int i = 0; i < 100; i++)
	{
		sum += NormalStats(start, end, expect, variance) * (getValue(start, bIsCar) + getValue(end, bIsCar)) / 2.00;
		start = end;
		end += steps;
	}

	return sum;
}