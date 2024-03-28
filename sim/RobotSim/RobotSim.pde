
Table table = new Table();
Robot robot = new Robot();

void setup(){
  size(100 + 3000/3, 100 + 2000/3);
  tablePNG = loadImage("./data/vinyle.png");
  robotSVG = loadShape("./data/robot.svg");
  
  frameRate(30);
}


int mx = 0, my = 0;


void draw(){
  background(52);

  text(toMM(mouseX - 50), 20, 20);
  text(toMM(mouseY - 50), 20, 30);
  
  translate(50,50);
  
  
  robot.control();
  robot.integrate(1.0/30.0);
  
  table.draw();
  robot.draw();
  
  strokeWeight(10);
  point(mx- 50, my- 50);
  /*Draw SDF
  PVector l = table.grad_sdf(table.getCursorPos());
  float dist = -toPixel(table.sdf(table.getCursorPos()));
  line(mouseX - 50, mouseY - 50, (mouseX - 50) + l.x * dist, (mouseY - 50)+ l.y*dist);
  */
}



void mousePressed(){
  mx = mouseX;
  my = mouseY;
  robot.setTarget(table.getCursorPos());
}
