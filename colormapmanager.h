#ifndef COLORMAPMANAGER_H
#define COLORMAPMANAGER_H

#include "colormap/colormap.h"

class ColorMapManager
{
public:
    ColorMapManager();

    ColorMap & getColorMap(std::string name);

    std::vector<std::pair<std::string, ColorMap>> cmaps;

    static ColorMapManager * get();
    int getIndex(std::string name);

private:
    static ColorMapManager * inst;
};

#endif // COLORMAPMANAGER_H
