#include "stdafx.h"
#include "fileprocessor.h"
#include <algorithm>
using namespace std;

fileprocessor::fileprocessor(const string &filename) :
m_filename(filename), m_exit(false)
{}

void fileprocessor::producer()
{
	fstream fs(m_filename, ios::in);
	while (fs.good())
	{
		int ball = 0;
		fs >> ball;
		unique_lock<mutex> lock(m_mutex);
		m_msgQ.push(static_cast<char>(ball));
		//if (m_msgQ.size() % 2 == 0)
			m_dataAvailable.notify_one();
	}
	unique_lock<mutex> lock(m_mutex);
	m_exit = true;
	m_dataAvailable.notify_one();
}

void fileprocessor::operator()()
{
	while (1)
	{
		{
			unique_lock<mutex> lock(m_mutex);
			m_dataAvailable.wait(lock, [=](){ return !m_msgQ.empty() || m_exit; });
			cout << " condition: " << endl;
			while (!m_msgQ.empty())
			{
				m_vecBalls.push_back(m_msgQ.front());
				m_msgQ.pop();
			}

			if (m_exit)
			{
				break;
			}

		}

		partition();
		
	}

	partition();
}

void fileprocessor::partition()
{
	cout << "before: " << m_vecBalls.size()<< endl;
	for (auto c : m_vecBalls)
	{
		cout << c << " ";
	}
	cout << endl << "after: " << endl;

	char order1 = *m_vecBalls.rbegin();
	vector<char>::iterator order1begin = m_vecBalls.begin();
	if (order1begin != m_vecBalls.end())
	{
		vector<char>::iterator order2iter = find_if(order1begin, m_vecBalls.end(), [=](char value){return value != order1; });
		if (order2iter != m_vecBalls.end())
		{
			char order2 = *order2iter;
			vector<char>::iterator newEnd = std::partition(order1begin, m_vecBalls.end(), [=](char value){return value == order1; });
			std::partition(newEnd, m_vecBalls.end(), [=](char value){return value == order2; });
		}
	}

	for (auto c : m_vecBalls)
	{
		cout << c << " ";
	}
	cout << endl;
}