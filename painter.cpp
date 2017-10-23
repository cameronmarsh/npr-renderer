#include "painter.h"
#include "circle.h"
#include <iostream>

using namespace cs40;

Painter::Painter(QWidget *parent) : QOpenGLWidget(parent) {
  //initialize any added variables
  m_blurFactor = 1.0;
  m_threshold = 30; 
  m_texture = NULL;
  m_vertexShader = NULL;
  m_fragmentShader = NULL;
  m_shaderProgram = NULL;
  vboVertices = NULL;
  vao = NULL;
  vertices = new QVector3D[4];
  vertices[0] = QVector3D(-1, -1, 0);
  vertices[1] = QVector3D(1, -1, 0);
  vertices[2] = QVector3D(-1, 1, 0);
  vertices[3] = QVector3D(1, 1, 0);
 
}

Painter::~Painter(){
  //delete dynamically allocated members
  m_texture->destroy();
  delete m_vertexShader; m_vertexShader=NULL;
  delete m_fragmentShader; m_fragmentShader=NULL;
  if(m_shaderProgram){
    m_shaderProgram->release();
    delete m_shaderProgram; m_shaderProgram=NULL;
  }
  for(int i = 0; i < m_strokes.length(); i++){
    delete m_strokes.last();
    m_strokes.pop_back();
  }

  delete [] vertices;

  vboVertices->destroy();
  vao->destroy();

}


QImage Painter::paintImage(int layers, int stopSize){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    if(stopSize > layers){
      std::cout << "Specified smallest brush size not valid, setting to 1." << std::endl;
      stopSize = 1;
    }

    QVector3D currentColor;
    QImage canvas = QImage(m_img);
    canvas.fill(Qt::white);
    QPixmap* dev = new QPixmap(m_img.size());
    dev->convertFromImage(canvas);
    QPainter painter;
    painter.begin(dev);
    int i = layers;
    
    while(i >= stopSize){
      //generate list of data for painter to draw
      //add to master list
      //draw master list with painter
      //create image for reference for next iteration
      std::cout << "Painting layer " << layers-i+1 << "..." << std::endl;
      m_strokes.append(getLayer(pow(2, i), canvas, m_threshold));
    
      //draw all strokes added to m_strokes
      for(int j = 0; j < m_strokes.length(); j++){
        currentColor = m_strokes[j]->getColor();
        painter.setPen(QColor(currentColor.x(), currentColor.y(), 
              currentColor.z()));
        painter.setBrush(QColor(currentColor.x(), currentColor.y(), 
              currentColor.z()));
        if(i == 1){
          painter.drawPoint(m_strokes[j]->getCenter().x(), m_strokes[j]->getCenter().y());
        }else{
          painter.drawEllipse(m_strokes[j]->getCenter().x(), 
              m_strokes[j]->getCenter().y(), pow(2, i-1), pow(2, i-1));
        }
      }

      canvas = dev->toImage();
      i--;
    }

    painter.end();
    canvas.save("../canvas.png");
    std::cout << "It's a masterpiece!" << std::endl;

    return canvas;
}



void Painter::initializeGL(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  createShaders();
  createVBOs();
 
  glClearColor(1, 1, 1, 1);
 
  m_img = QImage("../images/swat.jpg");
  m_img.fill(Qt::white);

  if(m_img.isNull()){ 
    std::cout << "No image loaded" << std::endl;
    return; 
  }

  m_texture = new QOpenGLTexture(m_img.mirrored()); 
  
  if(m_texture == NULL){
    std::cout << "Could not load texture" << std::endl;
    return;
  } else{
    std::cout << "Texture successfully loaded" << std::endl;
  }

  m_shaderProgram->bind();
}




void Painter::resizeGL(int w, int h)
{
      glViewport(0,0,w, h);
}



