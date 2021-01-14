#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Common.h"
#include "Block.h"
#include "Game.h"
#include "RgbImage.h"
#include "includes/texture.cpp"
#include "includes/light.cpp"
#include "includes/text.cpp"

#define SCREEN_SIZE     1000, 500
#define SCREEN_POSITION 800,  400
#define SCREEN_COLOR     0.0, 0.0, 0.0, 0.0
#define MAX_ZOOM 7.0f
#define MIN_ZOOM 10.0f

bool fullscreen = false;
bool stopped = false;
bool soundPaused = true;

void initFunc();
void funReshape(int w, int h);
void funDisplay();
void funIdle();
void funKeyboardUp(unsigned char key, int x, int y);
void funSpecial(int key, int x, int y);
void funMotion(int x, int y);
void funMouseWheel(int wheel, int direction, int x, int y);
void drawFrame();
void drawPanel();
void drawBlocks();
void drawPlane(GLfloat size);
void drawBlock(Block* block);
void drawSubBlock(SubBlock* sub);
void drawBasicBlock(bool withBorder = true);
void initLights();
void initTextures();
void selectColor(uint8 color);
void generateRandomBlock();
void PrintText(float x, float y, void* font, char* string, int red, int green, int blue);
void Pontos();

GLfloat cameraPos[3] = { 2.0, 3.0, 10.0 };
GLfloat lookat[3] = { 2.0, 3.0, -8.0 };
GLfloat up[3] = { 0.0, 1.0,  0.0 };

GLfloat currentRotationZ = 0.0;


uint8 color = COLOR_WHITE;

int32 oldX = 0, oldY = 0;

uint32 lastClickTime = 0;

uint64 nextMoveTime = 0;

Game* game = nullptr;



int main(int argc, char** argv) {

    srand(unsigned int(time(nullptr)));

    // Inicializamos OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(SCREEN_SIZE);
    glutInitWindowPosition(SCREEN_POSITION);
    glutCreateWindow("Tetris 3D");

  
    initFunc();

    // Configurar CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutKeyboardUpFunc(funKeyboardUp);
    glutSpecialFunc(funSpecial);
    glutMotionFunc(funMotion);
    glutIdleFunc(funIdle);
    glutMouseWheelFunc(funMouseWheel);

    //criar um jogo
    game = Game::CreateNewGame();
    if (!game)
        return(EXIT_FAILURE);
    //começa a musica
    PlaySoundTetris(TEXT("../src/main.wav"), nullptr, SND_LOOP | SND_ASYNC);
    //momento do inicio do jogo
    game->StartGame();


    // Bucle principal
    glutMainLoop();

    // Responsável por destruir os objetos
    return EXIT_SUCCESS;
}

//inicializa todo o processo para o jogo se inicializar
void initFunc() {

    // Iniar o GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        DEBUG_LOG("Error: %s\n", glewGetErrorString(err));
    }
    DEBUG_LOG("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Configuração de Parametros Fixos
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_CULL_FACE);
    initLights();
    initTextures();
    glPolygonOffset(1.0, 1.0);
    glShadeModel(GL_SMOOTH);
}

//função usada para redifinir forma dos blocos
void funReshape(int w, int h) {

    // Configurar a View Port
    glViewport(0, 0, w, h);

    //Modelo de Projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 50.0);
}

//função para ler as teclas e reproduzir a respetiva função despultar por cada click
void funKeyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'm':
        soundPaused = !soundPaused;
        if (!soundPaused)
            PlaySoundTetris(nullptr, nullptr, 0);
        else
            PlaySoundTetris(TEXT("../src/main.wav"), nullptr, SND_LOOP | SND_ASYNC);
        break;
    case 'r':
        cameraPos[0] = 2.0f;
        cameraPos[1] = 3.0f;
        cameraPos[2] = 10.0f;
        lookat[0] = 2.0f;
        lookat[1] = 3.0f;
        lookat[2] = -8.0f;
        break;
    case 'c':
        game->ChangeBlock();
        break;
    case ' ':
        game->RotateActiveBlock();
        break;
    case 27: // ESC
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    DEBUG_LOG("KEYBOARD: key: %c, x: %d, y: %d \n", key, x, y);
}

