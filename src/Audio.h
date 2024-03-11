/*
 * @Author: Luke_lu
 * @Date: 2019-12-12 18:39:37
 * @LastEditTime: 2019-12-15 19:37:58
 * @Description: Audio processing and sending thread
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <QtNetwork/qudpsocket.h>
#include <Network_QT.h>
#include <Setting.h>
#include <QtMultimedia/qaudio>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <qtimer.h>
#include <qmutex.h>
class Audio: public QObject {

	Q_OBJECT;
public:
	QUdpSocket* audioSocket;
	static Audio* audio;
	static Audio* getInstance();
	bool audioStart();
	bool audioRestart();
private:
	Audio();
	Audio(Audio&) = delete;
	Audio& operator=(Audio) = delete;
	void setaudioformat(int samplerate, int channelcount, int samplesize);

	QAudioInput* m_Input = nullptr;
	QIODevice* inputDevice = nullptr;
	QAudioOutput* m_Output = nullptr;
	QIODevice* outputDevice = nullptr;

	QAudioFormat format;
	QTimer* timer;
	QByteArray m_PCMDataBuffer;
	int m_CurrentPlayIndex = 0;
	QMutex m_Mutex;
	void addAudioBuffer(char* pData, int len);

	~Audio();
signals:
	void startSignal();

private slots:
	void onReadyRecord();
	void play();
	void receiveData();
	void startSlot() {
		this->audioStart();
	}
};
struct stream {
	int lens;
	char data[960];
};
/*
Some code and idea from 
https://blog.csdn.net/hanzhen7541/article/details/98794555
https://github.com/Wzing0421/QTAudio


*/

#endif
