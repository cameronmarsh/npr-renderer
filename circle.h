#pragma once

#include <drawable.h>
namespace cs40{

class Circle: public Drawable {

  public:

        Circle(QOpenGLShaderProgram* const prog, const vec2& center, 
            float radius);
        
        Circle(const Circle* const other);
        
        ~Circle() {/*do nothing*/};

        void draw();
        bool contains(const vec2 &pt) const;
        vec2 getCenter();
        float getRadius();


  private:

        vec2 m_pts[361];
        float m_radius;
        Circle& operator=(const Circle&);

};

}
