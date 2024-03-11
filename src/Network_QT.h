/*
 * @Author: Luke_lu
 * @Date: 2019-12-12 18:39:37
 * @LastEditTime: 2019-12-16 20:33:39
 * @Description: Main network class and Wrapper class
 */

#ifndef NETWORK_QT_H_
#define NETWORK_QT_H_


#include <nlohmann/json.hpp>
#include <vector>
#include <thread>
#include <guiddef.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <Base64.h>
//https://lowrey.me/guid-generation-in-c-11/
#include <sstream>
#include <random>
#include <string>
#include <qobject.h>
#include <qthread.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qtcpserver.h>
#include <Setting.h>
#include <detection.h>
#include <quuid.h>
#include <helper.hpp>
#include <QtNetwork/qudpsocket.h>
#include <Audio.h>
#include "acceptcall.h"

using namespace nlohmann;
std::string GenerateGuid();

typedef enum LatencyState { NORMAL, DELAY, DISCONNECTED };
class CallObj;

/**
 * @description: MotionObject Class containing all the information needed for GUI to display each call
 */
class MotionObject {
private:
	std::string sessionId;
	json motion;
	clock_t lastSuccessful;
	cv::Mat profile;
	const std::string modelID;
public:
	std::string name;
	bool alive;
	MotionObject() {//used by incoming calls
		this->alive = true;
		this->sessionId = GenerateGuid();
		lastSuccessful = clock();
	}
	MotionObject(std::string sessionId) {//used by calling
		this->alive = true;
		this->sessionId = sessionId;
	}

	/**
  * @description: Check and update the json object containing motion data
  * @param {string} received serialized json package
  * @return: success?
  */ 
 bool writeObject(std::string received) {
		json jtemp;
		try {
			jtemp = json::parse(received);
		}
		catch (json::exception e) {
			if (e.id == 101) {
				if (received == "") {
					//not Receiving
					qDebug() << "Empty package \n" << e.what();
					qDebug() << sessionId.c_str();
				}
				else {
					//data incorrect
					qDebug() << "Data incorrect \n" << e.what();
					qDebug() << sessionId.c_str();
				}
			}
			else {
				//unknown error
				qDebug() << "MotionObject Write unknown error \n" << e.what();
				qDebug() << sessionId.c_str();
			}

			return false;
		}
		motion.erase("data");
		motion["data"] = jtemp["data"];
		lastSuccessful = clock();
		return true;
	}

	json* getMotion() {
		return &motion;
	}

	/**
  * @description: Check the latency of current call
  * @param {void} 
  * @return: enum class LatencyState
  */ 
 LatencyState getLatencyState() {
		int delay = clock() -lastSuccessful;
		if (delay <= 300) {
			return NORMAL;
		}
		else if (delay > 300 && delay < 3000) {
			return DELAY;
		}
		else {
			return DISCONNECTED;
		}
	}

	bool setProfileByBase64(std::string base64) {
		profile = Base2Mat(base64);
		return profile.data == NULL;
	}

	std::string getSessionID(){
		return sessionId;
	}

};

/**
 * @description: Main network class using QT socket
 * Single Instance
 */
class Network_QT:public QObject {
	friend class AdvanceTCPSocket;
	Q_OBJECT;
public:
	bool networkInit();
	bool networkRestart();
	void updateMotion(json& motion);
	std::vector<MotionObject*>* getDisplayObjects();
	json* getSendJson();
	bool shouldStop() {
		return stopFlag;
	}
	
	/**
  	* @description: Accesser of network object
  	*/ 
 	static Network_QT* getInstance() {
		if (network_QT == nullptr)
			network_QT = new Network_QT();
		return network_QT;
	}
	void call(std::string, int);
	void call(std::string target);
	std::vector<CallObj*>* getCallObjs() {
		return &callObjs;
	}
public slots:
	void restartSlot() {
		networkRestart();
	}
signals:
	void restartSignal();

private:
	std::vector<MotionObject*> displayObjects  = std::vector<MotionObject*>(5);
	json sendObject;
	Network_QT();
	~Network_QT();
	Network_QT(Network_QT&) = delete;
	Network_QT& operator=(Network_QT) = delete;
	QTcpServer* listener;
	bool stopFlag;
	static Network_QT* network_QT;
	std::vector<QThread*> calls = std::vector<QThread*>();
	std::vector<CallObj*> callObjs = std::vector<CallObj*>();
	//Warning: All Network will end
	void stop() {
		stopFlag = true;
	}
	private slots:
		void ConnectHandler();
};


/**
 * @description: Call object handled by QThread
 */
class CallObj :public QObject {
	friend class Audio;
	Q_OBJECT;
	MotionObject* motion;
	QTcpSocket* s;
	json sendJson;
	int audioPort;
public:
	CallObj(MotionObject* motion, QTcpSocket* s,int audioPort);
	~CallObj();
	QTcpSocket* getSocket() {
		return s;
	}

	//TODO: Connect disconneted signal
	MotionObject* getMotionObject() {
		return motion;
	}

private slots:
	void sendObject() {
		sendJson.erase("data");
		sendJson["data"] = *Network_QT::getInstance()->getSendJson();
		s->write(sendJson.dump().c_str(), sendJson.dump().length());
	}
	void writeObject() {
		if(s->canReadLine())
			motion->writeObject(s->readLine().toStdString());
	}
};

class AdvanceTCPSocket:public QTcpServer {
	friend class Network_QT;
	void AdvanceTCPSocket::incomingConnection(qintptr socketDescriptor) {

	}
};
#endif // !NETWORK_QT_H_