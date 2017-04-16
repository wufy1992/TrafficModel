// TrafficModel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "People.h"
#include "Road.h"
#include <mutex>
#include <thread>
#include <iostream>

#define MAX_PEOPLE_NUM 20000

static CPeople people[MAX_PEOPLE_NUM];
static int finishedThreadNum = 0;
static std::mutex finishMutex;

static int nPeopleNum;
static bool bStopThread = false;

void correctThread(int id, int traffic)
{
	for (int i = id; i < nPeopleNum; i += 1000)
	{
		if (bStopThread)
			break;
		people[i].correctUserCarProbability(traffic, nPeopleNum - traffic * 2.8);
		for (int j = 1; j < 100 && i + j * 10 < nPeopleNum; j++)
		{
			people[i + j * 10].m_nUseCarProbability = people[i].m_nUseCarProbability;
		}
	}
	{
			std::lock_guard<std::mutex> lck(finishMutex);
			finishedThreadNum++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	double trafficMean;
// 	std::cout << "请输入稳态交通量： " << std::endl;
// 	std::cin >> trafficMean;

	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	int error= GetLastError();

	double nStartUseCarProbability;
	double nCarRoadCapacity;
	double nWalkRoadCapacity;
	double nCarFreeflowUtility;
	double nCarStandardUtility;
	double nWalkFreeflowUtility;
	double nWalkStandardUtility;

	std::cin >> nPeopleNum;
	std::cin >> nStartUseCarProbability;
	std::cin >> nCarRoadCapacity >> nWalkRoadCapacity;
	std::cin >> nCarFreeflowUtility >> nCarStandardUtility >> nWalkFreeflowUtility >> nWalkStandardUtility;

	std::vector<double> vecLastTraffic;

	// std::cout << nPeopleNum << std::endl;

	for (int i = 0; i < nPeopleNum; i++)
	{
		people[i] = CPeople(nStartUseCarProbability, nCarRoadCapacity, nWalkRoadCapacity, nCarFreeflowUtility, nCarStandardUtility, nWalkFreeflowUtility, nWalkStandardUtility);
	}

	CRoad road;
	int nWorkTimes = 10;
	while (nWorkTimes--)
	{
		// 交通量迭代
		int day = 1;
		vecLastTraffic.clear();
		bStopThread = false;
		while (1)
		{
			trafficMean = 0.00;
			finishedThreadNum = 0;
			int traffic = 0;
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			srand(sys.wMilliseconds % 100);
			for (int i = 0; i < nPeopleNum; i++)
			{
				if (people[i].isUserCar())
					traffic++;
			}

			traffic = (double)traffic / 2.80;

// 			for (int i = 0; i < nPeopleNum; i++)
// 			{
// 				people[i].correctUserCarProbability(traffic, nPeopleNum - traffic * 2.8);
// 			}

			for (int i = 0; i < 10; i++)
			{
				std::thread thr(&correctThread, i, traffic);//创建一个分支线程，回调到myThread函数里
				thr.detach();
			}

			int num = 0;
			while (1)
			{
				{
					std::lock_guard<std::mutex> lck(finishMutex);
					if (finishedThreadNum >= 10)
						break;
				}
				Sleep(100);
				num++;
				if (num > 100)
					bStopThread = true;

				if (num > 150)
					break;
			}
			std::cout << "day: " << day << "  traffic:" << traffic << std::endl;

			vecLastTraffic.push_back(traffic);

			bool bConvergence = true;

			if (day >= 10)
			{
				for (int i = 5; i > 0; i--)
				{
					trafficMean += vecLastTraffic[day - i];
				}
				trafficMean /= 5;

				for (int i = 5; i > 0; i--)
				{
					// std::cout << abs(vecLastTraffic[day - i] - trafficMean) / trafficMean << std::endl;
					if (abs(vecLastTraffic[day - i] - trafficMean) / trafficMean > 0.03)
					{
						bConvergence = false;
						break;
					}
				}
			}
			else
			{
				bConvergence = false;
			}

			if (bConvergence)
			{
				std::cout << "本次模拟已经趋向于收敛，交通量为： " << (int)trafficMean << std::endl;

				std::cout << "["<< vecLastTraffic[0];
				for (int i = 1; i < vecLastTraffic.size(); i++)
				{
					std::cout << ", " << vecLastTraffic[i];
				}
				std::cout << "]" << std::endl;
				break;
			}

			day++;
		}
		
		// 根据稳态交通量优化道路
		std::cout << "CarRoadUtility: " << people[0].getCarRoadUtility() << " WalkRoadUtility:" << people[0].getWalkRoadUtility() << std::endl;
		road.setTraffic(trafficMean * 2.8, nPeopleNum - trafficMean * 2.8);
		road.optimization();

		int temp = nCarRoadCapacity * 2 * 0.98;
		if (road.m_nCarRoadCapacity > temp)
		{
			road.m_nCarRoadCapacity = temp + (road.m_nCarRoadCapacity - temp) / 100;
		}

		nCarRoadCapacity = max(35000, road.m_nCarRoadCapacity);

		std::cout << "U = " << 0.8 * road.m_nCarRoadCapacity + road.m_nWalkRoadCapacity << std::endl;

		std::cout << "优化后，车道的容量为： " << (int)road.m_nCarRoadCapacity / 2 << std::endl;
		std::cout << "优化后，步行道道的容量为： " << (int)road.m_nWalkRoadCapacity * 2 << std::endl;
		
		people[0].setCorrection();

		for (int i = 0; i < nPeopleNum; i++)
		{
			people[i].setRoadCapacity(road.m_nCarRoadCapacity / 2, road.m_nWalkRoadCapacity * 2);
		}
	}
	// freopen("CON", "r", stdin);
	system("pause");
	return 0;
}