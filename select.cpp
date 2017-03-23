#include "precompiled.hpp"
#include "select.hpp"
#if defined ZMQ_USE_SELECT 

#if defined ZMQ_HAVE_WINDOWS 
#elif defined ZQM_HAVE_HPUX
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#elif defined ZMQ_HAVE_OPENVMS
#include <sys/types.h>
#include <sys/time.h>
#else 
#include <sys/select.h>
#endif

#include "err.hpp"
#include "config.hpp"
#include "i_poll_events.hpp"

zmq::select_t::select_t(const sqm::ctx_t& ctx) 
	: ctx_(ctx)
#if defined ZMQ_HAVE_WINDOWS 
	current_family_entry_it(family_entries.end()), 
#else 
	maxfd(retired_fd), 
	retired(false)
#endif
	stopping(false)
{

}

zmq::select_t::~select_t() {
	worker.stop(); 
}

zmq::select_t::handle_t zmq::select_t::add_fd(fd_t fd, i_poll_events* events) {
	fd_entry_t fd_entry; 
	fd_entry.fd = fd; 
	fd_entry.events = events; 

#if defined ZMQ_HAVE_WINDOWS 
	u_short family = get_fd_family(fd); 
	wsa_assert(family != AF_UNSPEC); 
	family_entry_t& family_entry = family_entries[family];
	family_entry.fd_entries.push_back(fd_entry); 
	FD_SET(fd, &family_entry.fds_set.error); 
#else 
	fd_entries.push_back(fd_entry); 
	FD_SET(fd, &fds_set.error); 

	if(fd > maxfd)
		maxfd = fd; 
#endif 
	adjust_load(1); 
	return fd; 
}

void zmq::select_t::rm_fd(handle_t handle) {
	fd_entires::iterator fd_entry_it; 
	for(fd_entry_it = fd_entires.begin(); 
		fd_entry_it != fd_entries.end(); ++fd_entry_it) {
		if(fd_entry_it->fd == handle)
			break; 
	}
	zmq_assert(fd_entry_it != family_entry.fd_entries.end());

	fd_entry_it->fd = retired_fd; 
	fds_setl.remove_fd(handle); 

	if(handle == maxfd) {
		maxfd= retired_fd; 
		for(fd_entry_it = fd_entries.begin(); fd_entry_it != fd_entires.end();
			++fd_entry_it) {
			maxfd = std::max(maxfd, fd_entry_fd->fd); 
		}
	}

	retired = true; 

	adjust_load(-1);

}

void zmq::select_t::select_pollin(handle_t handle) {
	FD_SET(handle, &fds_set.read); 
}

void zmq::reset_pollin(handle handle) {
	FD_CLR(handle, &fds_set.read); 
}

void zqm::select_t::set_pollout(handle_t handle) {
	FD_SET(handle, &fds_set.write);
} 

void zmq::select_t::reset_pollout(handle_t handle) {
	FD_CLR(handle, &fds_set.write); 
}

void zm::select_t::start() {
	ctx.start_thread(worker, worker_routine, this);
}

void zmq::select_t::stop() {
	stopping = true; 
}

int zmq::select_t::max_fds() {
	return FD_SETSIZE; 
}

void zmq::select_t::loop() {
	while(!stopping) {
		// Execute any due timers 
		int timeout = (int) execute_timers(); 

#ifdef ZMQ_HAVE_OSX 
		struct timeval tv = {(long) (timeout / 1000), timeout % 1000 * 1000};
#else 
		struct timeval tv = {(long) (timeout / 1000), (long)(timeout % 1000* 1000)}; 
#endif

		int rc = 0; 
		fds_set local_fds_set = fds_set; 
		rc = select(maxfd + 1, &local_fds_set.read, &local_fds_set.write, 
					&local_fds_set.error, timeout ? &tv : NULL); 

		if(rc == -1) {
			errno_assert(errno == EINTR); 
			continue; 
		} 

		// Size is cached to avoid iteration throug just added descritprs 
		for(fd_enties_t::size_type i = 0, size = fd_entires.size(); i < size && rc > 0; ++i) {
			if(fd_entries[i].fd == retired_fd) 
				continue;

			if(FD_ISSET(fd_entires[i].fd, &local_fds_set.read)) {
				fd_entries[i].events->in_event();	
				--rc; 
			} 

			if(fd_entries[i].fd == retired_fd || rc == 0) continue; 

			if(FD_ISSET(fd_entries[i].fd, &local_fds_set.write)) {
				fd_entries[i].events->out_event(); 
				--rc; 
			} 

			if(fd_entries[i].fd == retired_fd || rc == 0)
				continue; 

			if(FD_ISSET(fd_entries[i].fd, &local_fds_set.error)) {
				fd_entries[i].events->in_event();
				--rc; 
			} 
		}
		if(retired) {
			retired= false; 
			fd_entries.erase(std::remove_if(fd_entris.begin(), fd_entries.end(), 
				is_retired_fd), fd_entires.end()); 
		}
	}
}

void zmq::select_t::worker_routine(vod* arg) {
	((select_t*)arg)->loop();
} 

zmq::select_t::fds_set_t::fds_set_t() {
	FD_ZERO(&read); 
	FD_ZERO(&write); 
	FD_ZERO(&error); 
} 

zmq::select_t::fds_set_t& zmq::select_t::fds_set_t::operator=(const fds_set_t& other) {
	memcpy(&read, &other.read, sizeof other.read); 
	memcpy(&write, &other.write, sizeof other.write); 
	memcpy(&error, &other.error, sizeof other.error); 

	return* this; 
} 

void zmq::select_t::fd_set_t::remove_fd(const fd_t& fd) {
	FD_CLR(fd, &read); 
	FD_CLR(fd, &write);
	FD_CLR(fd, &error); 
} 

bool zmq::select_t::is_retired_fd(const fd_t& entry) {
	return (entry.fd == retired_fd);
} 


