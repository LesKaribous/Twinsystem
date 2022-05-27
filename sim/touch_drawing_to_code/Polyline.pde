class Polyline {

 ArrayList<Point> points = new ArrayList<Point>();
 float thickness = 10.5;
 
 void addPoint(float x, float y, long z) {
   points.add(new Point(x,y,z));
 }
 
 void addPoint(Point p) {
   points.add(p);
 }
 
 void render() {
   pushStyle();
  noFill();
  strokeWeight(thickness);
  beginShape();
  for(Point p : points) {
    vertex(p.x, p.y);
  }
  endShape();
  popStyle();
 }
 
 String toGCode() {
   
   String gcodePolyline = "";
    for(Point p : points) {
      String pointStr = "go(";
      // x, y, z
      pointStr += str(p.x-width/2) + ",";
      pointStr += str(p.y-height/2)+ ");\n";
      
      // add Point to Polyline
      gcodePolyline += pointStr;
    }
    return gcodePolyline;
 }
 
}
