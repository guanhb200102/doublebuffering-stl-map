#ifndef CONNECTION_SECRET_VAL_H
#define CONNECTION_SECRET_VAL_H

//connection_secret_val.h

#include <cstring>
#include <stdlib.h>



class connection_sec_val{
public:
	unsigned char client_addr[16];
	unsigned char key[32];
	connection_sec_val(void* srcclient_addr, void* srckey);
};



#endif
