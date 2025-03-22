#ifndef CONNECTION_SECRET_DOUBLEBUFFERINGDB_H
#define CONNECTION_SECRET_DOUBLEBUFFERINGDB_H
//connection_secret_DoubleBufferingDB.h

#include "connection_secret_baseDB_node.h"

#include <iostream>
#include <unistd.h>
#include <atomic>
#include <mutex>

using std::atomic;

class CONNECTION_SECRET_DOUBLEBUFFERING_DB{
public:
	std::mutex mtx;
	DB_BaseNode* NodeLIST;
	atomic<unsigned short> DB_version_No_;
	atomic<void*> read_DBNode;
	atomic<void*> write_DBNode;
	
	
public:
	CONNECTION_SECRET_DOUBLEBUFFERING_DB();
	
	void DB_insert(void* clientaddr, void* srcKey);
	int DB_getKey(void* clientaddr, void* KeyTarget);
	void DB_delete(void* clientaddr);
	unsigned short DB_getVersion();
	void DB_VersionUpdate();
};



#endif
