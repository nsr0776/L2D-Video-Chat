/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_VIEW_HPP_
#define LAPP_VIEW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Math/CubismMatrix44.hpp>
#include <Math/CubismViewMatrix.hpp>
#include "CubismFramework.hpp"
#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>


class TouchManager;
class LAppSprite;
class LAppModel;

/**
* @brief View Render Class
*/
class LAppView
{
public:

    /**
     * @brief The render target for LAppModel
     */
    enum SelectTarget
    {
        SelectTarget_None,                ///  Render default frame buffer
        SelectTarget_ModelFrameBuffer,    ///  Render the frame buffer maintained by each LAppModel
        SelectTarget_ViewFrameBuffer,     ///  Render the frame buffer maintained by each LAppView
    };

    /**
    * @brief Constructor
    */
    LAppView();

    /**
    * @brief Destructor
    */
    ~LAppView();

    /**
    * @brief Initialization
    */
    void Initialize();

    /**
    * @brief Perform render
    */
    void Render();

    /**
    * @brief Sprite initialization
    */
    void InitializeSprite();

    /**
    * @brief (As the name suggests)
    */
    void ResizeSprite();

    /**
    * @brief Transform device X coordinate to the corresponding View coordinate
    *
    * @param[in]       deviceX
    */
    float TransformViewX(float deviceX) const;

    /**
    * @brief Transform device Y coordinate to the corresponding View coordinate
    *
    * @param[in]       deviceY            
    */
    float TransformViewY(float deviceY) const;

    /**
    * @brief Transform device X coordinate to the corresponding Screen coordinate
    *
    * @param[in]       deviceX
    */
    float TransformScreenX(float deviceX) const;

    /**
    * @brief Transform device Y coordinate to the corresponding Screen coordinate
    *
    * @param[in]       deviceY
    */
    float TransformScreenY(float deviceY) const;

    /**
     * @brief   Called before drawing a model
     */
    void PreModelDraw(LAppModel& refModel);

    /**
     * @brief   Called after drawing a model
     */
    void PostModelDraw(LAppModel& refModel);

    /**
     * @brief   Decide the alpha value from other rendering targets
     */
    float GetSpriteAlpha(int assign) const;

    /**
     * @brief (As the name suggests)
     */
    void SwitchRenderingTarget(SelectTarget targetType);

    /**
     * @brief Set Clear Color when the rendering target is not using default render settings
     * @param[in]   r   Red(0.0~1.0)
     * @param[in]   g   Green(0.0~1.0)
     * @param[in]   b   Blue(0.0~1.0)
     */
    void SetRenderTargetClearColor(float r, float g, float b);

private:
    Csm::CubismMatrix44* _deviceToScreen;    ///  Device-to-screen matrix
    Csm::CubismViewMatrix* _viewMatrix;      ///  viewMatrix
    GLuint _programId;                       ///  Shader ID
    LAppSprite* _back;                       ///  Background image

    // The following are used when the rendering target is switched
    LAppSprite* _renderSprite;                                      
    Csm::Rendering::CubismOffscreenFrame_OpenGLES2 _renderBuffer;   
    SelectTarget _renderTarget;     
    float _clearColor[4];           
};

#endif