ArrayList<Node> nodes;
int sortType = 0;
int positionType = 4;

int NUM_NODE = 20;
int CELL_X_NUM = 5;
int CELL_Y_NUM = 4;

void settings() {
  size(1200, 800);
}

void setup() {
  surface.setResizable(true); //for processing-3
  surface.setTitle("Visualizer");

  init_dynamicButton();
  nodes_init(NUM_NODE);
}

void draw() {
  int pastTime = millis();
  background(0);

  fill(255);
  textAlign(LEFT, TOP);
  textSize(height / 10);
  text("Visualizer", 5, 0);
  displaySortType(5, height / ceil(sqrt(nodes.size())) * 5 / 8);

  nodes_display();

  loop_dynamicButton();

  //loop time and framerate drawing <===
  int interval = millis() - pastTime;
  println("one loop by millis() interval: " + interval + "ms frameRate: " + frameRate);
  //===> loop time and framerate drawing
}

void keyPressed() {
  switch(key) {
  case 'p' :
    positionType++;
    if (positionType > 5) positionType = 0;
    break;  
  case 's' :
    sortType++;
    if (sortType > 2) sortType = 0;
    changeSortType();
    break;
  case 27:
    exit();
  default: 
    break;
  }
}

void mousePressed() {
  mouseClicked_dynamicButton();
}