#pragma once
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/atomic.hpp>

#include <iostream>

class TwoProcesses
{
public:
	TwoProcesses();
	~TwoProcesses();
	void depositMoney(int& t_account, boost::mutex* t_coutGuard);
	void withdrawMoney(int& t_account, boost::mutex* t_coutGuard);
	void setDone(bool t_bool);

private:
	boost::mutex m_mutex;
	boost::condition_variable m_conditional;
	boost::condition_variable m_conditionalTest;
	bool m_done = false;

	boost::mutex m_screen;
};

TwoProcesses::TwoProcesses()
{
}

TwoProcesses::~TwoProcesses()
{
}

void TwoProcesses::depositMoney(int& t_account, boost::mutex* t_coutGuard)
{
	{
		// go to the bank etc...
		// wait in line...
		// lock cout until the closing brace
		//boost::mutex::scoped_lock lockTest(*t_coutGuard);

		boost::unique_lock<boost::mutex> lock(m_mutex);

		//m_screen.lock();
		std::cout << "Depositing money on Thread: " << boost::this_thread::get_id() << std::endl;
		//m_screen.unlock();
		t_account += 5000;

		// Notify others we're finished
		m_conditional.notify_all();
	}
}

void TwoProcesses::withdrawMoney(int& t_account, boost::mutex* t_coutGuard)
{
	{
		// lock cout until the closing brace
		//boost::mutex::scoped_lock lockTest(*t_coutGuard);
		//m_conditionalTest.wait(lockTest);

		boost::unique_lock<boost::mutex> lock(m_mutex);

		// Wait until we know the money is there
		m_conditional.wait(lock);
		//m_screen.lock();
		std::cout << "Withdrawing money on Thread: " << boost::this_thread::get_id() << std::endl;
		//m_screen.unlock();
		t_account -= 2000;
	}
}

void TwoProcesses::setDone(bool t_bool)
{
	m_done = t_bool;
}
