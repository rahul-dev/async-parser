#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

class fileprocessor
{
public:
	explicit fileprocessor(const std::string &filename);
	void operator()();

	void producer();

private:
	void partition();

private:
	const std::string m_filename;
	std::vector<char> m_vecBalls;

	std::mutex m_mutex;
	std::condition_variable m_dataAvailable;
	std::queue<char> m_msgQ;
	bool m_exit;
};