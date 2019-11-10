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
		boost::mutex::scoped_lock lock(m_mutex);

		std::cout << "Depositing money on Thread: " << boost::this_thread::get_id() << std::endl;

		t_account += 5000;

		// Notify others we're finished
		m_done = true;
		m_conditional.notify_all();
	}
}

void TwoProcesses::withdrawMoney(int& t_account, boost::mutex* t_coutGuard)
{
	{
		boost::mutex::scoped_lock lock(m_mutex);

		// Wait until we know the money is there
		while (!m_done)
		{
			m_conditional.wait(lock);
		}

		std::cout << "Withdrawing money on Thread: " << boost::this_thread::get_id() << std::endl;

		t_account -= 2000;
	}
}