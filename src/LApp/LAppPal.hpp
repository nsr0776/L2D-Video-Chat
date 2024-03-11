/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_PAL_HPP_
#define LAPP_PAL_HPP_

#include <CubismFramework.hpp>
#include <string>


#include <Utils/CubismJson.hpp>
#include <nlohmann/json.hpp>


/**
* @brief A helper class for Cubism-related functions
*
*/
class LAppPal
{
public:
    /**
    * @brief Load a file as a byte array
    *
    * @param[in]   filePath    Path to the file
    * @param[out]  outSize     Size of the loaded buffer to be assigned. (A mimic of the C# syntax)
    * @return                  Loaded binary file buffer
    */
    static Csm::csmByte* LoadFileAsBytes(const std::string filePath, Csm::csmSizeInt* outSize);


    /**
    * @brief Release the buffer created by LAppPal::LoadFileAsBytes
    *
    * @param[in]   byteData    The buffer to release
    */
    static void ReleaseBytes(Csm::csmByte* byteData);

    /**
    * @biref   Return the delta time (the time difference from the last frame)
    *
    */
    static Csm::csmFloat32 GetDeltaTime();

    static void UpdateTime();

    /**
    * @brief Write log to the console. Usage is similar to C-style printf
    *
    */
    static void PrintLog(const Csm::csmChar* format, ...);

    /**
    * @brief Write message to the console
    *
    */
    static void PrintMessage(const Csm::csmChar* message);

private:
    static double s_currentFrame;
    static double s_lastFrame;
    static double s_deltaTime;
};

#endif