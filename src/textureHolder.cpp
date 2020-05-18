#include "textureHolder.h"
#include <stdexcept>

TextureHolder::TextureHolder(){
    setFileNames();
    mTextures.resize(Textures::size);
    for(int i=0; i<Textures::size; ++i){
        mTextures[i].reset(new sf::Texture);
        if(!mTextures[i]->loadFromFile(mFileNames[i]))
            throw std::runtime_error("Cannot open file " + mFileNames[i]);
    }
}

void TextureHolder::setFileNames(){
    mFileNames.resize(Textures::size);

    mFileNames[Textures::background] = "bg5.jpg";	
    mFileNames[Textures::player] = "playerShip3_blue_edited.png"; 
    // image from Commando 2 (weapon: C25 Marrugo)
    mFileNames[Textures::bullet] = "C25_Marrugo_missile_burst_transp.png";	
    mFileNames[Textures::explosion] = "exp2_0.png";
    mFileNames[Textures::meteorBig1] = "meteorBrown_big1.png";
    mFileNames[Textures::meteorBig2] = "meteorBrown_big2.png";
    mFileNames[Textures::meteorBig3] = "meteorBrown_big3.png";
    mFileNames[Textures::meteorBig4] = "meteorBrown_big4.png";
    mFileNames[Textures::meteorMedium1] = "meteorBrown_med1.png";
    mFileNames[Textures::meteorMedium2] = "meteorBrown_med2.png";
    mFileNames[Textures::meteorSmall1] = "meteorBrown_small1.png";
    mFileNames[Textures::meteorSmall2] = "meteorBrown_small2.png";
    mFileNames[Textures::meteorTiny1] = "meteorBrown_tiny1.png";
    mFileNames[Textures::meteorTiny2] = "meteorBrown_tiny2.png";
}
