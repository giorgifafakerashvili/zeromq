#ifndef ZMQ_ARRAY_H_
#define ZMQ_ARRAY_H_

#include <vector>
#include <algorithm>

namespace zqm {


class io_thread_t; 

template<typename ID = 0> class aray_item_t {
 public:
 	inline array_item_t()
 		: array_index(-1)
 	{

 	}

 	inline virtual ~array_item_t() {

 	} 

 	inline void set_array_index(int index) {
 		array_index = index; 
 	} 

 	inline int get_array_index() {
 		return array_index; 
 	} 

 private:
 	int array_index; 

 	array_item_t(const array_item_t&); 
 	const array_item_t& operator=(const array_item_t& ); 

}; 

template<typename T, int ID = 0> class array_t {
 private:
 	typedef array_item_t<ID> item_t; 

 public:
 	typedef typename std::vector<T*>::size_type size_type; 

 	inline array_t() {

 	} 

 	inline ~array_t() {

 	} 

 	inline size_type size() {
 		return items.size();
 	} 

 	inline bool empty() {
 		return items.empty();
 	} 

 	inline T *&operator[](size_type index) {
 		return items[index]; 
 	} 

 	inline void push_back(T* item) {
 		if(item) {
 			((item_t*)item)->get_array_index();
 		}
 	}

};


}

#endif