QImage Painter:: createReferenceImage(qreal brushSize){
  
  QGraphicsBlurEffect *blur = new QGraphicsBlurEffect();
  blur->setBlurRadius(brushSize);

  QGraphicsScene scene;
  QGraphicsPixmapItem item;
  item.setPixmap(QPixmap::fromImage(m_img));
  item.setGraphicsEffect(blur);
  scene.addItem(&item);
  QImage result = QImage(m_img.size(), QImage::Format_RGB32);
  result.fill(Qt::transparent);
  QPainter painter(&result);
  scene.render(&painter);

  return result;
  
}



//get the vector form of a QRgb color
QVector3D Painter::toVector(QRgb color){
   QColor col = QColor(color);
   int r = col.red();
   int g = col.green();
   int b = col.blue();

   return QVector3D(r, g, b);
}





QList<cs40::Drawable*> Painter::getLayer(qreal brushSize, QImage canvas, 
    qreal threshold){
  //create blurred reference image
  //for each position of the image with spacing brushSize
  //    get the area of point plus/minus brushSize/2 on all four sides
  //    calculate error of area compared to reference image
  //    if error is greater than threshold
  //            add stroke for point with the max error in the area


  QList<cs40::Drawable*> outList;
  QList<qreal> region = QList<qreal>(); //temporary region color error
  QImage ref = createReferenceImage(brushSize*m_blurFactor); //reference image
  qreal left, right, up, down, max, current;
  QVector2D maxErrorPoint;

  max = -1;
  //get the area and errors
  for(int x = 0; x < ref.width(); x +=  std::max(1.0, brushSize/2.0)){
    for(int y = 0; y < ref.height(); y += std::max(1.0, brushSize/2.0)){
      region.clear();
      maxErrorPoint = QVector2D(x, y);

      //GOOD PLACE TO USE CUDA OR PARALLEL PROCESSES
      //get points within desired region
      left = x;
      right = x+1;
      up = y;
      down = y-1;
      //left points
      while(left > x-brushSize/2.0 && left >= 0){
        
        //points greater than y
        while(up < y+brushSize/2.0 && up < ref.height()){
          current = (toVector(ref.pixel(left, up))-toVector(canvas.pixel(left,up))).length();
          if(current > max){ 
            max = current;
            maxErrorPoint = QVector2D(left, up);
          }
          region.append(current);

          up++;
        }
        
        //points less than y
        while(down > y-brushSize/2.0 && down >= 0){
          current = (toVector(ref.pixel(left, down))-toVector(canvas.pixel(left, down))).length();
          if(current > max){ 
            max = current;
            maxErrorPoint = QVector2D(left, down);
          }
          region.append(current);

          down--;
        }

        left--;
      
      }

      //right points -- do same thing
      while(right < x+brushSize/2.0 && right < ref.width()){
        
        //points greater than y
        while(up < y+brushSize/2.0 && up < ref.height()){
          current = (toVector(ref.pixel(left, up))-toVector(canvas.pixel(left,up))).length();
          if(current > max){ 
            max = current;
            maxErrorPoint = QVector2D(right, up);
          }
          region.append(current);

          up++;
        }
        
        //points less than y
        while(down > y-brushSize/2.0 && down >= 0){
          current = (toVector(ref.pixel(left, down))-toVector(canvas.pixel(left, down))).length();
          if(current > max){ 
            max = current;
            maxErrorPoint = QVector2D(right, down);
          }
          region.append(current);

          down--;
        }

        right++;
      
      }

      //get total error of area
      qreal areaError = 0;
      for(int j = 0; j < region.size(); j++){
        areaError += region[j];
      }

      //if error is greater than threshold, then add a stoke for max point
      if(float(areaError)/region.size() > threshold){
        Circle* circ = new Circle(m_shaderProgram, maxErrorPoint, 
            brushSize/2.0);
        circ->setColor(toVector(ref.pixel(maxErrorPoint.x(), 
                maxErrorPoint.y())));
        circ->setCenter(maxErrorPoint.x(), maxErrorPoint.y());
        outList.append(circ);
      }

    }
  }

  //shuffle contents of outList for painterly effects
  for(int k = 0; k < outList.length(); k++){
    int first = qrand() % outList.size();
    int second = qrand() % outList.size();
    outList.swap(first, second);
  }

  return outList;

}



