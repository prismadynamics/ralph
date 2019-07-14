/** @file NotifyBuffer.h
 *  @brief This is a header impliemention of a notify buffer. This implimentation was based on LSD-SLAM.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef _NOTIFYBUFFER_H_
#define _NOTIFYBUFFER_H_

#include <deque>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>


class Notifiable
{
	public:
		void notify(){
				notifyCondition.notify_all();
		}
	protected:
		boost::condition notifyCondition;
};

template< typename T >
class NotifyBuffer{

	/*
	public:
		NotifyBuffer(int bufferSize);
		NotifyBuffer(int bufferSize, Notifiable* receiver);
		void setReceiver(Notifiable* receiver);
		bool pushBack(const T& object);
		int size();
		T first();
		T popFront();
		boost::recursive_mutex& getMutex();
	private:
		boost::recursive_mutex bufferMutex;
		boost::condition bufferNonEmptyCondition;
		int bufferSize;
		std::deque< T > queue;
		Notifiable* receiver;
	*/

	public:
		/**
		 * Creates a queue with the given maximum size.
		 */
		NotifyBuffer(int bufferSize)
			: bufferSize(bufferSize)
			, receiver(nullptr)
		{
		}
		
		/**
		 * Creates a queue with the given maximum size and Notifiable instance
		 * which will be notified when a new object becomes available.
		 */
		NotifyBuffer(int bufferSize, Notifiable* receiver)
			: bufferSize(bufferSize)
			, receiver(receiver)
		{
		}
		
		/**
		 * Sets the notification receiver.
		 * 
		 * The receiver can be nullptr to disable notifications.
		 */
		void setReceiver(Notifiable* receiver)
		{
			this->receiver = receiver;
		}
		
		/**
		 * Adds an object to the back of the queue.
		 * 
		 * If the queue is full already, discards the object. Returns if there
		 * was enough space to add the object.
		 */
		bool pushBack(const T& object)
		{
			boost::unique_lock<boost::recursive_mutex> lock(bufferMutex);
			
			if (static_cast<int>(queue.size()) >= bufferSize) {
				return false;
			}
			queue.push_back(object);
			
			lock.unlock();
			
			if (receiver) {
				receiver->notify();
			}
			bufferNonEmptyCondition.notify_one();
			return true;
		}
		
		/**
		 * Returns the number of objects in the buffer.
		 */
		int size()
		{
			boost::unique_lock<boost::recursive_mutex> lock(bufferMutex);
			return queue.size();
		}
		
		/**
		 * Returns a copy of the first object.
		 */
		T first() {
			boost::unique_lock<boost::recursive_mutex> lock(bufferMutex);
			return queue.front();
		}
		
		/**
		 * Removes the first object and returns a copy of it.
		 * 
		 * If there is no object in the queue, blocks until one is available.
		 */
		T popFront() {
			boost::unique_lock<boost::recursive_mutex> lock(bufferMutex);
			
			// Block in case there is no object
			while (!(queue.size() > 0)) {
				bufferNonEmptyCondition.wait(lock);
			}
			
			T object = queue.front();
			queue.pop_front();
			return object;
		}
		
		/**
		 * Returns the buffer access mutex. Must be locked when checking size()
		 * and waiting if size() == 0.
		 */
		boost::recursive_mutex& getMutex() {
			return bufferMutex;
		}
		
	private:
		boost::recursive_mutex bufferMutex;
		boost::condition bufferNonEmptyCondition;
		
		int bufferSize;
		std::deque< T > queue;
		
		Notifiable* receiver;
};
#endif
