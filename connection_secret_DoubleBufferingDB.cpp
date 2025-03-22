#include "connection_secret_DoubleBufferingDB.h"

CONNECTION_SECRET_DOUBLEBUFFERING_DB::CONNECTION_SECRET_DOUBLEBUFFERING_DB(){
	this->NodeLIST=new DB_BaseNode[2];
	if(!this->NodeLIST){
		std::cout << "this->NodeLIST" <<std::endl;
		exit(0);
	}
	this->NodeLIST->next=this->NodeLIST+1;
	this->NodeLIST->next->next=this->NodeLIST;
	
	this->read_DBNode.store(this->NodeLIST, std::memory_order_relaxed);
	this->write_DBNode.store(this->NodeLIST+1, std::memory_order_relaxed);
	this->DB_version_No_.store(0, std::memory_order_relaxed);
}

void CONNECTION_SECRET_DOUBLEBUFFERING_DB::DB_insert(void* clientaddr, void* srcKey){
	
	this->mtx.lock();
	DB_BaseNode* write_target=(DB_BaseNode*)this->write_DBNode.load(std::memory_order_relaxed);
	DB_BaseNode* read_target=(DB_BaseNode*)this->read_DBNode.load(std::memory_order_relaxed);
	
	while( write_target->Node_getConnections() != 0 ){
		usleep(100);
	}
	
	write_target->Node_insertToMap(clientaddr, srcKey);
	this->DB_VersionUpdate();
	write_target->Node_version_No_.store(this->DB_getVersion(), std::memory_order_relaxed);
	
	this->write_DBNode.store(read_target, std::memory_order_relaxed);
	this->read_DBNode.store(write_target, std::memory_order_relaxed);
	
	write_target=(DB_BaseNode*)this->write_DBNode.load(std::memory_order_relaxed);
	
	while( write_target->Node_AliveConnections.load(std::memory_order_relaxed) != 0 ){
		usleep(100);
	}
	
	write_target->Node_insertToMap(clientaddr, srcKey);
	write_target->Node_version_No_.store(this->DB_getVersion(), std::memory_order_relaxed);
	
	this->mtx.unlock();
}

int CONNECTION_SECRET_DOUBLEBUFFERING_DB::DB_getKey(void* clientaddr, void* KeyTarget){
	DB_BaseNode* read_target=(DB_BaseNode*) this->read_DBNode.load(std::memory_order_relaxed);
	
	while( read_target->Node_getVersionNo() != this->DB_getVersion() ){
		read_target=(DB_BaseNode*)this->read_DBNode.load(std::memory_order_relaxed);
		usleep(100);
	}
		
	
	int rest;
	rest= read_target->Node_getKeyFrMap(clientaddr, KeyTarget);
	return rest;
}

void CONNECTION_SECRET_DOUBLEBUFFERING_DB::DB_delete(void* clientaddr){
	this->mtx.lock();
	DB_BaseNode* write_target=(DB_BaseNode*)this->write_DBNode.load(std::memory_order_relaxed);
	DB_BaseNode* read_target=(DB_BaseNode*)this->read_DBNode.load(std::memory_order_relaxed);
	
	while( write_target->Node_getConnections() != 0 ){
		usleep(1000);
	}
	
	write_target->Node_deleteFrMap(clientaddr);
	this->DB_VersionUpdate();
	write_target->Node_version_No_.store(this->DB_getVersion(), std::memory_order_relaxed);
	
	this->write_DBNode.store(read_target, std::memory_order_relaxed);
	this->read_DBNode.store(write_target, std::memory_order_relaxed);
	
	write_target=(DB_BaseNode*)this->write_DBNode.load(std::memory_order_relaxed);
	
	while( write_target->Node_AliveConnections.load(std::memory_order_relaxed) != 0 ){
		usleep(1000);
	}
	
	write_target->Node_deleteFrMap(clientaddr);
	write_target->Node_version_No_.store(this->DB_getVersion(), std::memory_order_relaxed);
	
	this->mtx.unlock();
}

unsigned short CONNECTION_SECRET_DOUBLEBUFFERING_DB::DB_getVersion(){
	return this->DB_version_No_.load(std::memory_order_relaxed);
}

void CONNECTION_SECRET_DOUBLEBUFFERING_DB::DB_VersionUpdate(){
	this->DB_version_No_.fetch_add(1, std::memory_order_relaxed);
}


