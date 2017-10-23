
//STL
#include <string>
#include <cmath>
#include <algorithm>

//Qt
#include <QImage>
#include <QString>
#include <QtOpenGL>
#include <QtWidgets>

//cs40
#include "drawable.h"


typedef enum CS40_APP_MODE{
    NONE=0,
} app_mode_t;


class Painter : public QOpenGLWidget { 
  Q_OBJECT

  protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h); 

  public: 
    Painter(QWidget *parent = 0);
    ~Painter();

  public slots:
    void open();
    void paint();
    void setThreshold(qreal value);

  private:  

    qreal m_blurFactor; //changes how precise image is blurred
    QLabel *m_imageLabel; //box for image
    QImage m_img; //image being processed
    QOpenGLTexture* m_texture; //texture version of image
    QList<cs40::Drawable*> m_strokes;
    qreal m_threshold;

    QScrollArea *m_scrollArea;  /* scrollbars if image is too small */
    QGridLayout *m_layout;      /* declare layout of widget */

    //shaders and program
    QOpenGLShader *m_vertexShader;
    QOpenGLShader *m_fragmentShader;
    QOpenGLShaderProgram *m_shaderProgram;

    //buffers
    QVector3D *vertices;
    QOpenGLBuffer *vboVertices;
    QOpenGLVertexArrayObject *vao;
    
    void createShaders();
    void destroyShaders();
    void createVBOs();
    void setupVAO();
    QImage createReferenceImage(qreal brushSize); //get blurred image
    QList<cs40::Drawable*> getLayer(qreal brushSize, QImage canvas, qreal threshold); //compute strokes for layer
    QVector3D toVector(QRgb color);
    QImage paintImage(int layers, int stopSize=1); //main paint function, specify # layers or stopping brush size



};