// função que permite mover as peças pelas teclas
void funSpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:       			// F1 toggles fullscreen mode
        fullscreen = !fullscreen;
        if (fullscreen) glutFullScreen();
        else glutReshapeWindow(500, 500);
        break;
    case GLUT_KEY_UP:
        game->DropBlock();
        break;
    case GLUT_KEY_DOWN:
        game->IncreaseBlockSpeed();
        break;
    case GLUT_KEY_RIGHT:
        game->MoveBlock(true);
        break;
    case GLUT_KEY_LEFT:
        game->MoveBlock(false);
        break;
    default:
        break;
    }
    DEBUG_LOG("KEYBOARD SPECIAL: key: %d, x: %d, y: %d \n", key, x, y);
}

//função que vira o ecrã do jogo com o rato
void funMotion(int x, int y)
{
    if (!stopped)
    {
        cameraPos[0] -= float(oldX - x) / 300.0f;
        cameraPos[1] += float(oldY - y) / 300.0f;

        oldX = x;
        oldY = y;
    }

    DEBUG_LOG("MOTION: x: %d, y: %d \n", x, y);
}

//função que permite fazer zoom com o rato
void funMouseWheel(int wheel, int direction, int x, int y)
{
    cameraPos[2] = std::min<GLfloat>(MIN_ZOOM, std::max<GLfloat>(MAX_ZOOM, cameraPos[2] - direction * 0.3f));
    DEBUG_LOG("MOUSEWHEEL: wheel: %d, direction: %d, x: %d, y: %d, positionZ: %f \n", wheel, direction, x, y, cameraPos[2]);
}

//função que mostra que desenha a janela do jogo
void funDisplay()
{
    drawFrame();
}

void funIdle()
{
    if (!stopped)
        game->Update();

    drawFrame();
}

//função responsável por definir a janela do jogo
void drawFrame()
{
  
    glClearColor(SCREEN_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Posicionar a camara
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Posicão camara
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
        lookat[0], lookat[1], lookat[2],
        up[0], up[1], up[2]);

    glScaled(0.5f, 0.5f, 0.5f);
    drawPanel();
    drawBlocks();
    glScaled(1.0f, 1.0f, 1.0f);
    Pontos();
    char text[1024] = "PROJETO TETRIS 3D - COMPUTACAO GRÁFICA";
    PrintText(POINTS_X - 5, POINTS_Y + 1, GLUT_BITMAP_HELVETICA_12, text, 1.0f, 1.0f, 1.0f);
    glutSwapBuffers();
}


void drawBlocks()
{
    // Draw the active falling block
    if (game->GetActiveBlock())
        drawBlock(game->GetActiveBlock());

    // Draw other subBlocks
    for (SubBlock* sub : game->GetSubBlockList())
        drawSubBlock(sub);
}

