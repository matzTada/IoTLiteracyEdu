class Node {
  float x, y;
  int nodeid;
  int xbeeaddr;
  int destinationid;
  String name;
  String lastupdate;
  YSFGraph ysfgraph;

  Node() {
    x = 0.0;
    y = 0.0;
    nodeid = 0;
    xbeeaddr = 0;
    destinationid = 0;
    name = "";
    lastupdate = "";
    ysfgraph = new YSFGraph();
  }
  Node(int _nodeid, int _xbeeaddr, int _destinationid, String _name, String _lastupdate) {
    nodeid = _nodeid;
    xbeeaddr = _xbeeaddr;
    destinationid = _destinationid;
    name = _name;
    lastupdate = _lastupdate;
    ysfgraph = new YSFGraph();
  }

  void updateDrawParameter(float _x, float _y) {
    x = _x;
    y = _y;
  }

  void drawNode() {
    fill(255, 100);
    stroke(255, 0, 0);
    strokeWeight(3);
    ellipse(x, y, height/25, height/25);
    textSize(height / 50);
    fill(255);
    textAlign(CENTER);
    text("nodeid:" + nodeid + "\n"
      + "64L:" + hex(xbeeaddr, 8) + "\n"
      + "d_id:" + destinationid + "\n"
      + "name:" + name.trim() + "\n"
      + "lastupdate:" + lastupdate + "\n"
      , x, y);
    noFill();
    noStroke();
  }


  void drawPanel(float x, float y, float w, float h) {
    textAlign(LEFT);
    stroke(79, 0, 178);
    if (w/40<h/40)  strokeWeight(w/40);
    else strokeWeight(h/40);
    strokeJoin(ROUND);
    int tsize=0;
    if (w/15<=h/15) { 
      tsize=int(w)/10;
    } else { 
      tsize=int(h)/10;
    }
    PFont myFont = loadFont("BerlinSansFB-Reg-48.vlw");
    textFont(myFont);

    //color selection of background
    //fill(selectColorBasedOnTemperature(Temp));
    fill(0, 0, 0, 0);
    rect(x, y, w, h);

    //left window
    fill(255, 255, 200);
    rect(x, y+0*h/4, w/3, 3 * h/4);
    rect(x, y+0*h/4, w/3, h/4);
    fill(0, 0, 0);
    textSize(tsize);
    text("ID", x+w/40, y+h/10+0*h/4);
    textSize(tsize * 2.0);
    text(nodeid, x+w/8, y+h/5+0*h/4);

    //name window
    fill(255, 129, 25);
    rect(x, y+h*3/4, w, h/4);
    fill(255, 255, 255);
    textSize(tsize*2);
    text(name, x+w/20, y+h/5+h*3/4);
  }
};





void nodes_init() {
  nodes = new ArrayList<Node>();
}

void nodes_display() {
  //===> data fetch from database 
  int nodesNumber = nodes.size();
  int i = 0;
  int squareNumber = ceil(sqrt(nodesNumber));

  //displaying and sort
  //dynamic position calculation <===
  switch(positionType) {
  case 1: //linear
    for (Node tempNode : nodes) {
      tempNode.updateDrawParameter(
        (i + 0.5)/ nodesNumber *  width, 
        (i + 0.5)/ nodesNumber * height);
      i++;
    }
    for (Node tempNode : nodes) {
      tempNode.drawNode();
    }
    break;
  case 2: //Square Grid
    for (Node tempNode : nodes) {
      tempNode.updateDrawParameter(
        (i % squareNumber + 0.5) / squareNumber *  width, 
        (i / squareNumber + 0.5) / squareNumber * height);
      //      println(i + " " + i / squareNumber);
      i++;
    }
    for (Node tempNode : nodes) {
      tempNode.drawNode();
    }
    break;
  case 3: //circle
    float circleX = 0.35*width;
    float circleY = 0.35*height;
    for (Node tempNode : nodes) {
      tempNode.updateDrawParameter(
        circleX * cos((float) i / nodesNumber *  2 * PI) + 0.5 * width, 
        circleY * sin((float) i / nodesNumber *  2 * PI) + 0.5 * height);
      i++;
    }
    for (Node tempNode : nodes) {
      tempNode.drawNode();
    }
    break;
  case 4: 
    int cellWidth = width / squareNumber;
    int cellHeight = height / squareNumber;

    //graphs by Nakatsuka
    i = 0;
    for (Node tempNode : nodes) {
      tempNode.ysfgraph.drawGraph(
        (i % squareNumber) *  cellWidth + 0.34 * cellWidth, 
        (i / squareNumber) * cellHeight + cellHeight, 
        0.66 * cellWidth, 
        0.75 * cellHeight, 
        color(255), 
        15, 40, 
        tempNode.lastupdate);
      i++;
    }

    //cells by Niwacchi
    i = 0;    
    for (Node tempNode : nodes) {
      tempNode.drawPanel( 
        (i % squareNumber) * cellWidth, 
        (i/ squareNumber) * cellHeight + cellHeight, 
        cellWidth, 
        cellHeight);
      i++;
    }

    break;
  default: //list
    for (Node tempNode : nodes) {
      tempNode.updateDrawParameter(0, 0);
      i++;
    }
    for (Node tempNode : nodes) {
      tempNode.drawNode();
    }
    break;
  }
  //===> dynamic position calculation
}