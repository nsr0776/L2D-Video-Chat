#include "Communicator.hpp"
#include "detection.h"
#include "Network_QT.h"


using namespace Csm;

Communicator* s_instance = nullptr;

Communicator::Communicator() {
	QObject::connect(FacialLandmarkDetector::getInstance(), &FacialLandmarkDetector::NewDetection,
					this, &Communicator::fetchSelfFacialData);
}

void Communicator::releaseInstance() {
	delete s_instance;
}

Communicator* Communicator::getInstance() {
	if (s_instance == nullptr) {
		s_instance = new Communicator();
	}
	return s_instance;
}

const nlohmann::json* Communicator::getSelfFacialData() {
	if (!_selfFacialData.isUpdated) {
		return nullptr;
	}
	else {
		_selfFacialData.isUpdated = false;
		return _selfFacialData.data;
	}
}

const nlohmann::json* Communicator::getPeerFacialData() {
	if (!_peerFacialData.isUpdated) {
		return nullptr;
	}
	else {
		_peerFacialData.isUpdated = false;
		return _peerFacialData.data;
	}
}

const nlohmann::json* Communicator::getFacialData(int no) {
	if (no < 0) {
		return getSelfFacialData();
	}
	else {
		return getPeerFacialData();
	}
}

void Communicator::fetchSelfFacialData() {
	const nlohmann::json* temp = Network_QT::getInstance()->getSendJson();
	Network_QT::getInstance()->getCallObjs();
	if (temp == nullptr
		|| (*temp)["data"].is_null()) {
		// do nothing
	}
	else {
		_selfFacialData = FacialData(true, temp);
	}
}

void Communicator::fetchPeerFacialData() {
	std::vector<CallObj*>* callObjs = Network_QT::getInstance()->getCallObjs();

	// Loop through the vector to locate the first valid callObj
	for (const auto callObj : *callObjs) {
		if (callObj != nullptr && callObj->getSocket() != nullptr) {
			if (callObj->getSocket()->state() == QTcpSocket::ConnectedState) {

				auto motion = callObj->getMotionObject()->getMotion();

				if (motion == nullptr || (*motion)["data"].is_null()) {
					return;
				}
				else {
					_peerJsonCopy = *motion;
					_peerFacialData = FacialData(true, &_peerJsonCopy);
					return;
				}

			}
		}
	}
}