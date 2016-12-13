# what-the-map
Draw map from Shapefiles and associated attribute files and find the shortest way with GUI based on <a href="https://doc.qt.io/qt-5/">Qt-5.7</a>.

## Dependency
- <a href="https://doc.qt.io/qt-5/">Qt-5.7</a> for GUI
- <a href="http://shapelib.maptools.org/">shapelib</a> for loading Shapefiles and associated attribute file(.dbf)

## How-To-Use
Qt window comes up when executed. Every click on the map can select the point to start or end.<br>
At first click, start point is selected which is the nearest from the mouse clicked point and the popup will notice the detail(node ID and node Name).<br>
On the next click, end point is selected by same logic and show the detail with popup.<br>
If client chose Ok, the way from the start to the end will be drawn with the blue highlighted on the map. (For now, additionally the link path (with link ID) is printed on console).<br>
The next last mouse click will refresh the highlighted drawn path and the state is reset.<br>

Except getting the shortest path, other UIs are intuitive like zoom in/out and scrolling.<br>

## How-To-Run
For now, it's available/tested only on _Windows_ only.<br>
To run program without build, refer [here](deployment/README.md).<br>

## How-To-Build
Current development environment is based on visual-studio with Qt tool add-in for GUI.<br>
- Import visual studio project and install <a href="https://doc.qt.io/qt-5/">Qt-5.7</a> on Windows.
- Build & Run
- data files also needed in _./data/_ for running based on visual studio project. Refer [here](deployment/README.md).

## Screenshots
- View from far-away<br>
![Alt text](/full.png?raw=true)
- Find path.<br>
![Alt text](/way-drawn.png?raw=true)
