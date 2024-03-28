
PShape robotSVG;

class Robot{
  PVector position;
  PVector velocity;
  PVector acceleration;
  
  PVector target;
  float vMax = 500;
  float aMax = 500;
  
  Robot(){
    position = new PVector(225,225);
    target = new PVector(225,225);
    velocity = new PVector(0,0);
    acceleration = new PVector(0,0);
    
  }
  
  void integrate(float dt){
    velocity.add(acceleration.copy().mult(dt));
    position.add(velocity.copy().mult(dt));
  }
  
  void control(){
    PVector reltarget = target.copy().sub(position);
    float d = reltarget.mag();
    if(d == 0){
      if(velocity.mag() != 0)velocity.mult(0);
    }
    
    float deccelTime = velocity.mag()/aMax;
    float travelTime = sqrt(2.0*d/aMax);
    
    if(deccelTime >= travelTime){ //we should deccel
      acceleration = reltarget.copy().normalize().mult(-aMax);
      if(velocity.mag()<10 ){//stop
        acceleration.mult(0);
      }
    }else if(velocity.mag() < vMax){
      acceleration = reltarget.copy().normalize().mult(aMax);
    }else{
      acceleration.mult(0);
    }
  }
  
  void setTarget(PVector t){
    target = t;
  }
  
  void draw(){
    pushMatrix();
    translate(toPixel(position.x), toPixel(position.y));
    rotate(-position.z+PI/2);
    shape(robotSVG, 0, 0, toPixel(780), toPixel(780));
    popMatrix();
  }
}
