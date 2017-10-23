#include "drawable.h"
#include <iostream>

using namespace cs40;

Drawable::Drawable() : m_prog(NULL), m_vao(NULL), m_vbo(NULL),
    m_color(0,0,0), m_displacement(0,0), m_visible(true) , m_center(0,0) {
}

Drawable::~Drawable(){
    /* cleanup dynamically allocated base class objects */
    if(m_vbo){
        m_vbo->release();
        delete m_vbo; m_vbo=NULL;
    }

    if(m_vao){
        m_vao->release();
        delete m_vao; m_vao=NULL;
    }
}

bool Drawable::initVBO(){
    bool ok=true; /* trust but verify */

    if( ! m_vao ){
        /* create VAO first */
        m_vao = new QOpenGLVertexArrayObject();
        ok = m_vao->create();
        if (!ok) { return false; }
        m_vao->bind();
    }

    if ( ! m_vbo ){
        /* make new VBO ID, but don't allocate
         * we don't know how much space to save anyways
         * in the base class
         */
        m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        ok = m_vbo->create();
        if (!ok) { return false; }
        m_vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    }

    return ok;
}


void Drawable::move(float dx, float dy){
    /* apply new shift to current displacement from
     * original location */
    vec2 shift(dx,dy);
    m_displacement += shift;
}

void Drawable::drawHelper(GLenum mode, int count){
    m_vao->bind();
    m_vbo->bind();
    m_prog->bind();
    
    /* VAO should be remembering this. It is not */
    m_prog->enableAttributeArray("vPosition");
    m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);
    
    m_prog->setUniformValue("color", getColor());
    m_prog->setUniformValue("displacement", m_displacement);
    glDrawArrays(mode, 0 , count);
    m_prog->release();
    m_vbo->release();
    m_vao->release();
}

void Drawable::setCenter(int x, int y){
  m_center= vec2(x, y);
}

vec2 Drawable::getCenter(){
  return m_center;
}
