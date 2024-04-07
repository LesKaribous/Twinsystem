PImage tablePNG;

class Table{
  PVector dim;
  
  Table(){
   dim = new PVector(3000,2000); 
  }
  
  float sdf( PVector pos){
    PVector p = pos.copy().sub(new PVector(dim.x/2.0, dim.y/2.0));
    PVector d = (new PVector(abs(p.x), abs(p.y))).sub(dim.copy().mult(0.5));
    return (new PVector(max(d.x,0.0), max(d.y,0.0))).mag() + min(max(d.x,d.y),0.0);
  }
  
  PVector grad_sdf(PVector pos){
    float x = sdf(pos.copy().add(new PVector(0.1,0,0))) - sdf(pos.copy().sub(new PVector(0.1,0,0)));
    float y = sdf(pos.copy().add(new PVector(0,0.1,0))) - sdf(pos.copy().sub(new PVector(0,0.1,0)));
    PVector grad = new PVector(x, y);
    return grad.normalize();
  }
  
  PVector getCursorPos(){
    return new PVector(toMM(mouseX - 50), toMM(mouseY - 50));
  }
  
  void draw(){
    pushMatrix();
    rotate(PI/2.0);
    translate(0,-1000);
    image(tablePNG, 0, 0, toPixel(2000), toPixel(3000));
    popMatrix();
  }
}
