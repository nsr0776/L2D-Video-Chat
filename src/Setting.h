/*
 * @Author: Luke_lu
 * @Date: 2019-12-12 18:39:38
 * @LastEditTime: 2019-12-14 21:20:49
 * @Description: A single instance Setting class to store and get settings
 */

#ifndef SETTING_H_
#define SETTING_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <QtMultimedia/QAudioDeviceInfo>
class Setting
{

public:
	static Setting *getSetting()
	{
		if (!Setting::inited)
			setting = new Setting();
		return setting;
	}
	short const getCameraID()
	{
		return cameraID;
	}

	/**
   * @description: Check and set the camera avalibility
   * @param {short} Device ID 
   * @return: State
   */
	bool setCameraID(short ID)
	{
		cv::VideoCapture camera(ID);
		bool temp = false;
		if (camera.isOpened())
		{
			this->cameraID = ID;
			temp = true;
		}
		camera.release();
		return temp;
	}

	short const getListenPort()
	{
		return this->listenPort;
	}
	bool setListenPort(short port)
	{
		this->listenPort = port;
		return true;
	}

	short const getAudioPort()
	{
		return this->audioPort;
	}
	bool setAudioPort(short port)
	{
		this->audioPort = port;
		return true;
	}
	short const getCallPort()
	{
		return this->callPort;
	}
	bool setCallPort(short port)
	{
		this->callPort = port;
		return true;
	}

	short const getMaximumListenQueue()
	{
		return this->maximumListenQueue;
	}
	void setMaximumListenQueue(short maximumListenQueue)
	{
		this->maximumListenQueue = maximumListenQueue;
	}

	/**
  * @description: Check and set profile photo
  * @param {string} Path to the photo 
  * @return: success?
  */
	bool setProfile(const std::string addr)
	{

		//Probably some translate? / \?

		profilePhoto = cv::imread(addr);

		if (profilePhoto.data == NULL)
			return false;
		profiletype = addr.substr(addr.find_last_of('.'), addr.length() - addr.find_last_of('.'));
		pathToProfile = addr;
		return true;
	}
	std::string getProfilePath()
	{
		return pathToProfile;
	}

	cv::Mat getProfile()
	{
		return profilePhoto;
	}
	std::string getProfileType()
	{
		return profiletype;
	}
	std::string getName()
	{
		return name;
	}
	bool setName(std::string name)
	{
		if (name.length() < 4 || name.length() > 16)
			return false;
		this->name = name;
		return true;
	}

	std::string getModelID()
	{
		return modelID;
	}
	/**
  * @description: Check and set the model using
  * @param {string} modelID
  * @return: success?
  TODO
  */
	bool setModelID(std::string ModelID = "")
	{
		this->modelID = ModelID;
		return true;
	}
	void setInputDevice(QAudioDeviceInfo &device)
	{
		inputDevice = device;
	}
	void setOutputDevice(QAudioDeviceInfo &device)
	{
		outputDevice = device;
	}
	QAudioDeviceInfo getOutputDevice()
	{
		return outputDevice;
	}
	QAudioDeviceInfo getInputDevice()
	{
		return inputDevice;
	}
	/*
	bool getDebugMode() {
		return debug;
	}
	void setDebugMode(bool mode) {
		debug = mode;
	}
*/
private:
	Setting()
	{
		inited = true;
		listenPort = 1919;
		callPort = 1145;
		maximumListenQueue = 5;
		audioPort = 893;
		name = "Leader1";
		modelID = "Tasho koji";
		debug = false;
		outputDevice = QAudioDeviceInfo::defaultOutputDevice();
		inputDevice = QAudioDeviceInfo::defaultInputDevice();
		pathToProfile = "..\\..\\res\\defaultProfile.jpg";
		showCamera = false;
		ShowFR = false;
	};
	Setting(Setting &);
	static bool inited;
	short cameraID;
	short listenPort;
	short callPort;
	short audioPort;
	short maximumListenQueue;
	static Setting *setting;
	std::string name;
	std::string modelID;
	cv::Mat profilePhoto;
	std::string profiletype;
	std::vector<std::string> MACBlackList;
	QAudioDeviceInfo outputDevice;
	QAudioDeviceInfo inputDevice;
	std::string pathToProfile;

public:
	bool debug;
	bool showCamera;
	bool ShowFR; // Show facial recognization image
};

#endif