void Painter::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(m_texture != NULL){
    m_texture->bind();
  } else{
    std::cout << "Texture not loaded." << std::endl;
    return;
  }

  if(!m_shaderProgram){ 
    std::cout << "No shader program" << std::endl;
    return; 
  } 
  
  if(!vboVertices) { 
    std::cout << "No vertices loaded" << std::endl;
    return; 
  }

  vao->bind();
  vboVertices->bind();
  m_shaderProgram->bind();
  m_shaderProgram->setUniformValue("tex0",0);
  m_shaderProgram->enableAttributeArray("vPosition");
  m_shaderProgram->setAttributeBuffer("vPosition", GL_FLOAT, 0, 3, 0);
  m_shaderProgram->enableAttributeArray("vTexture");
  m_shaderProgram->setAttributeBuffer("vTexture", GL_FLOAT, 4*sizeof(QVector3D), 2, 0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glFlush();
  vao->release();
  vboVertices->release();

}




void Painter::createShaders(){

  destroyShaders();

  m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
  if(!m_vertexShader->compileSourceFile("shaders/vshader.glsl")){
    std::cout << "error with vShader" << std::endl;
    qWarning() << m_vertexShader->log();
  }

  m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
  if(!m_fragmentShader->compileSourceFile("shaders/fshader.glsl")){
    std::cout << "error with fShader" << std::endl;
    qWarning() << m_fragmentShader->log();
  }

  m_shaderProgram = new QOpenGLShaderProgram();
  m_shaderProgram->addShader(m_vertexShader);
  m_shaderProgram->addShader(m_fragmentShader);

  if(!m_shaderProgram->link()){
    qWarning() << m_shaderProgram->log();
  }
}




void Painter::createVBOs(){
  //destroy current

  vao = new QOpenGLVertexArrayObject();
  bool vaoOK = vao->create();
  if(!vaoOK){
    std::cout << "Could not create VAO" << std::endl;
    return;
  }
  
  vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  bool vboOK = vboVertices->create();
  if(!vboOK){
    std::cout << "Coud not create VBO" << std::endl;
    return;
  }
  
  vboVertices->bind();
  vboVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
  vboVertices->allocate(4*sizeof(QVector3D));
  vboVertices->write(0, vertices, 4*sizeof(QVector3D));
  vboVertices->release();
}




void Painter::setupVAO(){
   if(!vboVertices or !m_shaderProgram->isLinked()){
     return;
   }

   vao->bind();
   m_shaderProgram->bind();
   vboVertices->bind();
   m_shaderProgram->enableAttributeArray("vPosition");
   m_shaderProgram->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4, 0);
   m_shaderProgram->release();
   vao->release();


}



void Painter::open() {
    /* get filename from dialog menu */
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open File", QString(".."));

    if (!fileName.isEmpty()) {
        /* attempt to make an image object */
        m_img = QImage(fileName);
        if (m_img.isNull()) {
            QMessageBox::information((QWidget*)parent(), "Image Viewer",
                                     QString("Cannot load %1.").arg(fileName));
            return;
        }

        m_texture->destroy();
        m_texture = new QOpenGLTexture(m_img.mirrored());
    }

    update();
}


void Painter::paint(){
  m_texture->destroy();
  m_texture = new QOpenGLTexture(paintImage(6).mirrored());
  update();

}

void Painter::setThreshold(qreal value){
  m_threshold = value;
}


void Painter::destroyShaders(){

    delete m_vertexShader; m_vertexShader=NULL;
    delete m_fragmentShader; m_fragmentShader=NULL;

    if(m_shaderProgram){
        m_shaderProgram->release();
        delete m_shaderProgram; m_shaderProgram=NULL;
    }
}

