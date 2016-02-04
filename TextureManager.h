#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <string>
#include <deque>
#include "TGAImage.h"

class TextureManager
{
	public:
		struct Texture { std::string name; GLuint tex; };

		static void setLocation(std::string loc) {location=loc;}
		static GLuint get(std::string name, bool mipmaps = true);
		static void clean();

	private:
		static std::string location;
		static std::deque<Texture> textures;

		TextureManager();
};

#endif /* TEXTUREMANAGER_H_ */
