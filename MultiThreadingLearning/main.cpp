#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/atomic.hpp>

#include <iostream>
#include "SyncTwoProcesses.h"
#include "QueuingWork.h"


int main()
{
	// code to demo 2 threads sharing a variable //for some bizzare reason hangs "randomly" when ran
	{
		int accountAmount = 0;
		boost::mutex coutGuard;
		std::cout << "Starting amount: " << accountAmount << std::endl;

		TwoProcesses tp;

		// Run both threads simultaneously:
		boost::thread boostDeposit(boost::bind(&TwoProcesses::depositMoney, &tp, std::ref(accountAmount), &coutGuard));
		boost::thread boostWithdraw(boost::bind(&TwoProcesses::withdrawMoney, &tp, std::ref(accountAmount), &coutGuard));

		// Wait for both threads to finish // cout cases the thing to lock up
		boostDeposit.join();
		boostWithdraw.join();

		std::cout << "All transactions processed. Final amount: " << accountAmount << std::endl;
	}

	/*/code to demo queuing and de-queuing
	{

		QueuingWork qw;
		std::cout << "boost::lockfree::queue is ";
		if (!qw.isLockFree()) std::cout << "not ";
		std::cout << "lockfree" << std::endl;

		boost::thread producer_thread(boost::bind(&QueuingWork::producer, &qw));
		boost::thread consumer_thread(boost::bind(&QueuingWork::consumer, &qw));

		producer_thread.join();
		qw.setDone(true);
		consumer_thread.join();

		std::cout << "produced " << qw.getProducerCount() << " objects." << std::endl;
		std::cout << "consumed " << qw.getConsumerCount() << " objects." << std::endl;
	}*/


	system("pause");
	return 0;
}