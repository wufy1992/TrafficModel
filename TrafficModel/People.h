#pragma once
class CPeople
{
public:
	CPeople();
	CPeople(double nStartUseCarProbability, double nCarRoadCapacity, double nWalkRoadCapacity, double nCarFreeflowUtility, double nCarStandardUtility, double nWalkFreeflowUtility, double nWalkStandardUtility);
	~CPeople();

	// 出行概率修正函数
	void correctUserCarProbability(int nLastCarNumber, int nLastWalkNumber);

	// 判断本次出行是否开车
	bool isUserCar();

	// 修改街道属性
	void setRoadCapacity(double nCarRoadCapacity, double nWalkRoadCapacity);

private:
	// 初始化一些变量
	void init();

	// 根据交通量计算上次出行的时间
	double getLastDrivingTime();

	// 交通类街道效用函数
	double getCarRoadUtility();

	// 生活类街道效用函数
	double getWalkRoadUtility();

	// 数值计算
	double NormalExp(double utility, double expect = 0.0, double variance = 1.0);
	double NormalStats(double utilityMin, double utilityMax, double expect = 0.0, double variance = 1.0);
	double getValueWithProbability(double utilityMin, double utilityMax, double expect, double variance, bool bIsCar = true);

	// 前景理论价值函数
	double getValue(double utility, bool bIsCar = true);

	// 前景理论权重
	double getWeights(double P, double r);


	// -----------------------------------------数据区---------------------------------------------------------------------

	// 开车的概率
	double m_nUseCarProbability;

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

	// 上次的开车的人数
	int m_nLastCarNumber;
	// 上次步行的人数
	int m_nLastWalkNumber;

	// 自由流时开车效用值
	double m_nCarFreeflowUtility;
	// 标准点时开车效用值
	double m_nCarStandardUtility;

	// 自由流时步行效用值
	double m_nWalkFreeflowUtility;
	// 标准点时步行效用值
	double m_nWalkStandardUtility;

};

