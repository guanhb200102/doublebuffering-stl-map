#ifndef CONNECTION_KEY_H
#define CONNECTION_KEY_H
//connection_key.h

#include <cstring>
#include <stdlib.h>

class connection_key{
public:
	
	connection_key(void* arg);
	
	unsigned char client_addr[16];
	
	bool operator<(const connection_key& other) const {
        return std::memcmp(client_addr, other.client_addr, sizeof(client_addr)) < 0;
    }
};



#endif
