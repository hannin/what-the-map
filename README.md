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

## How-To-Build
For now, it's available/tested only on _Windows_ only.<br>
Import project (by opening _what-the-map.vcxproj_ with visual studio).<br>
Just build the project.<br>

## How-To-Run
Run executable(_what-the-map_) output in top of project directory.<br>
Executable should be located in top of project directory because it should be with _qt dlls_ and map resource in _data/_. <br>

## Remarks
To easy build & run, all dependent sources & libs are built in the project (qt libs, headers and shapelib sources).<br>
All qt objects related classes(main-window and view) are preprocessed already so no any additional step needed.<br>
It can be just built and run in pure visual studio project.<br>

## Screenshots
- View from far-away<br>
![Alt text](/full.png?raw=true)
- Find path.<br>
![Alt text](/way-drawn.png?raw=true)
