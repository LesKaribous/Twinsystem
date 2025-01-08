
PShape robotSVG;

class Robot{
  PVector position;
  PVector velocity;
  PVector acceleration;
  
  PVector target, newTarget;
  float vMax = 1000;
  float aMax = 2000;
  
  Robot(){
    position = new PVector(225,225);
    target = new PVector(225,225);
    newTarget = new PVector(225,225);
    velocity = new PVector(0,0);
    acceleration = new PVector(0,0);
    
  }
  
  void integrate(float dt){
    velocity.add(acceleration.copy().mult(dt));
    position.add(velocity.copy().mult(dt));
  }
  
  /*
  void control(){
    PVector reltarget = target.copy().sub(position);
    float d = reltarget.mag();
    if(d == 0){
      if(velocity.mag() < 5) velocity.mult(0);
      return;
    }
    
    PVector targetVel = reltarget.copy().normalize().mult(vMax);
    
    float deccelTime = velocity.mag()/aMax;
    float travelTime = sqrt(2.0*d/aMax);
    
    if(deccelTime > travelTime){ //we should deccel
      PVector velError = PVector.sub(targetVel, velocity);
      acceleration = velError.normalize().mult(aMax);
      
      if(velocity.mag()<100){//stop
        acceleration.mult(0);
        velocity.mult(0);
        position = target.copy();
      }
    }else if(velocity.mag() < vMax){
      PVector velError = PVector.sub(targetVel, velocity);
      acceleration = velError.normalize().mult(aMax);
    }else{
      acceleration.mult(0);
    }
  }
  */
  
  
  void control(){
    if(newTarget != target) target = newTarget;
    PVector reltarget = target.copy().sub(position);
    float d = reltarget.mag();    
    PVector targetVel = reltarget.copy().normalize().mult(vMax);
    float deccelTime = velocity.mag()/aMax;
    float travelTime = d/velocity.mag();
    
    PVector velError = PVector.sub(targetVel, velocity);
    
    if(deccelTime >= travelTime){ //we should deccel
      println("deccel");
      PVector velError2 = PVector.sub(targetVel.copy().mult(0), velocity);
      acceleration = velError2.normalize().mult(aMax);
      
      if(velocity.mag()<100){//stop
        acceleration.mult(0);
        velocity.mult(0);
        position = target.copy();
      }
    }else if(velError.mag() > 0.1){
      println("accel");
      acceleration = velError.normalize().mult(aMax);
    }else{
      println("constant");
      acceleration.mult(0);
    }
  }
  
  
  void setTarget(PVector t){
    newTarget = t;
  }
  
  void draw(){
    pushMatrix();
    translate(toPixel(position.x), toPixel(position.y));
    rotate(-position.z+PI/2);
    shape(robotSVG, 0, 0, toPixel(780), toPixel(780));
    popMatrix();
  }
}
