//função que permite dar print do texto
void PrintText(float x, float y, void* font, char* string, int red, int green, int blue)
{
    glColor3f(red, green, blue);
    glRasterPos2i(x, y);

    for (char* c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}