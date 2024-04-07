
class Ray{
  PVector ro, rd;
  
  
};


class PolarPoint{
  float angle;
  float distance;
};


class Lidar{
 int resolution = 360;
 PVector position;
 
 Lidar(){
   position = new PVector(0,0);
 }
 
 void castRays(){
   for(int i = 0; i < resolution; i++){
      
   }
 }
 
 void setPosition(PVector pos){
   position = pos.copy();
 }
  
  
  
  
};


class BinaryMap{
  
  
};
