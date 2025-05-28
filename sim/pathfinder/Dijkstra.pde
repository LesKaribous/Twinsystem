import java.util.PriorityQueue;


// === DIJKSTRA PATHFINDING ===
class DijkstraPathfinder {
  int[][] map;
  int cols, rows;
  int cellSize;

  DijkstraPathfinder(int[][] grid, int rows_, int cols_, int cellSize_) {
    map = grid;
    rows = rows_;
    cols = cols_;
    cellSize = cellSize_;
  }

  ArrayList<PVector> computePath(PVector startMM, PVector endMM) {
    int startX = floor(startMM.x / cellSize);
    int startY = floor(startMM.y / cellSize);
    int endX = floor(endMM.x / cellSize);
    int endY = floor(endMM.y / cellSize);
  
    boolean[][] visited = new boolean[rows][cols];
    float[][] dist = new float[rows][cols];
    PVector[][] prev = new PVector[rows][cols];
  
    // Initialize distances
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        dist[y][x] = Float.MAX_VALUE;
        prev[y][x] = null;
      }
    }
  
    dist[startY][startX] = 0;
  
    ArrayList<PVector> openList = new ArrayList<PVector>();
    openList.add(new PVector(startX, startY));
  
    int[][] directions = {
      {1, 0}, {-1, 0}, {0, 1}, {0, -1},  // Cardinal
      {1, 1}, {-1, -1}, {1, -1}, {-1, 1} // Diagonal
    };
  
    while (openList.size() > 0) {
      // Find node with lowest distance
      int bestIndex = 0;
      float bestDist = dist[(int)openList.get(0).y][(int)openList.get(0).x];
      for (int i = 1; i < openList.size(); i++) {
        PVector p = openList.get(i);
        float d = dist[(int)p.y][(int)p.x];
        if (d < bestDist) {
          bestDist = d;
          bestIndex = i;
        }
      }
  
      PVector current = openList.remove(bestIndex);
      int cx = (int)current.x;
      int cy = (int)current.y;
  
      if (visited[cy][cx]) continue;
      visited[cy][cx] = true;
  
      if (cx == endX && cy == endY) break;
  
      for (int i = 0; i < directions.length; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int nx = cx + dx;
        int ny = cy + dy;
  
        if (nx < 0 || ny < 0 || nx >= cols || ny >= rows) continue;
        if (map[ny][nx] != 0 || visited[ny][nx]) continue;
  
        float stepCost = (abs(dx) + abs(dy) == 2) ? 1.00f : 1.0f; // Diagonal vs straight
        float newDist = dist[cy][cx] + stepCost;
  
        if (newDist < dist[ny][nx]) {
          dist[ny][nx] = newDist;
          prev[ny][nx] = new PVector(cx, cy);
          openList.add(new PVector(nx, ny));
        }
      }
    }
  
    // Reconstruct path
    ArrayList<PVector> path = new ArrayList<PVector>();
    PVector step = new PVector(endX, endY);
    if (prev[endY][endX] == null) return path; // No path found
  
    while (step != null) {
      float px = step.x * cellSize + cellSize / 2;
      float py = step.y * cellSize + cellSize / 2;
      path.add(0, new PVector(px, py));
      step = prev[(int)step.y][(int)step.x];
    }
  
    return path;
  }  
  
  
  ArrayList<PVector> simplifyPath(ArrayList<PVector> input) {
    ArrayList<PVector> simplified = new ArrayList<PVector>();
    if (input.size() < 2) return input;
  
    simplified.add(input.get(0));  // Always include first point
  
    for (int i = 1; i < input.size() - 1; i++) {
      PVector prev = input.get(i - 1);
      PVector curr = input.get(i);
      PVector next = input.get(i + 1);
  
      PVector dir1 = PVector.sub(curr, prev).normalize();
      PVector dir2 = PVector.sub(next, curr).normalize();
  
      // If direction changes, keep the current point
      if (!isSameDirection(dir1, dir2)) {
        simplified.add(curr);
      }
    }
  
    simplified.add(input.get(input.size() - 1)); // Always include last point
    return simplified;
  }
  
  // Small tolerance for direction equality
  boolean isSameDirection(PVector a, PVector b) {
    float angle = degrees(PVector.angleBetween(a, b));
    return abs(angle) < 1;  // You can increase tolerance to ~5–10° if needed
  }

}
