/*
 tdogl::Texture
 
 Copyright 2012 Thomas Dalling - http://tomdalling.com/
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "Texture.h"
#include <stdexcept>

using namespace tdogl;

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format, bool srgb)
{
    switch (format) {
        case Bitmap::Format_Grayscale: return GL_LUMINANCE;
        case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
        case Bitmap::Format_RGB: return (srgb ? GL_SRGB : GL_RGB);
        case Bitmap::Format_RGBA: return (srgb ? GL_SRGB_ALPHA : GL_RGBA);
        default: throw std::runtime_error("Unrecognised Bitmap::Format");
    }
}

void Texture::AddTexture(const Bitmap& bitmap)
{
	if (_texcount == _maxtex)
		throw std::runtime_error("more textures than declared max");

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, _texcount,
		bitmap.width(), bitmap.height(), 1,
		TextureFormatForBitmapFormat(bitmap.format(), false),
		GL_UNSIGNED_BYTE, bitmap.pixelBuffer());
	_texcount++;
}

Texture::Texture(unsigned int maxwidth, unsigned int maxheight, unsigned int texcount, GLint minMagFiler, GLint wrapMode) :
    _originalWidth((GLfloat)maxwidth),
    _originalHeight((GLfloat)maxheight),
    _texcount(0),
    _maxtex(texcount)
{
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _object);
	//glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapMode);
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB, maxwidth, maxheight, texcount);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_SRGB_ALPHA, maxwidth,
            maxheight, texcount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_object);
}

GLuint Texture::object() const
{
    return _object;
}

GLfloat Texture::originalWidth() const
{
    return _originalWidth;
}

GLfloat Texture::originalHeight() const
{
    return _originalHeight;
}
