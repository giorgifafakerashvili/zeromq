#ifndef ZMQ_THREAD_H_
#define ZQM_THREAD_H_

#ifndef ZMQ_HAVE_WINDOWS
	#include <pthread.h>
#endif

namespace zmq 
{

typedef void (thread_fn)(void*);


class thread_t 
{
 public:
 	inline thread_t()
 		: tfn(NULL)
 		, arg(NULL)
 	{

 	} 

 	/**
 	 * Creates OS thread. 'tfn' is main 
 	 */
 	void start(thread_fn* tfn, void* arg);

 	void stop();

 	void setSchedulingParameters(int priority, int scheduling_policy);

 	void setThreadName(const char* name); 

 	thread_fn* tfn; 
 	void* arg; 

 private:
 	pthread_t descriptor; 

 	thread_t(const thread_t&);
 	const thread_t& operator=(const thread_t&);
 	


};


}

#endif