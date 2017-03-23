#include "precompiled.hpp"
#include "poller_base.hpp"
#include "i_poll_events.hpp"
#include "err.hpp"

zmq::poller_base_t::poller_base_t() {

}

zmq::poller_base_t::~poller_base_t() {
	/**
	 * Make sure thre is no more load on the shutdown
	 */ 
	zmq_assert(get_load() == 0); 
}

int zmq::poller_base_t::get_load() {
	return load.get();
} 

void zmq::poller_base_t::adjust_load(int amount) {
	if(amount > 0) 
		load.add(amount); 
	else if(amount < 0) 
		load.sub(-amount); 
}

void zmq::poller_base_t::add_timer(int timeout, i_poll_events* sink, int id) {
	uint64_t expiration = clock.now_ms() + timeout; 
	timers_info_t info = {sink, id}; 
	timers.insert(timers_t::value_type(expiration, info)); 
}

void zmq::poller_base_t::cancel_timer(i_poll_events* sink, int id) {
	/**
	 * Complexity of this operation is O(N). We assume it is rarely used 
	 */ 
	for(timer_t::iterator it = timers.begin(); it != timers.end(); ++it) {
		if(it->second.sink == sink && it->second.id == id) {
			timers.erase(it); 
			return;
		}
	}

	// Timers not found
	zmq_assert(false);
}

uint64_t zmq::poller_base_t::execute_timers() {
	// Fast track 
	if(timers.empty()) return 0; 

	// Get the current time 
	uint64_t current = clock.now_ms();

	// Execute the timers that are alrady deu 
	timers_t::iterator it = timers.begin(); 

	while(it != timers.end()) {
		/** 
		 * If we have to wai to execute the item, same will be true
		 * about all the following items 
		 */
		if(it->first > current)
			return it->first - current; 

		// Triger the timer 
		it->second.sink->timer_event(it->second.id); 

		// Remove it from the list of active timers 
		timer_t::iterator o = it; 
		++it; 
		timers.erase(o); 
	}
}



