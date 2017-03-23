#ifndef ZMQ_POLLER_H_
#define ZQM_POLLER_H_

#if defined ZMQ_USE_KQUEUE
	#include "kqueue.hpp"
#elif defined ZMQ_USE_EPOLL
	#include "epoll.hpp"
#elif defined ZMQ_USE_DEVPOLL
	#include "devpoll.hpp"
#elif defined ZMQ_USE_POLL
	#include "poll.hpp"
#elif defined ZMQ_USE_POLLSET
	#include "pollset.hpp"
#elif defined ZMQ_USE_SELECT 
	#include "select.hpp"
#elif defined ZMQ_HAVE_GNU
	#define ZMQ_USE_POLL
	#include "poll.hpp"
#endif 

#if defined ZMQ_USE_SELECT 
	#define ZMQ_POLL_BASED_ON_SELECT 
#else 
	#define ZMQ_POLL_BASED_ON_POLL
#endif



#endif