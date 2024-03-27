
PShape robotSVG;

class Robot{
  PVector position;
  
  Robot(){
    position = new PVector(225,225);
  }
  
  void draw(){
    pushMatrix();
    translate(toPixel(position.x), toPixel(position.y));
    rotate(-position.z+PI/2);
    shape(robotSVG, 0, 0, toPixel(780), toPixel(780));
    popMatrix();
  }
}
