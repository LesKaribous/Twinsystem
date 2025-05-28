
/*
  Occupancy Map Editor - Eurobot 2025
  -----------------------------------
  Un éditeur graphique pour générer une carte d’occupation
  - Peinture de tuiles (occupé/libre)
  - Sauvegarde automatique (.map + .h)
  - Export manuel du .h
  - Réinitialisation
  - Interface graphique avec quadrillage et image de fond
*/

import javax.swing.JFileChooser;
import java.io.File;

// === PARAMÈTRES GÉNÉRAUX ===
int cellSize = 150; // en mm
int scale = 50;     // pixels par cellule
int cols = 3000 / cellSize;
int rows = 2000 / cellSize;
int[][] grid = new int[rows][cols];

// === AUTO-SAVE ===
int autoSaveInterval = 30; // secondes
int lastAutoSaveTime = 0;
String lastSaveMessage = "";
int lastSaveDisplayTime = 0;
int saveMessageDuration = 3000; // ms

// === AIDE VISUELLE ===
boolean showHelp = true;
String[] helpText = {
  "[S] Sauvegarder le fichier .h",
  "[R] Réinitialiser la carte",
  "[H] Afficher/masquer cette aide",
  "[P] Path edit mode",
  "[Clic gauche] Peindre (occupé) ou ajouter un point en path mode",
  "[Clic droit] Effacer (libre)",
  "[Auto-save] toutes les " + autoSaveInterval + "s dans occupancy.map + .h"
};

// === RESSOURCES ===
PImage terrain;

boolean pathMode = false;
int startend = 0;

DijkstraPathfinder pathfinder = new DijkstraPathfinder(grid, rows, cols, cellSize);
PVector start = new PVector(0, 0); // mm
PVector end = new PVector(0, 0);  // mm
ArrayList<PVector> path = new ArrayList<PVector>();





// === INITIALISATION ===
void settings() {
  size(cols * scale, rows * scale);
}

void setup() {
  loadTerrain();
  loadExistingMap();
  //path = pathfinder.computePath(start, end);
}

// === AFFICHAGE PRINCIPAL ===
void draw() {
  background(255);
  drawTerrain();
  drawGridOverlay();
  drawOccupancy();
  //autoSaveIfNeeded();
  //drawSaveNotification();
  
  drawPath(path);
  
  if(pathMode){
    ellipse(start.x * scale / cellSize, start.y * scale / cellSize, 8, 8);
    ellipse(end.x * scale / cellSize, end.y * scale / cellSize, 8, 8);
    
    
    ellipse(mouseX, mouseY, 8, 8);
  }
  
  
  if (showHelp) drawHelpOverlay();
}

// === INTERACTIONS SOURIS ===
void mousePressed() {
  int gx = mouseX / scale;
  int gy = mouseY / scale;
  
  
  if(!pathMode){
    if (gx < cols && gy < rows) {
      grid[gy][gx] = (mouseButton == LEFT) ? 1 : 0;
    }
  }else{
    float x = gx * cellSize + cellSize / 2.0;
    float y = gy * cellSize + cellSize / 2.0;
    
    if (startend == 0) {
      start = new PVector(x, y);
    } else if (startend == 1) {
      end = new PVector(x, y);
    }
    
    startend++;
    
    if(startend == 2){
      path = pathfinder.computePath(start, end);
      
      print("Dijkstra generated : ");
      println(path.size());
      path = pathfinder.simplifyPath(path);
      print("Optimized to : ");
      println(path.size());
      startend = 0;
    }
  }
}



void drawPath(ArrayList<PVector> mypath) {
  if (mypath == null || mypath.size() < 2) return;

  stroke(0, 255, 0);
  strokeWeight(3);
  noFill();

  // Draw path lines
  for (int i = 0; i < mypath.size() - 1; i++) {
    PVector p1 = mypath.get(i);
    PVector p2 = mypath.get(i + 1);
    line(p1.x * scale / cellSize, p1.y * scale / cellSize,
         p2.x * scale / cellSize, p2.y * scale / cellSize);
  }

  // Draw path points
  fill(0, 255, 0);
  noStroke();
  for (PVector p : mypath) {
    ellipse(p.x * scale / cellSize, p.y * scale / cellSize, 8, 8);
  }
}






