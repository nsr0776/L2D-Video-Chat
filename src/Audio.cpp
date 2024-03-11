/*
 * @Author: Luke_lu
 * @Date: 2019-12-12 18:39:37
 * @LastEditTime: 2019-12-15 20:57:51
 * @Description: file content
 */
#include <Audio.h>
#define MAX_AUDIO_LEN 960000 //如果接收缓冲区大于这个数值就剪掉
#define FRAME_LEN_60ms 960 //每一个语音帧长度是960字节
Audio* Audio::audio = nullptr;

Audio* Audio::getInstance()
{
	if (audio == nullptr)
		audio = new Audio();
	return audio;
}

/**
 * @description: Start the audio process
 * @param
 * @return: true if success
 */
bool Audio::audioStart()
{
    if (m_Output != nullptr && m_Input != nullptr)
        return false;
    return audioRestart();
}

/**
 * @description: Start or restart the audio process
 * @param
 * @return: true if success
 */
bool Audio::audioRestart() {
    bool status = true;
    if (m_Output != nullptr && m_Input != nullptr) {
        delete m_Output;
        delete m_Input;
    }
    int port = Setting::getSetting()->getAudioPort();
    status = audioSocket->bind(QHostAddress::Any, port);
    setaudioformat(8000, 1, 16);//TODO:add setting
    inputDevice = m_Input->start();
    outputDevice = m_Output->start();
    timer->start(60);
    QObject::connect(inputDevice, &QIODevice::readyRead, this, &Audio::onReadyRecord);
    QObject::connect(timer, &QTimer::timeout, this, &Audio::play);
    QObject::connect(audioSocket, &QUdpSocket::readyRead, this, &Audio::receiveData);
    qDebug() << "[Audio]audio is re/started" << endl;
    return status;
}

Audio::Audio()
{
	audioSocket = new QUdpSocket(this);
    timer = new QTimer(this);
    QObject::connect(this, &Audio::startSignal, this, &Audio::startSlot);

}

/**
 * @description: Set the sudio sampling format
 * @param {int} samplerate(Hz) e.g. 8000
 * @param {int} channelcount: how many channel e.g. 2 for Stereo
 * @param {int} samplesize(bit) e.g. 16 bit for normal, 32bit for high quality
 */
void Audio::setaudioformat(int samplerate, int channelcount, int samplesize) {
    format.setSampleRate(samplerate);
    format.setChannelCount(channelcount);
    format.setSampleSize(samplesize);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    QAudioDeviceInfo info = Setting::getSetting()->getInputDevice();
    QAudioDeviceInfo outfo = Setting::getSetting()->getOutputDevice();//XD
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }
    m_Input = new QAudioInput(info,format, this);
    m_Output = new QAudioOutput(outfo,format);

}

/**
 * @description: slot. Call when there is audio data to read from mic
 */
void Audio::onReadyRecord() {
    stream vp;
    memset(&vp.data, 0, sizeof(vp));

    // read audio from input device
    vp.lens = inputDevice->read(vp.data, 960);
    for (CallObj* callobj : *Network_QT::getInstance()->getCallObjs()) {
        if(callobj != nullptr && callobj->getSocket() != nullptr)
        if(callobj->getSocket()->state() == QTcpSocket::ConnectedState)
            audioSocket->writeDatagram((const char*)&vp, sizeof(vp), callobj->getSocket()->peerAddress(), callobj->audioPort);
    }

}

/**
 * @description: slot. Call every 60ms to play audio data receied
 * TODO: Mix audio from multiple sources
 */
void Audio::play() {
    if (m_PCMDataBuffer.size() < m_CurrentPlayIndex + FRAME_LEN_60ms) {//缓冲区不够播放60ms音频
        return;
    }
    char* writeData = new char[FRAME_LEN_60ms];
    memcpy(writeData, &m_PCMDataBuffer.data()[m_CurrentPlayIndex], FRAME_LEN_60ms);
    // 写入音频数据
    outputDevice->write(writeData, FRAME_LEN_60ms);
    m_CurrentPlayIndex += FRAME_LEN_60ms;
    //qDebug() << m_CurrentPlayIndex;
    delete[]writeData;

    //如果长度超过了MAX_AUDIO_LEN就从
    if (m_CurrentPlayIndex > MAX_AUDIO_LEN) {
        m_PCMDataBuffer = m_PCMDataBuffer.right(m_PCMDataBuffer.size() - MAX_AUDIO_LEN);
        m_CurrentPlayIndex -= MAX_AUDIO_LEN;
    }
}

/**
 * @description: slot. Call when there is audio data to read from socket
 * Data will be push into buffer
 */
void Audio::receiveData() {
    
    while (audioSocket->hasPendingDatagrams()) {
        QHostAddress senderip;
        quint16 senderport;
        stream vp;
        memset(&vp, 0, sizeof(vp));
        audioSocket->readDatagram((char*)&vp, sizeof(vp), &senderip, &senderport);
        //outputDevice->write(vp.data,vp.lens);
        addAudioBuffer(vp.data, vp.lens);
    }
}
void Audio::addAudioBuffer(char* pData, int len)
{
    //QMutexLocker locker(&m_Mutex);

    m_PCMDataBuffer.append(pData, len);
    //m_IsPlaying = true;
}

Audio::~Audio() {
    delete m_Input;
    delete inputDevice;
    delete m_Output;
    delete outputDevice;
    delete timer;
}