// TrafficModel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "People.h"
#include <windows.h>
#include <mutex>
#include <thread>
#include <iostream>

#define MAX_PEOPLE_NUM 20000

static CPeople people[MAX_PEOPLE_NUM];
static int finishedThreadNum = 0;
static std::mutex finishMutex;

static int nPeopleNum;

void correctThread(int id, int traffic)
{
	for (int i = id; i < nPeopleNum; i += 10)
	{
		people[i].correctUserCarProbability(traffic, nPeopleNum - traffic);
	}
	{
		std::lock_guard<std::mutex> lck(finishMutex);
		finishedThreadNum++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	freopen("in.txt", "r", stdin);
	int error = GetLastError();

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


	std::cout << nPeopleNum << std::endl;

	for (int i = 0; i < nPeopleNum; i++)
	{
		people[i] = CPeople(0.80, 12303.00, 3717.00, 4.0, 8.0, 0.5, 1);
	}

	int day = 1;
	while (1)
	{
		finishedThreadNum = 0;
		int traffic = 0;
		for (int i = 0; i < nPeopleNum; i++)
		{
			if (people[i].isUserCar())
				traffic++;
		}

		traffic = (double)traffic / 2.80;

		for (int i = 0; i < 10; i++)
		{
			std::thread thr(&correctThread, i, traffic);//创建一个分支线程，回调到myThread函数里
			thr.detach();
		}

		while (1)
		{
			{
				std::lock_guard<std::mutex> lck(finishMutex);
				if (finishedThreadNum >= 10)
					break;
			}
			Sleep(100);
		}
		std::cout << "day: " << day << "  traffic:" << traffic << std::endl;

		day++;
		if (day > 200)
			break;
	}
	system("pause");
	return 0;
}

