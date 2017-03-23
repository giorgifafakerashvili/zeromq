#ifndef ZMQ_POLLER_BASE_H_
#define ZMQ_POLLER_BASE_H_

namespace zmq 
{

class poller_base_t {
 public:
 	poller_base_t();
 	virtual ~poller_base_t(); 

 	/** 
 	 * Returns load of the poller. Note that this function
 	 * can be invoked from a different thread!
 	 */
 	int get_load(); 

 	/**
 	 * Add a timeout to expire in timer_out milliseconds. After 
 	 * the expiration timer_event on sink_object will be called with 
 	 * argument set to id.
 	 */
 	void add_timer(int timeout, zmq::i_poll_events* sink, int id); 

 	/** 
 	 * Cancell the timer created by sink object with ID equal to di 
 	 */ 
 	void cancel_timer(zmq::i_poll_events* sink, int id); 
 protected:
 	/**
 	 * Called by individual poller implementations to manage the load
 	 */ 
 	void adjust_load(int timeout); 

 	/** 
 	 * Execute any timers that are done arudue. Retursn number of millisecods 
 	 * to wait to match the next timer or 0 meaning "no timers'
 	 */
 	uint64_t execute_timers(); 

 	private:
 		clock_t clock; 

 		/** 
 		 * List of active timers 
 		 */ 
 		struct timer_info_t {
 			zmq::i_poll_events* sink; 
 			int id; 
 		}; 

 		typedef std::multimap<uint64_t,timer_info_t> timers_t; 
 		timer_t timers; 

 		/**
 		 * Load of the poller. Currently the number of fiel descritpors 
 		 * registered
 		 */
 		atomic_counter_t load; 

 		poller_base_t(const poller_base_t&); 
 		const poller_base_t& operator=(const poller_base_t&); 
};


}

#endif