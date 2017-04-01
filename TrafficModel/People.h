#pragma once
class CPeople
{
public:
	CPeople();
	CPeople(double nStartUseCarProbability, double nCarRoadCapacity, double nWalkRoadCapacity, double nCarFreeflowUtility, double nCarStandardUtility, double nWalkFreeflowUtility, double nWalkStandardUtility);
	~CPeople();

	// ���и�����������
	void correctUserCarProbability(int nLastCarNumber, int nLastWalkNumber);

	// �жϱ��γ����Ƿ񿪳�
	bool isUserCar();

	// �޸Ľֵ�����
	void setRoadCapacity(double nCarRoadCapacity, double nWalkRoadCapacity);

private:
	// ��ʼ��һЩ����
	void init();

	// ���ݽ�ͨ�������ϴγ��е�ʱ��
	double getLastDrivingTime();

	// ��ͨ��ֵ�Ч�ú���
	double getCarRoadUtility();

	// ������ֵ�Ч�ú���
	double getWalkRoadUtility();

	// ��ֵ����
	double NormalExp(double utility, double expect = 0.0, double variance = 1.0);
	double NormalStats(double utilityMin, double utilityMax, double expect = 0.0, double variance = 1.0);
	double getValueWithProbability(double utilityMin, double utilityMax, double expect, double variance, bool bIsCar = true);

	// ǰ�����ۼ�ֵ����
	double getValue(double utility, bool bIsCar = true);

	// ǰ������Ȩ��
	double getWeights(double P, double r);


	// -----------------------------------------������---------------------------------------------------------------------

	// �����ĸ���
	double m_nUseCarProbability;

	// ����������
	double m_nCarRoadCapacity;
	// ���е�������
	double m_nWalkRoadCapacity;

	// ��ε�ѡ���Ƿ񿪳�
	bool m_bUseCar;

	// �����Ĵ���
	int m_nUseCarTimes;
	// ���еĴ���
	int m_nWalkTimes;

	// ��¼���ο������������
	std::vector<double> m_vecUseCarProfit;
	// ��¼���β������������
	std::vector<double> m_vecWalkProfit;

	// ��¼��������ľ�ֵ
	double m_nUseCarProfitMean;
	// ��¼��������ľ�ֵ
	double m_nWalkProfitMean;

	// ��¼��������ķ���
	double m_nUseCarProfitVariance;
	// ��¼��������ķ���
	double m_nWalkProfitVariance;

	// �ϴεĿ���������
	int m_nLastCarNumber;
	// �ϴβ��е�����
	int m_nLastWalkNumber;

	// ������ʱ����Ч��ֵ
	double m_nCarFreeflowUtility;
	// ��׼��ʱ����Ч��ֵ
	double m_nCarStandardUtility;

	// ������ʱ����Ч��ֵ
	double m_nWalkFreeflowUtility;
	// ��׼��ʱ����Ч��ֵ
	double m_nWalkStandardUtility;

};

