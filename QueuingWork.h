#pragma once
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/atomic.hpp>
#include <iostream>

class QueuingWork
{
public:
	QueuingWork();
	~QueuingWork();
	void producer();
	void consumer();
	bool isLockFree();
	void setDone(bool t_bool);
	int getProducerCount();
	int getConsumerCount();


private:
	int m_producer_count = 0;
	boost::atomic_int m_consumer_count;
	boost::lockfree::spsc_queue<int, boost::lockfree::capacity<1024>> m_spsc_queue;
	const int m_iterations = 10000000;
	boost::atomic<bool> m_done;

};

QueuingWork::QueuingWork() :
	m_consumer_count(0),
	m_done(false)
{
}

QueuingWork::~QueuingWork()
{
}

void QueuingWork::producer()
{
	for (int i = 0; i != m_iterations; ++i) {
		int value = ++m_producer_count;
		while (!m_spsc_queue.push(value));
	}
}
void QueuingWork::consumer()
{
	int value;
	while (!m_done) {
		while (m_spsc_queue.pop(value)) ++m_consumer_count;
	}

	while (m_spsc_queue.pop(value)) ++m_consumer_count;
}

bool QueuingWork::isLockFree()
{
	return m_spsc_queue.is_lock_free();
}

void QueuingWork::setDone(bool t_bool)
{
	m_done = t_bool;
}

inline int QueuingWork::getProducerCount()
{
	return m_producer_count;
}

inline int QueuingWork::getConsumerCount()
{
	return m_consumer_count;
}

