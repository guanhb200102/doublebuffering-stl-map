#include "connection_secret_baseDB_node.h"


DB_BaseNode::DB_BaseNode(){
	this->Node_DBsize=0;
	this->Node_AliveConnections.store(0, std::memory_order_relaxed);
	this->Node_version_No_.store(0, std::memory_order_relaxed);
}

int DB_BaseNode::Node_getKeyFrMap(void* clientaddr, void* KeyTarget){
	
	this->Node_AliveConnections.fetch_add(1, std::memory_order_relaxed);
	
	connection_key key(clientaddr);
	auto it=this->Node_map.find(key);
	
	if(it==this->Node_map.end()){
		this->Node_AliveConnections.fetch_sub(1, std::memory_order_relaxed);
		return -1;
	}
	
	if(it->second)
		memcpy(KeyTarget, it->second->key, 32);
	
	this->Node_AliveConnections.fetch_sub(1, std::memory_order_relaxed);
	return 1;
}

void DB_BaseNode::Node_insertToMap(void* clientaddr, void* srcKey){
	connection_key key(clientaddr);
	connection_sec_val* val=new connection_sec_val(clientaddr, srcKey);
	this->Node_map[key]=val;
	this->Node_DBsize++;
}

void DB_BaseNode::Node_deleteFrMap(void* clientaddr){
	connection_key key(clientaddr);
	auto it=this->Node_map.find(key);
	if(it!=this->Node_map.end()) {
		if(it->second) delete it->second;
		this->Node_map.erase(it);
	}
	this->Node_DBsize--;
}

unsigned short DB_BaseNode::Node_getConnections(){
	return this->Node_AliveConnections.load(std::memory_order_relaxed);
}

unsigned short DB_BaseNode::Node_getVersionNo(){
	return this->Node_version_No_.load(std::memory_order_relaxed);
}

void DB_BaseNode::Node_VersionUpdate(){
	this->Node_version_No_.fetch_sub(1, std::memory_order_relaxed);
}
