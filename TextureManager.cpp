#include <TextureManager.h>

std::string TextureManager::location = ".";
std::deque<TextureManager::Texture> TextureManager::textures;

GLuint TextureManager::get(std::string name, bool mipmaps)
{
	for(unsigned int i=0; i<textures.size(); i++)
		if(textures[i].name == name) return textures[i].tex;

	textures.push_back(Texture());
	textures.back().name = name;
	textures.back().tex = TGAImage::loadGLTexture(location+"/"+name, mipmaps);

	return textures.back().tex;
}

void TextureManager::clean()
{
	for(unsigned int i=0; i<textures.size(); i++)
		if(glIsTexture(textures[i].tex)) glDeleteTextures(1, &textures[i].tex);
}
