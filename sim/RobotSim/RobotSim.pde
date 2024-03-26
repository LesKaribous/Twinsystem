
PImage tablePNG;
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
  
  drawTable();
  //rect(50,50,table_px.x, table_px.y);
  robot.draw();
  

}

void drawTable(){
  pushMatrix();
  rotate(PI/2.0);
  translate(10,-1000);
  image(tablePNG, 0, 0, toPixel(2000), toPixel(3000));
  popMatrix();
}

void drawRobot(PVector p) {

}
