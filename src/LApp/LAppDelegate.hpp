/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_DELIGATE_HPP_
#define LAPP_DELIGATE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LAppAllocator.hpp"

#include <QObject>

class LAppView;
class LAppTextureManager;

/**
* @brief   Video window class
*   Manage Cubism SDK
*/
class LAppDelegate : public QObject
{
	Q_OBJECT;

public:
    /**
    * @brief   Return the singleton instance of this class
    */
    static LAppDelegate* GetInstance();

    /**
    * @brief   Release the singleton instance of this class
    *
    */
    static void ReleaseInstance();

    /**
    * @brief   OpenGL-related initializations
    */
    bool Initialize();

    /**
    * @brief   (As the name suggests)
    */
    void Release();

    /**
    * @brief　Register the shader
    */
    GLuint CreateShader();

    /**
    * @brief   Obtain information about the window
    */
    GLFWwindow* GetWindow() { return _window; }

    /**
    * @brief   Obtain information about the View
    */
    LAppView* GetView() { return _view; }

    /**
    * @brief   Is the video app ended
    */
    bool GetIsEnd() { return _isEnd; }

    /**
    * @brief   End the video application
    */
    void AppEnd() { _isEnd = true; }

    LAppTextureManager* GetTextureManager() { return _textureManager; }

	/**
	* @brief   Main loop that updates the window
	*/
	void Run();

	void emitStartSignal() { emit startSignal(); }

	int getPeerNumber() const { return _peerNumber; }

	void setPeerNumber(int no) { _peerNumber = no; }

private slots:

	void start() {
		Initialize();
		Run(); 
	}

signals:
	void startSignal();

private:
    /**
    * @brief   Constructor
    */
    LAppDelegate();

    /**
    * @brief   Destructor
    */
    ~LAppDelegate();

    /**
    * @brief   Initialization of Cubism SDK
    */
    void InitializeCubism();

    /**
     * @brief   Helper for CreateShader(). Perform error checks
     */
    bool CheckShader(GLuint shaderId);

	int _peerNumber;							 /// Peer number (<0 will refer to self)

    LAppAllocator _cubismAllocator;              ///  Cubism SDK Allocator
    Csm::CubismFramework::Option _cubismOption;  ///  Cubism SDK Option
    GLFWwindow* _window;                         ///  OpenGL window
    LAppView* _view;                             ///  View information
    bool _isEnd;                                 
    LAppTextureManager* _textureManager;         ///  テクスチャマネージャー

    int _windowWidth;                            ///  Window width. Set in Initialize()
    int _windowHeight;                           ///  Window height. Set in Initialize()

};

#endif