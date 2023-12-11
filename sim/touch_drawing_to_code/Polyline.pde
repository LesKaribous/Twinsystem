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
   int pts = 0;
   String gcodePolyline = "";
    for(Point p : points) {
      pts++;
      String pointStr = "";
      // x, y, z
      
      pointStr += "PR[" + str(pts) + ":1]=";
      pointStr += str(p.x   -width/2) + "\n";
      pointStr += "PR[" + str(pts) + ":2]=";
      pointStr += str(p.y   -height/2)+ "\n";
      pointStr += "PR[" + str(pts) + ":3]=";
      pointStr += str(50)+ "\n"; 
      
      pointStr +="L PR[" + str(pts) + "] 500mm/sec FINE\n";
       
      // add Point to Polyline
      gcodePolyline += pointStr;
    }
    return gcodePolyline;
 }
 
}
