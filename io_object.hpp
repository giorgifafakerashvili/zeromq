#ifndef ZMQ_IO_OBJECT_H
#define ZMQ_IO_OBJECT_H



#include "i_poll_events.hpp"
#include "poller.hpp"



namespace zmq 
{
	
/** 
 * Forward declaration of thread_t class
 */ 
class io_thread_t; 


/**
 * Simple base class for objects that live in I/O threads. 
 * It makes communication with the poller object easier and 
 * makes defining unneeded event handlers uncessrely
 */ 
class io_object_t : public i_poll_events {
 public:
 	io_object_t(zmq::io_thread_t* io_thread = nullptr); 
 	~io_object_t();

 	/** 
 	 * When migrating an object from one I/O thread to another, first 
 	 * unplug it, then migrate it, then plug it to the new thread 
 	 */ 
 	void plug(zmq::io_thread_t* io_thread); 
 	void unplug();

 protected:
 	typedef poller_t::handle_t handle_t; 

 	/** 
 	* Methods to access underlying poller object
 	*/
 	handle_t add_fd(fd_t fd); 
 	void rm_fd(handle_t handle); 
 	void set_pollin(handle_t handle); 
 	void reset_pollin(handle_t handle); 
 	void reset_pollout(handle_t handle); 
 	void add_timer(int timeout, int id); 
 	void cancel_timer(int id); 

 	/** 
 	 * i_poll_events interface implememntation
 	 */ 
 	void in_event(); 
 	void out_event(); 
 	void timer_event(int id); 

 private:
 	poller_t* poller_;

 	io_object_t(const io_object_t&); 
 	const io_object_t& operator=(const io_object_t&);


};


}

#endif