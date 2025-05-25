

class Obstacle{
 PVector position;
 float radius = 100;
 
 Obstacle(float x, float y){
   position = new PVector(x, y); 
 }
 
 
 float sdf(PVector point){
   return point.copy().sub(position).mag() - radius;
 }
  
 void draw(){
    pushMatrix();
    fill(100, 255, 100);
    strokeWeight(4);
    stroke(0);
    translate(toPixel(position.x), toPixel(position.y));
    ellipse(0, 0, toPixel(2*radius), toPixel(2*radius));
    popMatrix();
 }
  
}