// === INTERACTIONS CLAVIER ===
void keyPressed() {
  if (key == 's') {
    JFileChooser chooser = new JFileChooser();
    chooser.setDialogTitle("Enregistrer le fichier .h");
    chooser.setSelectedFile(new File("occupancy_map.h"));
    if (chooser.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
      saveHeaderTo(chooser.getSelectedFile());
    }
  } else if (key == 'h') {
    showHelp = !showHelp;
  } else if (key == 'r') {
    clearGrid();
    showMessage("Carte réinitialisée");
  }else if (key == 'p') {
    pathMode = ! pathMode;
  }
}

// === CHARGEMENT ===
void loadTerrain() {
  terrain = loadImage("terrain.png");
  if (terrain != null) {
    terrain.resize(width, height);
    println("Image chargée : " + terrain.width + "x" + terrain.height);
  } else {
    println("Image non trouvée !");
  }
}

void loadExistingMap() {
  try {
    String[] lines = loadStrings("occupancy.map");
    for (int y = 0; y < min(rows, lines.length); y++) {
      String[] values = split(lines[y], ",");
      for (int x = 0; x < min(cols, values.length); x++) {
        grid[y][x] = int(values[x]);
      }
    }
    println("Map chargée depuis occupancy.map");
  } catch (Exception e) {
    println("Aucune map à charger, on part de zéro.");
  }
}

// === AFFICHAGE ===
void drawTerrain() {
  if (terrain != null) image(terrain, 0, 0);
}

void drawGridOverlay() {
  stroke(50);
  strokeWeight(1);
  noFill();
  for (int y = 0; y <= rows; y++) line(0, y * scale, cols * scale, y * scale);
  for (int x = 0; x <= cols; x++) line(x * scale, 0, x * scale, rows * scale);
}

void drawOccupancy() {
  noStroke();
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      if (grid[y][x] == 1) {
        fill(255, 0, 0, 128);
        rect(x * scale, y * scale, scale, scale);
      }
    }
  }
}

void drawHelpOverlay() {
  int boxHeight = helpText.length * 20 + 20;
  fill(0, 180);
  rect(0, 0, width, boxHeight);
  fill(255);
  textSize(14);
  textAlign(LEFT, TOP);
  for (int i = 0; i < helpText.length; i++) {
    text(helpText[i], 10, 10 + i * 20);
  }
}

void drawSaveNotification() {
  if (millis() - lastSaveDisplayTime < saveMessageDuration) {
    fill(0, 150);
    rect(10, 10, 220, 30, 8);
    fill(255);
    textSize(14);
    textAlign(LEFT, CENTER);
    text(lastSaveMessage, 20, 25);
  }
}

// === SAUVEGARDE ===
void autoSaveIfNeeded() {
  if (millis() - lastAutoSaveTime > autoSaveInterval * 1000) {
    saveMap();
    lastAutoSaveTime = millis();
  }
}

void saveMap() {
  File defaultFile = new File(sketchPath("occupancy.map"));
  saveMapTo(defaultFile);
}

void saveMapTo(File file) {
  PrintWriter output = createWriter(file.getAbsolutePath());
  for (int y = 0; y < rows; y++) {
    String line = "";
    for (int x = 0; x < cols; x++) {
      line += grid[y][x];
      if (x < cols - 1) line += ",";
    }
    output.println(line);
  }
  output.flush();
  output.close();

  // Sauvegarde du .h
  File headerFile = new File(file.getParent(), "occupancy_map.h");
  saveHeaderTo(headerFile);
}

void saveHeaderTo(File file) {
  PrintWriter output = createWriter(file.getAbsolutePath());
  output.println("// Auto-generated occupancy map");
  output.println("const uint8_t occupancy_map[" + cols + "][" + rows + "] = {");
  for (int x = 0; x < cols; x++) {
    String line = "  {";
    for (int y = 0; y < rows; y++) {
      line += grid[y][x];
      if (y < rows - 1) line += ", ";
    }
    line += "}";
    if (x < cols - 1) line += ",";
    output.println(line);
  }
  output.println("};");
  output.flush();
  output.close();
  showMessage(".h sauvegardé à " + getTimeString());
}

// === OUTILS ===
void clearGrid() {
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      grid[y][x] = 0;
    }
  }
}

void showMessage(String msg) {
  lastSaveMessage = msg;
  lastSaveDisplayTime = millis();
  println(msg);
}

String getTimeString() {
  return nf(hour(), 2) + ":" + nf(minute(), 2) + ":" + nf(second(), 2);
}
