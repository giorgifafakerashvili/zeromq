#ifndef ZMQ_CTX_H_
#define ZMQ_CTX_H_

namespace zmq 
{

class object_t; 
class io_thread_t; 
class socket_base_t; 
class reaper_t; 
class pipe_t; 

struct endpoint_ {
	socket_base_t* socket; 
	options_t options; 
}; 

class ctx_t {
 public:
 	// create 
 	ctx_t();


 	bool check_tag(); 

 	int terminate(); 

 	int shutdonw(); 

 	int set(int option, int optval); 
 	int get(int option);

 	zmq::socket_base_t* create_socket(int type); 
 	void destroy_socket(zmq::socket_base_t* soket); 

 	void start_thread(thread_t& thread, thread_fn* tfn, void* arg) const; 

 	// Send command to the destination thread 
 	void send_command(uint32_t tid, const command_t& command); 


 	zmq::io_thread_t* choose_io_thread(uint64_t affinity); 

 	// returns reaper thread object 
 	zmq::object_t* get_reaper();

 	// Management of inproc endpoints 
 	int register_endpoint(const char* addr, const endpoint_t& endpoint); 
 	int unregister_endpoint(const std::string& addr, socket_base_t* socket);	
 	void unregister_endpoints(zmq::socket_base_t* socket); 
 	void pend_connection(const std::string& addr, 
 											 const endpoint_& endpoint, pipe_t** pipes); 
 	void connect_pending(const char* addr, zmq::socket_base_t* bind_socket); 


 	enum {
 		term_tid = 0, 
 		reaper_tid = 1; 
 	}; 

 	~ctx_t();

 private:
 	struct pending_connection_t {
 		endpint_t endpoint; 
 		pipe_t* connect_pipe; 
 		pipe_t* bind_pipe; 
 	}; 

 	// Used to check whether the object is a context 
 	uint32_t tag; 
 	typedef array_t<socket_base_t> sockets_t; 
 	sockets_t sockets; 

 	// List of unused thread slots 
 	typedef std::vector<uin32_t> empty_slots_t; 
 	empty_slots_t empty_slots; 

 	//
 	bool starting;

 	bool terminatig; 

 	mutex_t slot_sync; 

 	zmq::reaper_t* reaper; 

 	//I/O threads 
 	typedef std::vector<zmq::io_thread*> io_threads_t; 
 	io_threads_t io_threads; 

 	
};

}


#endif