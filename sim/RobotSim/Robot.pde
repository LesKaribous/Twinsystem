
PShape robotSVG;

class Robot{
  PVector position;
  
  Robot(){
    position = new PVector(215,215);
  }
  
  void draw(){
    pushMatrix();
    translate(position.x, position.y);
    rotate(position.z);
    shape(robotSVG, 0, 0, toPixel(700), toPixel(700));
    popMatrix();
  }
}
