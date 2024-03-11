#ifndef COMMUNICATOR_HPP_
#define COMMUNICATOR_HPP_

#include <nlohmann/json.hpp>
#include <Utils/CubismJson.hpp>
#include <QObject>

/*
* @brief A class to obtain facial data. 
* Singleton.
*
*/
class Communicator : public QObject {
	Q_OBJECT;

private:
	struct FacialData {
		FacialData() = default;
		FacialData(bool isUpdated, const nlohmann::json* data)
			: isUpdated(isUpdated), data(data) { }

		bool isUpdated = false;
		const nlohmann::json* data = nullptr;
	};

public:
	static void releaseInstance();
	static Communicator* getInstance();

	const nlohmann::json* getFacialData(int no);

	void fetchPeerFacialData();

private:
	// Fetch and return a nlohmann-style json object valid for conversion
	// Return nullptr if the attempt fails
	const nlohmann::json* getSelfFacialData();

	const nlohmann::json* getPeerFacialData();


private:
	Communicator();
	Communicator(const Communicator&) = delete;
	~Communicator() {}
	
private:
	FacialData _selfFacialData;
	FacialData _peerFacialData;
	nlohmann::json _peerJsonCopy;

private slots:
	void fetchSelfFacialData();
};

#endif