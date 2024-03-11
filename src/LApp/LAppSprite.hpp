/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_SPRITE_HPP_
#define LAPP_SPRITE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>


/**
* @brief Sprite implementation
*
* Manage texture ID & rectangles
*
*/
class LAppSprite
{
public:
    /**
    * @brief Rect struct (Rectangle)
    */
    struct Rect
    {
    public:
        float left;     ///  The left side
        float right;    ///  ditto.
        float up;       ///  ditto.
        float down;     ///  ditto.
    };

    /**
    * @brief Constructor
    *
    * @param[in]       x            
    * @param[in]       y            
    * @param[in]       width        
    * @param[in]       height       
    * @param[in]       textureId    
    * @param[in]       programId    
    */
    LAppSprite(float x, float y, float width, float height, GLuint textureId, GLuint programId);

    /**
    * @brief Destructor
    */
    ~LAppSprite();

    /**
    * @brief Return texture ID
    */
    GLuint GetTextureId() { return _textureId; }

    /**
    * @brief Perform render on the sprite
    *
    */
    void Render();

    /**
    * @brief Render the designated texture
    *
    */
    void RenderImmidiate(GLuint textureId, const GLfloat uvVertex[8]);


    /**
     * @brief (As the name suggests)
     *
     * @param[in]       r (0.0~1.0)
     * @param[in]       g (0.0~1.0)
     * @param[in]       b (0.0~1.0)
     * @param[in]       a (0.0~1.0)
     */
    void SetColor(float r, float g, float b, float a);

    /**
     * @brief Reset size
     *
     * @param[in]       x            
     * @param[in]       y            
     * @param[in]       width        
     * @param[in]       height       
     */
    void ResetRect(float x, float y, float width, float height);

private:
    GLuint _textureId;      ///  Texture ID
    Rect _rect;             ///  Rectangle
    int _positionLocation;  ///  Position attribute
    int _uvLocation;        ///  UV attribute
    int _textureLocation;   ///  Texture attribute
    int _colorLocation;     ///  Color attribute

    float _spriteColor[4];  ///  Display color
};

#endif