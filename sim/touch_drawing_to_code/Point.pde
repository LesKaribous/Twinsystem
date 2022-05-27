
class Point {
 
  float x;
  float y;
  float z;
  
  Point(float x, float y) {
    this.x = x;
    this.y = y;
  }
  
  Point(float x, float y, long z) {
    this.x = x;
    this.y = y;
    this.z = z;
    
  }
  
  float distanceTo(Point p) {
    return sqrt(pow(p.x - this.x, 2) + pow(p.y - this.y, 2));
  }
  
}
