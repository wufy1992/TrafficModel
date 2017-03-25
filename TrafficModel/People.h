#pragma once
class CPeople
{
public:
	CPeople();
	~CPeople();

	// 出行概率修正函数
	void correctUserCarProbability();

	// 判断本次出行是否开车
	bool isUserCar();

private:

	// 根据交通量计算上次出行的时间
	double getLastDrivingTime();

	// 交通类街道效用函数
	double getCarRoadUtility();

	// 生活类街道效用函数
	double getWalkRoadUtility();


	// -----------------------------------------数据区---------------------------------------------------------------------

	// 开车的概率
	double m_nUserCarProbability;

	// 车道的容量
	double m_nCarRoadCapacity;
	// 步行道的容量
	double m_nWalkRoadCapacity;

	// 这次的选择是否开车
	bool m_bUseCar;

	// 开车的次数
	int m_nUseCarTimes;
	// 步行的次数
	int m_nWalkTimes;
	
	// 记录历次开车收益的容器
	std::vector<double> m_vecUseCarProfit;
	// 记录历次步行收益的容器
	std::vector<double> m_vecWalkProfit;

	// 记录开车收益的均值
	double m_nUseCarProfitMean;
	// 记录步行收益的均值
	double m_nWalkProfitMean;

	// 记录开车收益的方差
	double m_nUseCarProfitVariance;
	// 记录步行收益的方差
	double m_nWalkProfitVariance;

	// 上次的交通量
	int m_nLastCarNumber;


};

