#include "MapLoader.h"
#include "Vector.h"
#include <cstring>
#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

/************************ This loads the map from openVDB.
                          You can use the native viewer once you complete this.
                          There are comments for each snippet of code.
                          Hint: http://www.openvdb.org/documentation/doxygen/codeExamples.html **************/



bool**
MapLoader::loadVDBMap(char* filename)
{
    // Initialize the OpenVDB library.  This must be called at least
    // once per program and may safely be called multiple times.
  // Create a VDB file object.
  openvdb::initialize();
  openvdb::io::File file(filename);
// Open the file.  This reads the file header, but not any grids.
  file.open();
  openvdb::GridBase::Ptr baseGrid;
  for (openvdb::io::File::NameIterator nameIter = file.beginName();nameIter != file.endName(); ++nameIter)
  {
    // Read in only the grid we are interested in.
      if (nameIter.gridName() == "SDF") 
      {
          baseGrid = file.readGrid(nameIter.gridName());
      } 
      else 
      {
          std::cout << "skipping grid " << nameIter.gridName() << std::endl;
      }
  }
  file.close();
  openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
  for (openvdb::MetaMap::MetaIterator iter = grid->beginMeta();
      iter != grid->endMeta(); ++iter)
  {
      const std::string& name = iter->first;
      openvdb::Metadata::Ptr value = iter->second;
      std::string valueAsString = value->str();
      std::cout << name << " = " << valueAsString << std::endl;
  }

  std::string s = grid->metaValue<std::string>("boundx");
  int boundx = stoi(s);
  x_boundary = boundx;
  s = grid->metaValue<std::string>("boundy");
  int boundy = stoi(s);
  y_boundary = boundy;
  s = grid->metaValue<std::string>("end_radius");
  endRadius = stof(s);
  s = grid->metaValue<std::string>("endx");
  int endx = stoi(s);
  s = grid->metaValue<std::string>("endy");
  int endy = stoi(s);
  s = grid->metaValue<std::string>("start_radius");
  startRadius = stof(s);
  s = grid->metaValue<std::string>("startx");
  int startx = stoi(s);
  s = grid->metaValue<std::string>("starty");
  int starty = stoi(s);

  posStart = Vec2f(startx,starty);
  posEnd = Vec2f(endx,endy);

  //bool **map;
  mapData = (bool **)malloc(sizeof(bool*)*boundx);
  grid_array = (float **)malloc(sizeof(float*)*boundx);
  for (int i = 0; i < boundx; ++i)
   {
      mapData[i] = (bool *)malloc(sizeof(bool)*boundy);
      grid_array[i] = (float *)malloc(sizeof(float)*boundy);
   } 

  openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
  for (int i = 0; i < boundx; ++i)
  {
    for (int j = 0; j < boundy; ++j)
    {
      openvdb::Coord xyz(i,j,0);
      float value = accessor.getValue(xyz);
      grid_array[i][j] = value;
      //std::cout << "Grid" << xyz << " = " << accessor.getValue(xyz) << std::endl;
      if(value < abs(5))
        mapData[i][j] = true;
      else
        mapData[i][j] = false;
      //std::cout << map[i][j]; 
    }
    //std::cout << "\n" ;
  }
  loaded = true;
  return mapData;

}

float**
MapLoader::getSDF(){
  return grid_array;

}

MapLoader::MapLoader() {

}

Vec2f
MapLoader::getStartPosition() {
  return posStart;
}

Vec2f
MapLoader::getEndPosition() {
  return posEnd;
}

unsigned int
MapLoader::gety_boundary() {
  return y_boundary;

}

unsigned int
MapLoader::getx_boundary() {
  return x_boundary;

}

unsigned int
MapLoader::getStartRadius() {

  return startRadius;

}

unsigned int
MapLoader::getEndRadius() {
  return endRadius;

}