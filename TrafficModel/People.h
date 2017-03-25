#pragma once
class CPeople
{
public:
	CPeople();
	~CPeople();

	// ���и�����������
	void correctUserCarProbability();

	// �жϱ��γ����Ƿ񿪳�
	bool isUserCar();

private:

	// ���ݽ�ͨ�������ϴγ��е�ʱ��
	double getLastDrivingTime();

	// ��ͨ��ֵ�Ч�ú���
	double getCarRoadUtility();

	// ������ֵ�Ч�ú���
	double getWalkRoadUtility();


	// -----------------------------------------������---------------------------------------------------------------------

	// �����ĸ���
	double m_nUserCarProbability;

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

	// �ϴεĽ�ͨ��
	int m_nLastCarNumber;


};

