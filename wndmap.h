#ifndef WNDMAP_H
#define WNDMAP_H

#include <QMap>
#include <QGLWidget>

#include "room.h"
#include "area.h"

namespace ts
{
    struct glCoords {
        float aX,aY,bX,bY;
    };

    class GlMap : public QGLWidget
    {
        Q_OBJECT

        public:
            explicit GlMap(QList<Room> rooms, QWidget *parent = 0);
            ~GlMap();
        protected:
            void paintGL();
            void initializeGL();
            void resizeGL(int w, int h);
            GLuint makeobject();
        private:
            GLfloat      xrot;                               // X Rotation ( NEW )
            GLfloat     yrot;                               // Y Rotation ( NEW )
            GLfloat     zrot;                               // Z Rotation ( NEW )
            GLfloat xspeed;                                 // X Rotation Speed
            GLfloat yspeed;                                 // Y Rotation Speed
            GLuint      texture[3];                         // Storage For One Texture ( NEW )
            GLuint filter;
            GLuint object;
            QMap<VNumber, glCoords> objMap;
            QList<Room> m_rooms;

    };

    class WndMap : public QWidget
    {
        Q_OBJECT

        public:
            WndMap(Area ar);

        private:
            GlMap *map;

    };
}
#endif
