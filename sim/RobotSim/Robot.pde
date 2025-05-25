
PShape robotSVG;

class Robot{
  PVector position;
  PVector velocity;
  PVector acceleration;
  
  PVector target, newTarget;
  float vMax = 1000;
  float aMax = 2000;
  float slipThreshold = 10;
  float slipCorrectionFactor = 0.0;
  float stopThreshold = 20;
  float obstacleRepelThreshold = 600.0;
  float obstacleRepelStrength = 1000000000.0;
  float repelForceDamping = 0.05;
  PVector repelForce = new PVector(0, 0);
  
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
  
void control(float dt) {
    if (newTarget != target) target = newTarget;
    
    // Compute relative target position
    PVector reltarget = target.copy().sub(position);
    float d = reltarget.mag();
    
    // Desired velocity in the target direction
    PVector targetVel = reltarget.copy().normalize().mult(vMax);
    
    // Compute time needed to stop and estimated travel time
    float deccelTime = velocity.mag() / (aMax + 1e-5); // Prevent division by zero
    float travelTime = d / (velocity.mag() + 1e-5); // Prevent division by zero
    
    // Compute velocity error
    PVector velError = PVector.sub(targetVel, velocity);
    
    if (d < stopThreshold) { // Stop condition when close enough to target
        println("Stopping");
        acceleration.mult(0);
        velocity.mult(0);
        target = position.copy();
        return;
    } else if (deccelTime >= travelTime) { // Deceleration phase
        println("Decelerating");
        acceleration = velocity.copy().normalize().mult(-aMax); // Slow down gradually
    } else if (velError.mag() > 0.1) { // Acceleration phase
        println("Accelerating");
        acceleration = velError.normalize().mult(aMax);
    } else { // Constant velocity phase
        println("Constant speed");
        acceleration.mult(0);
    }
    
    // Predict future position further based on velocity
    float predictionFactor = PVector.dist(position, target) / vMax; // Scale based on target distance
    PVector predictedPosition = PVector.add(position, velocity.copy().mult(dt * predictionFactor));
    
    // Compute potential repulsion force from obstacles
    repelForce.mult(repelForceDamping);
    for (Obstacle obs : obstacles) {
        float sdf = obs.sdf(predictedPosition); // Check at future position
        if (sdf < obstacleRepelThreshold) { // Only consider close obstacles
            PVector direction = PVector.sub(predictedPosition, obs.position).normalize();
            
            // Apply velocity-dependent repulsion scaling
            float speedFactor = velocity.mag() / vMax; // Normalize speed effect
            float strength = (obstacleRepelStrength * (1 + speedFactor)) / ((sdf * sdf) + 1e-5); // Increase repulsion at higher speed
            float influence = (1 - (sdf / obstacleRepelThreshold));
            repelForce.add(direction.mult(strength * influence * influence)); // Exponentially scale repulsion
        }
    }
    
    // If the obstacle is extremely close, override target attraction completely
    if (repelForce.mag() > aMax) {
        acceleration.set(repelForce.limit(aMax)); // Fully prioritize avoidance
    } else {
        acceleration.add(repelForce.limit(aMax / 1.5)); // Gradual repulsion when not critically close
    }
    
    // Apply acceleration limit
    if (acceleration.mag() > aMax) {
        acceleration.normalize().mult(aMax);
    }
}
/**/
  
  /*
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
  */
  
  void setTarget(PVector t){
    newTarget = t;
  }
  
  void draw(){
    pushMatrix();
    translate(toPixel(position.x), toPixel(position.y));
    rotate(-position.z+PI/2);
    shape(robotSVG, 0, 0, toPixel(780), toPixel(780));
    
    
    float minSDF = 9999999;
    for (Obstacle obs : obstacles) {
        float sdf = obs.sdf(position); // Check at future position
        if(sdf < minSDF) minSDF = sdf;
    }
    
    if(minSDF < obstacleRepelThreshold) stroke(255,80,80);
    else stroke(80,255,80);
    
    noFill();
    strokeWeight(2);
    ellipse(0,0,2*toPixel(minSDF), 2*toPixel(minSDF));
    popMatrix();
  }
}
