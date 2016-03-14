// producer_consumer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fileprocessor.h"
#include <future>
using namespace std;


int main(int argc, char* argv[])
{
	string f("C:\\rahul\\producer_consumer\\bag.txt");
	fileprocessor fp(f);

	auto t1 = async(launch::async,
		[&](){ fp.producer(); });

	auto t2 = async(launch::async,
		[&](){  fp(); });

	t1.get();
	t2.get();

	return 0;
}