//função que desenha os blocos
void drawBlock(Block* block)
{
    if (!block)
        return;

    if (block->GetType() >= MAX_BLOCK_TYPE)
    {
        DEBUG_LOG("Block type not supported: type (%d)", block->GetType());
        exit(1);
    }

    color = block->GetColor();
    Block::SubBlockVector subBlocks = block->GetSubBlocks();

    float correction[2] = { 0.0f, 0.0f };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPushMatrix();
    {
        glTranslatef(block->GetPositionX() + correction[0], block->GetPositionY() + correction[1], block->GetPositionZ());

        if (correction[0] != 0.0f || correction[1] != 0.0f)
            glTranslatef(-correction[0], -correction[1], 0.0f);

        for (SubBlock* sub : subBlocks)
        {
            glPushMatrix();
            glTranslatef(sub->GetPositionX(), sub->GetPositionY(), sub->GetPositionZ());
            drawBasicBlock();
            glPopMatrix();
        }
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawSubBlock(SubBlock* sub)
{
    color = sub->GetColor();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPushMatrix();
    glTranslatef(sub->GetPositionX(), sub->GetPositionY(), sub->GetPositionZ());
    drawBasicBlock();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

//função para definir a cor do bloco da peça
void selectColor(uint8 color)
{
    switch (color)
    {
    case COLOR_WHITE:
    {
        GLfloat Kad[] = { 1.0, 1.0, 1.0, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(1.0, 1.0, 1.0);
        break;
    }
    case COLOR_BLACK:
    {
        GLfloat Kad[] = { 0.0, 0.0, 0.0, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(0.0, 0.0, 0.0);
        break;
    }
    case COLOR_RED:
    {
        GLfloat Kad[] = { 0.863f, 0.078f, 0.235f, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(0.863f, 0.078f, 0.235f);
        break;
    }
    case COLOR_BLUE:
    {
        GLfloat Kad[] = { 0.118f, 0.565f, 1.000f, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(0.118f, 0.565f, 1.000f);
        break;
    }
    case COLOR_GREEN:
    {
        GLfloat Kad[] = { 0.235f, 0.702f, 0.443f, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(0.235f, 0.702f, 0.443f);
        break;
    }
    case COLOR_YELLOW:
    {
        GLfloat Kad[] = { 1.0, 1.0, 0.0, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(1.0, 1.0, 0.0);
        break;
    }
    case COLOR_PINK:
    {
        GLfloat Kad[] = { 1.0, 0.0, 0.5, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(1.0, 0.0, 0.5);
        break;
    }
    case COLOR_ORANGE:
    {
        GLfloat Kad[] = { 1.0, 0.5, 0.0, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(1.0, 0.5, 0.0);
        break;
    }
    case COLOR_GRAY:
    {
        GLfloat Kad[] = { 0.653f, 0.653f, 0.653f, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(0.753f, 0.753f, 0.753f);
        break;
    }
    case COLOR_CYAN:
    {
        GLfloat Kad[] = { 0.902f, 0.902f, 0.980f, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
        glColor3f(0.902f, 0.902f, 0.980f);
        break;
    }
    default:
        break;
    }
}

//função responsável por desenhar o cubo que vai definir os blocos 
void drawCube(GLfloat size)
{
    GLfloat dimension = size / 2.0f;

    // DESENHAR UM CUBO COM 6 QUADRILÁTEROS
    glBegin(GL_QUADS);				// COMEÇO DO DESENHO DO CUBO

        // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-dimension, -dimension, dimension);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(dimension, -dimension, dimension);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(dimension, dimension, dimension);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-dimension, dimension, dimension);	// Top Left Of The Texture and Quad

    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-dimension, -dimension, -dimension);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-dimension, dimension, -dimension);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(dimension, dimension, -dimension);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(dimension, -dimension, -dimension);	// Bottom Left Of The Texture and Quad

    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-dimension, dimension, -dimension);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-dimension, dimension, dimension);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(dimension, dimension, dimension);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(dimension, dimension, -dimension);	// Top Right Of The Texture and Quad

    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-dimension, -dimension, -dimension);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(dimension, -dimension, -dimension);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(dimension, -dimension, dimension);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-dimension, -dimension, dimension);	// Bottom Right Of The Texture and Quad

    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(dimension, -dimension, -dimension);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(dimension, dimension, -dimension);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(dimension, dimension, dimension);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(dimension, -dimension, dimension);	// Bottom Left Of The Texture and Quad

    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-dimension, -dimension, -dimension);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-dimension, -dimension, dimension);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-dimension, dimension, dimension);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-dimension, dimension, -dimension);	// Top Left Of The Texture and Quad

    glEnd();			

}

void drawBasicBlock(bool withBorder /*=true*/)
{
    selectColor(color);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    drawCube(1.0f);

    glDisable(GL_TEXTURE_2D);
}

//função que desenha o panel de jogo
void drawPanel()
{
    uint8 oldColor = color;
    color = COLOR_GRAY;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //parte de baixo do u
    glPushMatrix();
    {
        glTranslatef(0.0, -1.0, 0.0);
        for (uint8 i = 0; i < MAX_WIDTH; i++)
        {
            drawBasicBlock();
            glTranslatef(1.0, 0.0, 0.0);
        }
    }
    glPopMatrix();

    // parte esquerda do u
    glPushMatrix();
    {
        glTranslatef(-1.0, -1.0, 0.0);
        for (uint8 i = 0; i < MAX_HEIGHT; i++)
        {
            drawBasicBlock();
            glTranslatef(0.0, 1.0, 0.0);
        }
    }
    glPopMatrix();
    //parte direita do u
    glPushMatrix();
    {
        glTranslatef(MAX_WIDTH, -1.0, 0.0);
        for (uint8 i = 0; i < MAX_HEIGHT; i++)
        {
            drawBasicBlock();
            glTranslatef(0.0, 1.0, 0.0);
        }
    }
    glPopMatrix();
    color = oldColor;
    glDisable(GL_TEXTURE_2D);

}

//função que mostra pontos
void Pontos() {

    char text[1024] = "";
    sprintf_s(text, "%u pontos", game->GetPoints(), std::to_string(game->GetPoints()));
    PrintText(POINTS_X, POINTS_Y - 1, GLUT_BITMAP_HELVETICA_12, text, 1.0f, 1.0f, 1.0f);

}




