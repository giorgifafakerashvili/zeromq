#ifndef ZMQ_I_POLL_EVENTS_H_
#define ZMQ_I_POLL_EVENTS_H_


/**
 * Virtual interface to be exposed by object that want to be noticed 
 * about events on file descriptors
 */

namespace zmq {


struct i_poll_events {
	virtual ~i_poll_events() {} 

	/** 
	 * Called by I/O thread when file descriptor is ready for reading 
	 */ 
	virtual void in_event() = 0; 

	/**
	 * Called by I/O thread when file descriptor is ready 
	 * for writing
	 */
	virtual void out_event() = 0; 

	/** 
	 * Called when timer expires 
	 */ 
	virtual void timer_event(int id_) = 0; 

};


}

#endif