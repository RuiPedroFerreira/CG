

GLfloat ambientLightIntensity[] = { 0.2f, 0.2f, 0.2f, 0.2f };


//função responsável pela iluminação do jogo
void initLights()
{
    glEnable(GL_LIGHTING);

    // Luz ambiente global
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightIntensity);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    {
        GLfloat Ia0[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat Id0[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat Is0[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat point[] = { 4.0, 4.0, 5.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, Ia0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Id0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, Is0);
        glLightfv(GL_LIGHT0, GL_POSITION, point);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.50f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
        glEnable(GL_LIGHT0);
    }

    {
        GLfloat Ia0[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat Id0[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat Is0[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat point[] = { -4.0, 4.0, 5.0, 1.0 };
        glLightfv(GL_LIGHT1, GL_AMBIENT, Ia0);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, Id0);
        glLightfv(GL_LIGHT1, GL_SPECULAR, Is0);
        glLightfv(GL_LIGHT1, GL_POSITION, point);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.50f);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01f);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);
        glEnable(GL_LIGHT1);
    }
}