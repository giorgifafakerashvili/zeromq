#ifndef ZMQ_IP_H_
#define ZMQ_IP_H_

namespace zmq 
{

	// Same as socket(2), but allows for transparent tweaking the options. 
	fd_t open_socket(int domain, int type, int protocol); 

	// Sets the socket into non-blocking mode. 
	void unblock_socket(fd_t s); 

	// Enable IPv4-mapping of addresses in case it is disable 
	// by default 
	void enable_ipv4_mapping(fd_t s); 

	//Returns string representation of peer's address 
	// Socket sockfd must be connected. Returns true iff succesful
	int get_peer_ip_address(fd_t socket_fd, std::string& ip_addr); 

	//Sets the IP Type-Of-Service for the underlying socket 
	void set_ip_type_of_service(fd_t s, int iptos);

	//Sets the SO_NOSIGPIPE options for the underlying socket 
	//Return 0 on success, -1 if the connection has been closed by the peer 
	int set_nosigpipe(fd_t s); 

}

#endif