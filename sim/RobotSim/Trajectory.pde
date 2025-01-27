
class Waypoint{
  float speed;
  PVector position;
  Waypoint(PVector pos, float vel){
   speed = vel;
   position = pos;
  }
}

class Trajectory{
 int index = 0;
 ArrayList<Waypoint> waypoints;
 
 Trajectory(){
   waypoints = new ArrayList<Waypoint>();
 }
 
 void addPoint(PVector pos, float vel){
   waypoints.add(new Waypoint(pos.copy(), vel));
 }
 
 void reset(){
  index = 0; 
 }
 
 int next(){
   index++;
   if(index >= waypoints.size()){
     reset();
     return -1;
   }
   return index;
 };
 
 int size(){
   return waypoints.size(); 
 }
 
 Waypoint get(int i){
   if(i < waypoints.size()) return waypoints.get(i);
   else return waypoints.get(0);
 }
 
 PVector interpolate(PVector a, PVector b, float v){
    return a.copy().mult(1.0 - v).add(b.copy().mult(v));
 }
 
}
