/*******************************************************************************
 * Copyright 2011 See AUTHORS file.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#pragma once
#include "../../GL.h"

class MipMapGenerator {
	static void generateMipMapGLES20 (int target, /*SDL2::Surface*/SDL_Surface* data,int format) {
		glTexImage2D(target, 0, format, data->w, data->h, 0,
			format, GL_UNSIGNED_BYTE, data->pixels);
		glGenerateMipmap(target);
	}

	static void generateMipMapDesktop (int target, /*SDL2::Surface*/SDL_Surface* data, int textureWidth, int textureHeight,int format) {
		//if (Gdx.graphics.supportsExtension("GL_ARB_framebuffer_object")
		//	|| Gdx.graphics.supportsExtension("GL_EXT_framebuffer_object") || Gdx.gl30 != null) {
			glTexImage2D(target, 0, format, data->w, data->h, 0,format, GL_UNSIGNED_BYTE, data->pixels);
			glGenerateMipmap(target);
		//} else generateMipMapCPU(target, data, textureWidth, textureHeight);
	}

	static void generateMipMapCPU (int target, /*SDL2::Surface*/SDL_Surface* data, int textureWidth, int textureHeight,int format) {
		glTexImage2D(target, 0, format, data->w, data->h, 0,format, GL_UNSIGNED_BYTE, data->pixels);
		if (textureWidth != textureHeight){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"texture width and height must be square when using mipmapping.");
			return;
        }
		int width = data->w / 2;
		int height = data->h / 2;
		int level = 1;
		while (width > 0 && height > 0) {
			glTexImage2D(target, level, format, data->w, data->h, 0,format, GL_UNSIGNED_BYTE, data->pixels);
			width = data->w / 2;
			height = data->h / 2;
			level++;
		}
	}
public:
    static void generateMipMap (/*SDL2::Surface*/SDL_Surface* data, int textureWidth, int textureHeight) {
		generateMipMap(GL_TEXTURE_2D, data, textureWidth, textureHeight,true);
	}

	/** Sets the image data of the {@link Texture} based on the {@link Pixmap}. The texture must be bound for this to work. If
	 * <code>disposePixmap</code> is true, the pixmap will be disposed at the end of the method.
	 * @param pixmap the Pixmap */
	static void generateMipMap (/*SDL2::Surface*/SDL_Surface* data, int textureWidth, int textureHeight,bool useHWMipMap) {
		generateMipMap(GL_TEXTURE_2D, data, textureWidth, textureHeight,useHWMipMap);
	}
    
    static void generateMipMap (int target, /*SDL2::Surface*/SDL_Surface* data, int textureWidth, int textureHeight){
        generateMipMap(target,data,textureWidth,textureHeight,true);
    }

	/** Sets the image data of the {@link Texture} based on the {@link Pixmap}. The texture must be bound for this to work. If
	 * <code>disposePixmap</code> is true, the pixmap will be disposed at the end of the method. */
	static void generateMipMap (int target, /*SDL2::Surface*/SDL_Surface* data, int textureWidth, int textureHeight,bool useHWMipMap) {
        int format = data->format->BytesPerPixel == 4? GL_RGBA : GL_RGB;
		if (!useHWMipMap) {
			generateMipMapCPU(target, data, textureWidth, textureHeight,format);
			return;
		}

        #ifdef DESKTOP
            generateMipMapDesktop(target, data, textureWidth, textureHeight,format);
        #else
            generateMipMapGLES20(target, data,format);
        #endif
	}
};