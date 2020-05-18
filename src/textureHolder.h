#pragma once
#include <SFML/Graphics.hpp>
#include <vector> 
#include <memory>
#include <string>

// Ne treba mijenjati. 
namespace Textures{
    enum ID{  background, player, bullet, explosion,
              meteorBig1, meteorBig2, meteorBig3, meteorBig4,
              meteorMedium1, meteorMedium2,
              meteorSmall1, meteorSmall2,
			  meteorTiny1, meteorTiny2,
              size
    };
}

class TextureHolder{
public:
    TextureHolder();
    sf::Texture & getTexture(Textures::ID i) { return *mTextures[i]; }
private:
    std::vector<std::unique_ptr<sf::Texture>>  mTextures; 
    std::vector<std::string> mFileNames;
    void setFileNames();
};
