#include "stdafx.h"
#include "People.h"


CPeople::CPeople()
{
}


CPeople::~CPeople()
{
}


void CPeople::correctUserCarProbability()
{

}

bool CPeople::isUserCar()
{
	// 根据出行概率，用随机函数判断当前这人是否开车
	double randD = 0.01 * (rand() % 100);
	m_bUseCar = randD < m_nUserCarProbability;

	if (m_bUseCar)
		m_nUseCarTimes++;
	else
		m_nWalkTimes++;

	return m_bUseCar;
}

double CPeople::getLastDrivingTime()
{
	return (1 + 0.15 * (m_nLastCarNumber / m_nCarRoadCapacity) * (m_nLastCarNumber / m_nCarRoadCapacity)  * (m_nLastCarNumber / m_nCarRoadCapacity)  * (m_nLastCarNumber / m_nCarRoadCapacity)) / 11.00;
}

double CPeople::getCarRoadUtility()
{
	int lastDrivingTime = getLastDrivingTime();
	double Fj = 0.20;
	double Cj = m_nCarRoadCapacity / m_nLastCarNumber*2.67;
	double Fj1 = (2 * (Cj - lastDrivingTime) - (Cj - Fj)) / (Cj - lastDrivingTime);

	return -0.154 - 0.202 * Fj1 + 0.612 *Cj;
}

double CPeople::getWalkRoadUtility()
{
	return -0.154 / 1.5 + 0.612  *  m_nWalkRoadCapacity / m_nLastWalkNumber * 4.67;
}