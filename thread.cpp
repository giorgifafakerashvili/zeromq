#include "precompiled.hpp"
#include "macros.hpp"
#include "thread.hpp"
#include "err.hpp"


static unsigned int __stdcall thread_routine(void* arg) {
	zmq::thread_t *self = (zmq::thread_t*) arg; 
	self->tfn(self->arg); 
	return 0;
} 

void zmq::thread_t::start(thread_fn* tfn, void* arg) {
	this->tfn = tfn; 
	this->arg = arg; 
	descriptor = (HANDLE) _beginthreadex(NULL, 0, 
			&thread_routine, this, 0, NULL); 
	
}