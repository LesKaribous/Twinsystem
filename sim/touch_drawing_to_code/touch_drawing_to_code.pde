// A list of polylines to store the lines we draw
ArrayList<Polyline> polylines;

// A list of UI buttons
ArrayList<Button> buttons;
Button exportButton;
Button newButton;
Button undoButton;
float buttonWidth = 60;
float buttonHeight = 23;

// Distance between points
float pointDistance = 15;

boolean shouldSaveFrame = false;

boolean isDrawingLocked = false;

void setup() {
  // Processing setup
  //size(800,600);
  fullScreen(); 
  pixelDensity(displayDensity());
  // Add UI buttons
  buttons = new ArrayList<Button>();
  
  exportButton = new Button("Export", new Frame(50, 50, buttonWidth, buttonHeight), "EXPORT");
  newButton = new Button("New", new Frame(50, 80, buttonWidth, buttonHeight), "NEW");
  undoButton = new Button("Undo", new Frame(50, 110, buttonWidth, buttonHeight), "UNDO");
  
  buttons.add(exportButton);
  buttons.add(newButton);
  buttons.add(undoButton);
  
  // Force load fonts?
  for(Button b : buttons) { b.render(); }
  background(255);
  
  // Init drawing
  cleanup();
}

void draw() {
  background(255);
  for(Polyline p : polylines) { p.render(); }
  if(!shouldSaveFrame) {
    // Render controls (only when we are not exporting the frame)
    for(Button b : buttons) { b.render(); }
  } else {
    // Save a snapshot of the current drawing
    String date = str(year()).substring(2)+""+nf(month(),2)+""+nf(day(),2)+"_"+nf(hour(),2)+""+nf(minute(),2)+""+nf(second(),2);
    saveFrame("data/"+date+"_drawing.jpg");
    shouldSaveFrame = false;
  }
}

void updateUI() {
  if(polylines.size() == 0) {
    exportButton.isEnabled = false;
    newButton.isEnabled = false;
    undoButton.isEnabled = false;
  } else {
    exportButton.isEnabled = true;
    newButton.isEnabled = true;
    undoButton.isEnabled = true;
  }
}

// COMMANDS

// Empty existing polylines
void cleanup() {
  polylines = new ArrayList<Polyline>();
  updateUI();
}

// If there are polylines, remove last one
void undo() {
  int count = polylines.size();
  if(count > 0) {
    polylines.remove(count-1);
  }
  updateUI();
}

void export() {
  String code = "";

  for(Polyline p : polylines) {
    if(p.points.size() > 0) {
      code += p.toGCode();
    } else {
      println("skipped a polyline");    
    }
  }
  
  shouldSaveFrame = true;
  writeToFile(code);
  saveFile();
  print("Saved GCODE file and image!\n\r");
}

boolean isDrawing = false;
boolean isFirstPoint = false;
boolean isLastPoint = false;

Polyline activePolyline;

void mousePressed() {
  
   isButtonPressed = false;
   for(Button b : buttons) {
     if(b.isBelowMouse()) {
       isButtonPressed = true;
     }
   }
   
  if(mouseButton == LEFT && !isButtonPressed) {
    if(!isDrawing)isFirstPoint = true;
    isDrawing = true;
    activePolyline = new Polyline();
    polylines.add(activePolyline);
  }
}

void mouseReleased() {
  if(mouseButton == LEFT) {  
    if(isDrawing) isLastPoint = true;
    isDrawing = false;
  }
  updateUI();
}

long lastEventTimestamp = 0;
Point lastPoint = null;

void mouseDragged(){
  if(!isDrawingLocked) {
  
  long millis = millis();
  if(lastEventTimestamp < millis) {
    if(isDrawing) {
      Point p = new Point(mouseX, mouseY, (isFirstPoint || isLastPoint) ? 10 : 0 );
      isFirstPoint = false;
      isLastPoint = false;
      if(lastPoint == null || lastPoint.distanceTo(p) > pointDistance) {
        activePolyline.addPoint(p);
        lastEventTimestamp = millis;
        lastPoint = p;
      }
    }
  }
  }
}

boolean isButtonPressed = false;

void mouseClicked() {
 if(mouseButton == RIGHT) {
       undo();
 }
 if(mouseButton == CENTER) {
   cleanup(); 
 }
 if(mouseButton == LEFT) {
   for(Button b : buttons) {
     if(b.isBelowMouse()) {
       
       b.click();
       
       if(b.action == "EXPORT") {
         export();
       } else if(b.action == "NEW") {
         cleanup();
       } else if(b.action == "UNDO") {
         undo();
       }
     }
   }
 }
}

void keyPressed() {
  if(keyCode == 83) {
    export();
  }
    if(keyCode == 85) {
    undo();
  }
   if(keyCode == 76) {
   isDrawingLocked = !isDrawingLocked;
  }   if(keyCode == 27) {
   export();
  }

}
