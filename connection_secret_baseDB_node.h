#ifndef CONNECTION_SECRET_BASEDB_NODE_H
#define CONNECTION_SECRET_BASEDB_NODE_H
//connection_secret_baseDB_node.h

#include "connection_secret_val.h"
#include "connection_key.h"

#include <map>
#include <atomic>
//#include <mutex>

using std::atomic;

class DB_BaseNode{
public:
	std::map<connection_key, connection_sec_val*> Node_map;
	unsigned int Node_DBsize;
	DB_BaseNode* next;
	atomic<unsigned short> Node_AliveConnections;
	atomic<unsigned short> Node_version_No_;
//	std::mutex mtx;
public:
	DB_BaseNode();
	int Node_getKeyFrMap(void* clientaddr, void* KeyTarget);
	void Node_insertToMap(void* clientaddr, void* srcKey);
	void Node_deleteFrMap(void* clientaddr);
	unsigned short Node_getConnections();
	unsigned short Node_getVersionNo();
	void Node_VersionUpdate();
};





#endif
