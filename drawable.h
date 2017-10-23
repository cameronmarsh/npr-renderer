#pragma once /* replaces #ifndef blocks */

#include <QtOpenGL>

/* prevent name collisions by putting our code in
 * a cs40 namespace */
namespace cs40{

/* make glsl-esque shortcuts
 * but these types are really QVector types */
typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

/* not much going on here. Functionality in derived classes */
class Drawable {
public:

    //default, empty constructor
    Drawable();


    /* classes can have only a single destructor "virtual" with a destructor
     * means that derived classes can change the functionality of the base
     * class destructor by adding their own destructor to clean-up their part
     * of the class. Without virtual only the base class destructor is invoked.
     * Overriding a destructor has different semantics than overriding
     * a regular function (all destructors are invoked). Destructors are called
     * in this order:
     *   1. derived class destructor 2. base class destructor
     */
    virtual ~Drawable();

    /* Pure virtual function. Must be implemented by derived class
     * Draws object in current openGL context*/
    virtual void draw()=0;

    /* Generate Vertex Buffer Object, but do not bind/allocate.
     * Return true if generate was a success, false otherwise
     */
    bool initVBO();

    /* Set color of object */
    /* Since this method is the same for all derived classes
         * we can implement it once in the base class and we
         * don't need to keep re-implementing it */
    inline void setColor(const vec3& color){
        m_color=color;
    }

    /* Move the object by an amount dx, dy
     * Provide a default implemtation, but allow over-riding
     */
    virtual void move(float dx, float dy);

    /* return true if shape contains point pt, false otherwise
     * Pure virtual. Must be implemented in derived class
     */
    virtual bool contains(const vec2& pt) const = 0;

    /* Get current color of object */
    inline vec3 getColor() const { return m_color; }

    /* Hide shape so that it is not drawn */
    inline void hide(){ m_visible=false; }

    /* return true if visibile, false otherwise */
    inline bool isVisible() const { return m_visible; }

    void drawHelper(GLenum mode, int count);

    void setCenter(int x, int y);
    vec2 getCenter();

protected:

    /* a reference to the current shader program  */
    QOpenGLShaderProgram* m_prog;

    /* a standard vao for rapid drawing */
    QOpenGLVertexArrayObject *m_vao;

    /* geometry */
    QOpenGLBuffer *m_vbo;

    /* color of object */
    vec3 m_color;

    /* displacement from original location */
    vec2 m_displacement;

    /* is current object visible? */
    bool m_visible;

    vec2 m_center;

};

} //namespace

