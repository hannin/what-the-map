# what-the-map
Draw map from Shapefiles and associated attribute files and find the shortest way with GUI based on <a href="https://doc.qt.io/qt-5/">Qt-5.7</a>.

## Dependency
- <a href="https://doc.qt.io/qt-5/">Qt-5.7</a> for GUI (pre built in project already)
- <a href="http://shapelib.maptools.org/">shapelib</a> for loading Shapefiles and associated attribute file(.dbf) (source is in project already)
- <a href="https://git-lfs.github.com/">git-lfs</a> for managing large map data in git project. (installation needed before git clone)

## How-To-Use
Qt window comes up when executed. Every click on the map can select the point to start or end.

1. At first click, start point is selected which is the nearest from the mouse clicked point and the popup will notice the detail(node ID and node Name).
2. On the next click, end point is selected by same logic and show the detail with popup.
3. If client chose Ok, the way from the start to the end will be drawn with the blue highlighted on the map. Additionally, the link path (with link ID) is printed on console.
4. The next last mouse click will refresh the highlighted drawn path and the state is reset.

Except getting the shortest path, other UIs are intuitive like zoom in/out and scrolling.

## How-To-Build
Needs Visual C++ 14.0.<br>
For now, it's available/tested only on _Windows_ only. It tested on Windows 7 and 10.

1. Import project (by opening _what-the-map.vcxproj_ with visual studio).
2. Just build the project.

Pre built libraries are in 32 bit, so project should be built in x86 target only.

## How-To-Run
- Run executable(_what-the-map_) output in top of project directory.

Executable should be located in top of project directory because it should be with _qt dlls_ and map resource in _data/_.

## Remarks
- Before you git clone, <a href="https://git-lfs.github.com/">git-lfs</a> should be installed so as to clone large map data(>100MB).
- To easy build & run, all dependent sources & libs are built in the project (qt libs, headers and shapelib sources).
- All qt objects related classes(main-window and view) are preprocessed already so no any additional step needed.
- It can be just built and run in pure visual studio project.

## Screenshots
### View from far-away
![Alt text](/screenshots/full.png?raw=true)
### Find path
![Alt text](/screenshots/way-drawn.png?raw=true)
