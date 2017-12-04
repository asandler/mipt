#define _USE_MATH_DEFINES
#include <math.h>

#include <glimg/glimg.h>

#include "texture.h"

void getColorFromLinearPalette(float value, float& r, float& g, float& b);

GLuint Texture::loadTexture(std::string filename) {
    GLuint texId;

    try {
        boost::shared_ptr<glimg::ImageSet> pImageSet;

        if (filename.find(".dds") != std::string::npos) {
            pImageSet.reset(glimg::loaders::dds::LoadFromFile(filename));
        } else {
            pImageSet.reset(glimg::loaders::stb::LoadFromFile(filename));
        }

        glimg::SingleImage pImage = pImageSet->GetImage(0, 0, 0);
        glimg::Dimensions dims = pImage.GetDimensions();

        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        if (filename.find(".png") != std::string::npos) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, dims.width, dims.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage.GetImageData());
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, dims.width, dims.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage.GetImageData());
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    } catch (glimg::loaders::stb::StbLoaderException&) {
        std::cerr << "Failed to load texture " << filename << std::endl;
        exit(1);
    } catch (glimg::loaders::dds::DdsLoaderException&) {
        std::cerr << "Failed to load texture " << filename << std::endl;
        exit(1);
    }

    return texId;
}
