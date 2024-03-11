/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_TEXTURE_MANAGER_HPP_
#define LAPP_TEXTURE_MANAGER_HPP_

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Type/csmVector.hpp>

/**
* @brief Texture Manager
*
* Load and maintain image textures
*/
class LAppTextureManager
{
public:

    /**
    * @brief Texture information struct
    */
    struct TextureInfo
    {
        GLuint id = 0;              ///  Texture ID
        int width = 0;              ///  Width
        int height = 0;             ///  Height
        std::string fileName;       ///  File name
    };

    /**
    * @brief Constructor
    */
    LAppTextureManager();

    /**
    * @brief Destructor
    *
    */
    ~LAppTextureManager();


    /**
    * @brief Perform pre-multiplication
    *
    * @param[in] red  
    * @param[in] green  
    * @param[in] blue 
    * @param[in] alpha  
    *
    * @return The color value after pre-multiplication
    */
    inline unsigned int Premultiply(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    {
        return static_cast<unsigned>(\
            (red * (alpha + 1) >> 8) | \
            ((green * (alpha + 1) >> 8) << 8) | \
            ((blue * (alpha + 1) >> 8) << 16) | \
            (((alpha)) << 24)   \
            );
    }

    /**
    * @brief Load image
    *
    * @param[in] fileName  Image file name
    * @return TextureInfo structure. NULL if the attempt fails
    */
    TextureInfo* CreateTextureFromPngFile(std::string fileName);

    /**
    * @brief Release all images
	*
    */
    void ReleaseTextures();

    /**
     * @brief Release the designated image (by ID)
     *
     * @param[in] textureId  The image texture to release
     **/
    void ReleaseTexture(Csm::csmUint32 textureId);

    /**
    * @brief Release the designated image (by name)
    *
    * @param[in] fileName  The image texture to release
    **/
    void ReleaseTexture(std::string fileName);

    /**
     * @brief (As the name suggests)
     *
     * @return  TextureInfo structure. May be NULL.
     */
    TextureInfo* GetTextureInfoById(GLuint textureId) const;

private:
    Csm::csmVector<TextureInfo*> _textures;
};


#endif