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

#include "../GL.h"
#include "glutils/MipMapGenerator.h"
//import com.badlogic.gdx.files.FileHandle;

/** A Texture wraps a standard OpenGL ES texture.
 * <p>
 * A Texture can be managed. If the OpenGL context is lost all managed textures get invalidated. This happens when a user switches
 * to another application or receives an incoming call. Managed textures get reloaded automatically.
 * <p>
 * A Texture has to be bound via the {@link Texture#bind()} method in order for it to be applied to geometry. The texture will be
 * bound to the currently active texture unit specified via {@link GL20#glActiveTexture(int)}.
 * <p>
 * You can draw {@link Pixmap}s to a texture at any time. The changes will be automatically uploaded to texture memory. This is of
 * course not extremely fast so use it with care. It also only works with unmanaged textures.
 * <p>
 * A Texture must be disposed when it is no longer used
 * @author badlogicgames@gmail.com */
class Texture{
	static void uploadImageData (int target, /*SDL2::Surface*/SDL_Surface* data,bool useMipMaps) {
		uploadImageData(target, data, 0,useMipMaps);
	}
	
	static void uploadImageData (int target, /*SDL2::Surface*/SDL_Surface* data, int miplevel,bool useMipMaps) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (useMipMaps)
			MipMapGenerator::generateMipMap(target, data, data->w, data->h);
		else {
            int format = data->format->BytesPerPixel == 4? GL_RGBA : GL_RGB;
			glTexImage2D(target, miplevel, format, data->w, data->h, 0,
				format, GL_UNSIGNED_BYTE, data->pixels);
		}
	}
    
	/** Used internally to reload after context loss. Creates a new GL handle then calls {@link #load(TextureData)}. Use this only
	 * if you know what you do! */
	void reload () {
        destroy();
		glGenTextures(1, &glHandle);
		load(data);
	}
    
	/** The target of this texture, used when binding the texture, e.g. GL_TEXTURE_2D */
	GLenum glTarget;
	GLuint glHandle;
	GLenum minFilter = GL_NEAREST,magFilter = GL_NEAREST;
	GLenum uWrap = GL_CLAMP_TO_EDGE,vWrap = GL_CLAMP_TO_EDGE;
    bool useMipMaps;
public:
	/*SDL2::Surface*/SDL_Surface* data;
    
    Texture(){}
    
    Texture(std::string internalPath,GLenum minFilter,GLenum magFilter,GLenum uWrap,GLenum vWrap,bool useMipMaps){
        this->minFilter = minFilter;
        this->magFilter = magFilter;
        this->uWrap = uWrap;
        this->vWrap = vWrap;
        this->useMipMaps = useMipMaps;
        data = /*SDL2::Surface(*/IMG_Load(internalPath.c_str())/*)*/;
        if(!data)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"IMG_Load: %s\n",IMG_GetError());
        else load(data);
    }
    
    Texture (int glTarget,/*SDL2::Surface*/SDL_Surface* data){
        this->glTarget = glTarget;
        glGenTextures(1, &glHandle);
		load(data);
	}

	Texture (/*SDL2::Surface*/SDL_Surface* data):Texture(GL_TEXTURE_2D,data) {}
    
    /** Disposes all resources associated with the texture */
	~Texture() {destroy();SDL_Log("TEXTURE DESTROY!");}

	void load (/*SDL2::Surface*/SDL_Surface* data) {
		this->data = data;
        
        //Create image, set its properties.
        bind();
		uploadImageData(GL_TEXTURE_2D, data,useMipMaps);
		setFilter(minFilter, magFilter);
		setWrap(uWrap, vWrap);
        
        //Reset texture pipeline to null.
		unbind(glTarget);
	}
    
    void destroy(){
        if (glHandle != 0) {
			glDeleteTextures(1,&glHandle);
			glHandle = 0;
		}        
    }

	/** Draws the given {@link Pixmap} to the texture at position x, y. No clipping is performed so you have to make sure that you
	 * draw only inside the texture region. Note that this will only draw to mipmap level 0!
	 * 
	 * @param pixmap The Pixmap
	 * @param x The x coordinate in pixels
	 * @param y The y coordinate in pixels */
	void draw (/*SDL2::Surface*/SDL_Surface* data, int x, int y) {
        int format = data->format->BytesPerPixel == 4? GL_RGBA : GL_RGB;
		bind();
		glTexSubImage2D(glTarget, 0, x, y, data->w, data->h, format, GL_UNSIGNED_BYTE,data->pixels);
	}
    
    GLuint getTextureObjectHandle () {return glHandle;}

    //SDL_Surface data.
    /*SDL2::Surface*/SDL_Surface* getTextureData () {return data;}
	int getWidth () {return data->w;}
	int getHeight () {return data->h;}
	int getDepth () {return data->pitch;}
    
    //Filters and wraps.
    GLint getMinFilter () {return minFilter;}
	GLint getMagFilter () {return magFilter;}
    GLenum getUWrap () {return uWrap;}
    GLenum getVWrap () {return vWrap;}
    
    static void unbind(GLenum target){glBindTexture(target, 0);}
    
	/** Binds this texture. The texture will be bound to the currently active texture unit specified via
	 * {@link GL20#glActiveTexture(int)}. */
	void bind () {glBindTexture(glTarget, glHandle);}

	/** Binds the texture to the given texture unit. Sets the currently active texture unit via {@link GL20#glActiveTexture(int)}.
	 * @param unit the unit (0 to MAX_TEXTURE_UNITS). */
	void bind (int unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(glTarget, glHandle);
	}

	/** Sets the {@link TextureWrap} for this texture on the u and v axis. This will bind this texture!
	 * @param u the u wrap
	 * @param v the v wrap */
	void setWrap (GLenum u, GLenum v) {
		this->uWrap = u;
		this->vWrap = v;
		bind();
		glTexParameterf(glTarget, GL_TEXTURE_WRAP_S, u);
		glTexParameterf(glTarget, GL_TEXTURE_WRAP_T, v);
	}

	/** Sets the {@link GLenum} for this texture for minification and magnification. This will bind this texture!
	 * @param minFilter the minification filter
	 * @param magFilter the magnification filter */
	void setFilter (GLenum minFilter, GLenum magFilter) {
		this->minFilter = minFilter;
		this->magFilter = magFilter;
		bind();
		glTexParameterf(glTarget, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameterf(glTarget, GL_TEXTURE_MAG_FILTER, magFilter);
	}
};