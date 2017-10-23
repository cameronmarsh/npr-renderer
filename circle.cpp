#include "circle.h"
#include <math.h>

using namespace cs40;

Circle::Circle(QOpenGLShaderProgram* const prog, const vec2& center, 
                                                float radius){
  m_prog = prog;
  m_radius = radius;
  m_pts[0] = center;
  float rad, x, y;
  for(int i=1; i<361; i++){
      rad = i*float(180)/M_PI;
      x = cos(rad)*radius + center.x();
      y = sin(rad)*radius + center.y();
      m_pts[i] = QVector2D(x,y);
  }

  if(initVBO()){
    m_vao->bind();
    m_prog->bind();
    m_vbo->bind();
    m_vbo->allocate(m_pts, 361*sizeof(QVector2D));
    m_prog->enableAttributeArray("vPosition");
    m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);
    m_vao->release();
  }

  m_visible = true;

}

Circle::Circle(const Circle* const other){
    m_prog = other->m_prog;
    m_radius = other->m_radius;
    memcpy(m_pts, other->m_pts, 361*sizeof(vec2));
    for(int i = 0; i<361; i++){
        m_pts[i] = other->m_pts[i];
    }

    m_displacement = other->m_displacement;
    m_color = other->m_color;

    if(initVBO()){
      m_vao->bind();
      m_prog->bind();
      m_vbo->bind();
      m_vbo->allocate(m_pts, 361*sizeof(QVector2D));
      m_prog->enableAttributeArray("vPosition");
      m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);
      m_vao->release();
    }

    m_visible = true;

}

vec2 Circle::getCenter(){
  return m_pts[0];
}

float Circle::getRadius(){
  return m_radius;
}

void Circle::draw(){
  if(m_visible){  
    drawHelper(GL_TRIANGLE_FAN, 361);
  }
}

bool Circle::contains(const vec2 &pt) const{
  if(sqrt(pow((pt.x()-(m_pts[0]+m_displacement).x()),2)+pow((pt.y()-(m_pts[0]+m_displacement).y()),2)) > m_radius){return false;} 
  
  return true;
}
