
#include "wndmap.h"
#include <QHBoxLayout>

namespace ts
{
GlMap::GlMap(QList<Room> rooms, QWidget *parent)
    : QGLWidget(parent)
{
    m_rooms = rooms;
    object = 0;
    xrot = 3;
    yrot = 0;
    zrot = 0;
}

GlMap::~GlMap()
{
}

void GlMap::initializeGL()
{
    texture[0] = bindTexture(QPixmap("bookmark.png"), GL_TEXTURE_2D);
    object = makeobject();
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


}

void GlMap::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
    glLoadIdentity();                       // Reset The View
    glCallList(object);


}

void GlMap::resizeGL(int w, int h)
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
    glMatrixMode( GL_MODELVIEW );
}

GLuint GlMap::makeobject()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    glTranslatef(  0.0,  0.0, -15.0 );
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    for(int i = 0; i < m_rooms.size(); i++) {
        struct glCoords gC;
        if(!i) {
            glBegin(GL_QUADS);
            glTexCoord2f( 0.0, 0.0 ); glVertex3f( -0.2, -0.2,  0.2 );
            glTexCoord2f( 1.0, 0.0 ); glVertex3f(  0.2, -0.2,  0.2 );
            glTexCoord2f( 1.0, 1.0 ); glVertex3f(  0.2,  0.2,  0.2 );
            glTexCoord2f( 0.0, 1.0 ); glVertex3f( -0.2,  0.2,  0.2 );
            glEnd();
            gC.aX = -0.2;
            gC.aY = 0.2;
            gC.bX = 0.2;
            gC.bY = -0.2;
            objMap[m_rooms.at(i).vnumber()] = gC;
        } else if(objMap.contains(m_rooms.at(i).vnumber())) {

        } else {

        }

//         for(int k = 0; k < m_rooms.at(i).exits() ; k++) {
//
//         }
//         glBindTexture(GL_TEXTURE_2D, texture[0]);
//         glBegin(GL_QUADS);
//         glTexCoord2f( 0.0, 0.0 ); glVertex3f( -0.2, -0.2,  0.2 );
//         glTexCoord2f( 1.0, 0.0 ); glVertex3f(  0.2, -0.2,  0.2 );
//         glTexCoord2f( 1.0, 1.0 ); glVertex3f(  0.2,  0.2,  0.2 );
//         glTexCoord2f( 0.0, 1.0 ); glVertex3f( -0.2,  0.2,  0.2 );
//         glEnd();
    }

//     qglColor(Qt::white);
//     glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
//
//     glBegin(GL_QUADS);
//         // Front Face
//     glTexCoord2f( 0.0, 0.0 ); glVertex3f( -0.2, -0.2,  0.2 );
//     glTexCoord2f( 1.0, 0.0 ); glVertex3f(  0.2, -0.2,  0.2 );
//     glTexCoord2f( 1.0, 1.0 ); glVertex3f(  0.2,  0.2,  0.2 );
//     glTexCoord2f( 0.0, 1.0 ); glVertex3f( -0.2,  0.2,  0.2 );
//
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, texture[1]);               // Select Our Texture
//
//     glBegin(GL_LINES);
//     glVertex3f(0.2f, 0.0f, 0.2f); // origin of the line
//     glVertex3f(0.4f, 0.0f, 0.2f); // ending point of the line
//
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
//
//     glBegin(GL_QUADS);
//         // Front Face
//     glTexCoord2f( 0.0, 0.0 ); glVertex3f(  0.4,  -0.2,  0.2 );
//     glTexCoord2f( 1.0, 0.0 ); glVertex3f(  0.8,  -0.2,  0.2 );
//     glTexCoord2f( 1.0, 1.0 ); glVertex3f(  0.8,  0.2,  0.2 );
//     glTexCoord2f( 0.0, 1.0 ); glVertex3f(  0.4,  0.2,  0.2 );
//
//     glEnd();

    glEndList();
    return list;
}

WndMap::WndMap(Area ar)
{
    map = new GlMap(ar.rooms(), this);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(map);
    setLayout(lay);

    resize(800, 600);
}

}
