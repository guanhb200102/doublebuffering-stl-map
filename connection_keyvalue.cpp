#include "connection_secret_val.h"
#include "connection_key.h"

connection_sec_val::connection_sec_val(void* srcclient_addr, void* srckey){
	memcpy(this->client_addr, srcclient_addr, 16);
	memcpy(this->key, srckey, 32);
}

connection_key::connection_key(void* arg){
	memcpy(this->client_addr, arg, 16);
}

