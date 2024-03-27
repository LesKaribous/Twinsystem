
Table table = new Table();
Robot robot = new Robot();

void setup(){
  size(100 + 3000/3, 100 + 2000/3);
  tablePNG = loadImage("./data/vinyle.png");
  robotSVG = loadShape("./data/robot.svg");
}

void draw(){
  background(52);
  
  text(toMM(mouseX - 50), 20, 20);
  text(toMM(mouseY - 50), 20, 30);
  
  translate(50,50);
  
  table.draw();
  robot.draw();
  
  PVector l = table.grad_sdf(table.getCursorPos());
  float dist = toPixel(table.sdf(table.getCursorPos()));
  line(mouseX - 50, mouseY - 50, l.x * dist, l.y*dist);
}
