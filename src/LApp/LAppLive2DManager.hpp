/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_LIVE2D_MANAGER_HPP_
#define LAPP_LIVE2D_MANAGER_HPP_

#include <CubismFramework.hpp>
#include <Math/CubismMatrix44.hpp>
#include <Type/csmVector.hpp>
#include "Model.hpp"

class LAppModel;

/**
* @brief Manage a Live2D model including its
*		 construction, destruction and real-time rendering
*/
class LAppLive2DManager
{

public:
    /**
    * @brief   Return the singleton instance of this class
    */
    static LAppLive2DManager* GetInstance();

    /**
    * @brief   Release the singleton instance of this class
    *
    */
    static void ReleaseInstance();

    /**
    * @brief   Return the model managed in the current scene
    *
    * @return  Return the instance of the managed model. May be nullptr
    */
	Model* getModel() const { return _model; }


    /**
    * @brief   Release the model managed in the current scene
    *
    */
    void ReleaseModel();

    /**
    * @brief   Called when the scene is updated
    *          Update the model parameters and re-render it
    */
    void OnUpdate() const;

    /**
    * @brief   Load the main scene
    */
    void LoadScene(Csm::csmInt32 index);

private:
    /**
    * @brief  Constructor
    */
    LAppLive2DManager();

    /**
    * @brief  Destructor
    */
    virtual ~LAppLive2DManager();

    Csm::CubismMatrix44*        _viewMatrix; /// View matrix for model rendering
	Model* _model = nullptr;				 /// The managed model
};


#endif
