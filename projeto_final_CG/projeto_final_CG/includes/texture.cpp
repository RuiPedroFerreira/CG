const GLuint numTextures = 1;
GLuint textureName[numTextures];

//função que carrega as texturas para depois serem aplicadas no jogo
void initTextures()
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(numTextures, textureName);

    const char* filename[numTextures] = { "../src/brick1.bmp" };

    // Carregar as texturas
    glBindTexture(GL_TEXTURE_2D, textureName[0]);
    RgbImage texture(filename[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.GetNumCols(), texture.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.ImageData());
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.GetNumCols(), texture.GetNumRows(), GL_RGB, GL_UNSIGNED_BYTE, texture.ImageData());

    // Configurar as texturas
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}