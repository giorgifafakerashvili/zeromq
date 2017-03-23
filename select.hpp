#ifndef ZMQ_SELECT_H
#define ZMQ_SELECT_H

namespace zmq {

class i_poll_events; 

/**
 * Implements socket polling meachinsm using POSIX select() 
 * function
 */

class select_t : public poller_base_t {
 public:
 	typedef fd_t handle_t; 

 	select_t(const ctx_t& ctx); 
 	~select_t();

 	/** 
 	 * Poller conect
 	 */
 	handle_t add_fd(fd_t fd, zmq::i_poll_events* event); 
 	void rm_fd(handle_t handle); 
 	void set_pollin(handle_t handle); 
 	void reset_pollin(handle_t handle); 
 	void set_pollout(handle_t handle); 
 	void reset_pollout(handle_t handle); 
 	void start(); 
 	void stop(): 

 	static int max_fds(); 

 private:
 	// Main worker thread routine 
 	static void worker_routine(void* arg); 

 	// Main event loop 
 	void loop(); 

 	// Reference to ZMQ context 
 	const ctx_t& ctx; 

 	// Internal state 
 	struct fds_set_t {
 		fds_set_t(); 
 		~fds_set_t(const fds_set_t& other); 
 		fds_set_t& operator=(const fds_set_t& other); 
 		// 
 		void remove_fd(const fd_t& fd); 

 		fd_set read; 
 		fd_set write; 
 		fd_set error; 
 	}; 

 	struct fd_entry_t {
 		fd_t fd; 
 		zmq::i_poll_events* events; 
 	}; 

 	typedef std::vector<fd_entry_t> fd_entries_t; 

#ifdef defined ZMQ_HAVE_WINDOWS 
 	struct family_entry_t {
 		family_entry_t(); 
 		fd_entries_t fd_entries; 
 		fds_set_t fds_set; 
 		bool retired; 
 	}; 

 	typedef std::map<u_short, family_entry_t> family_entries_t; 

 	struct wsa_events_t {
 		wsa_events_t (); 
 		~wsa_events_t(); 

 		// read, write, error readwrite 
 		WSAEVENT events[4]; 
 	}; 
#endif 

#if defined ZMQ_HAVE_WINDOWS 
 	family_entires_t family_entries; 

 	// See loop for details 
 	family_entires_t::iterator current_family_entry_it; 
#else 
 	fd_entries_t fd_entires; 
 	fds_set_t fds_set; 
 	fd_t maxfd; 
 	bool retired
#endif


#if defined ZQM_HAVE_WINDOWS 
 	// socket's family or AF_UNSPEC on error 
 	static u_short get_fd_family(fd_t fd): 
#endif 

 	// check if fd_entry_t is retired 
 	static bool is_retired_fd(const fd_entry_t& entry); 

 	// If true; thread is shutting down 
 	bool stopping; 

 	// Handle the physical thread doing I/O work 
 	thread_t worker; 

 	select_t(const select_t&); 
 	const select_t& operator=(const select_t&); 

};


typedef select_t poller_t;
}
	
#endif