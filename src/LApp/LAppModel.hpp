/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_MODEL_HPP_
#define LAPP_MODEL_HPP_

#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <ICubismModelSetting.hpp>
#include <Type/csmRectF.hpp>
#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>


/**
 * @brief  Implements CubismUserModel
 *         Model construction, model components management and model rendering
 *
 */
class LAppModel : public Csm::CubismUserModel
{
protected:
	static Csm::csmByte* CreateBuffer(const Csm::csmChar* path, Csm::csmSizeInt* size);
	void DeleteBuffer(Csm::csmByte* buffer, const Csm::csmChar* path = "");

public:
    /**
     * @brief Constructor
     */
    LAppModel();

    /**
     * @brief Destructor
     *
     */
    virtual ~LAppModel();

    /**
     * @brief Construct model by reading the model3.json file
     *
     */
    void LoadAssets(const Csm::csmChar* dir, const  Csm::csmChar* fileName);

    /**
     * @brief (As the name suggests)
     *
     */
    void ReloadRenderer();

	/**
	 * @brief   Draw the model
	 *
	 * @param[in]  matrix  View-Projection matrix
	 */
	void Draw(Csm::CubismMatrix44& matrix);


    /**
     * @brief   Set expression by the given ID
	 *
     */
    void SetExpression(const Csm::csmChar* expressionID);

    /**
     * @brief   (As the name suggests)
     *
     */
    void SetRandomExpression();


    /**
     * @brief   Get off-screen frame render buffer
     */
    Csm::Rendering::CubismOffscreenFrame_OpenGLES2& GetRenderBuffer();

protected:
    /**
     *  @brief  Perform draw
     *
     */
    void DoDraw();

	Csm::csmFloat32 _userTimeSeconds; ///  Total time in seconds

private:
    /**
     * @brief Setup model from the given setting data
     *
     * @param[in]   setting     ICubismModelSetting instance
     *
     */
    void SetupModel(Csm::ICubismModelSetting* setting);

    /**
     * @brief Setup textures for OpenGL texture units
     *
     */
    void SetupTextures();

    /**
     * @brief   Load a motion group according to the given group name and the setting data
     *
     * @param[in]   group  Name of the motion group
     */
    void PreloadMotionGroup(const Csm::csmChar* group);

    /**
     * @brief   Release a motion group according to the given group name and the setting data
     *
     * @param[in]   group  Name of the motion group
     */
    void ReleaseMotionGroup(const Csm::csmChar* group) const;

    /**
    * @brief Release all loaded motion objects
	*
    */
    void ReleaseMotions();

    /**
    * @brief Release all expression objects
	*
    */
    void ReleaseExpressions();

    Csm::ICubismModelSetting* _modelSetting; 
    Csm::csmString _modelHomeDir; 
    Csm::csmVector<Csm::CubismIdHandle> _eyeBlinkIds; /// 　Parameter ID for auto eye-blink
    Csm::csmVector<Csm::CubismIdHandle> _lipSyncIds; ///  Parameter ID for lip-sync
    Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>   _motions; ///  Loaded motion objects
    Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>   _expressions; ///  Loaded expression objects
    Csm::csmVector<Csm::csmRectF> _hitArea;
    Csm::csmVector<Csm::csmRectF> _userArea;
    const Csm::CubismId* _idParamAngleX; ///  Parameter ID: ParamAngleX
    const Csm::CubismId* _idParamAngleY; ///  Parameter ID: ParamAngleX
    const Csm::CubismId* _idParamAngleZ; ///  Parameter ID: ParamAngleX
    const Csm::CubismId* _idParamBodyAngleX; ///  Parameter ID: ParamBodyAngleX
    const Csm::CubismId* _idParamEyeBallX; ///  Parameter ID: ParamEyeBallX
    const Csm::CubismId* _idParamEyeBallY; ///  Parameter ID: ParamEyeBallXY

    Csm::Rendering::CubismOffscreenFrame_OpenGLES2  _renderBuffer;   ///  Off-screen frame render buffer
};


#endif
