/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_DEFINE_HPP_
#define LAPP_DEFINE_HPP_

#include <CubismFramework.hpp>

/**
* @brief  Constants used by LApp
*
*/
namespace LAppDefine {

    using namespace Csm;

    extern const csmFloat32 ViewMaxScale;           ///  Maximum view scale
    extern const csmFloat32 ViewMinScale;           ///  Minimum view scale

    extern const csmFloat32 ViewLogicalLeft;        
    extern const csmFloat32 ViewLogicalRight;       

    extern const csmFloat32 ViewLogicalMaxLeft;     
    extern const csmFloat32 ViewLogicalMaxRight;    
    extern const csmFloat32 ViewLogicalMaxBottom;   
    extern const csmFloat32 ViewLogicalMaxTop;      

    extern const csmChar* ResourcesPath;         
    extern const csmChar* BackImageName;         

    // Models
    extern const csmChar* ModelDir[];               ///  Model names array
    extern const csmInt32 ModelDirSize;             

                                                    // Motion priority levels
    extern const csmInt32 PriorityNone;             
    extern const csmInt32 PriorityIdle;             
    extern const csmInt32 PriorityNormal;           
    extern const csmInt32 PriorityForce;            

    extern const csmBool DebugLogEnable;            

    // Cubism Framework log level
    extern const CubismFramework::Option::LogLevel CubismLoggingLevel;

    // Default window size
    extern const csmInt32 RenderTargetWidth;
    extern const csmInt32 RenderTargetHeight;
}

#endif