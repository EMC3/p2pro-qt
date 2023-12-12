#include "colormap.h"
#include <cmath>
#include <fstream>
#include <cmdline.h>
#include "hardcoded_colormaps.h"
ColorMap::ColorMap(InternalMaps map){
    const unsigned int * ptr;
    switch(map){
    case PARULA:
        ptr = paddedParulaI;
        break;
    case HOTIRON:
        ptr = hotIronMap;
        break;
    case PLASMA:
        ptr = plasmaMap;
        break;
    case INFERNO:
        ptr = infernoMap;
        break;
    case MAGMA:
        ptr = magmaMap;
        break;
    case VIRIDIS:
        ptr = viridisMap;
        break;
    default:
        return;
    }

    values.resize(1000);
    for(int i = 0; i < 1000; i++){
        values[i] = ptr[i];
    }
}


ColorMap::ColorMap(std::string csvFile)
{
    std::ifstream ifs(csvFile);
    std::string line;
    while(std::getline(ifs, line)){
        CMDLine ln(line, CMDLine::OutOfRangeException, ",");
        uint32_t r = ln[0];
        uint32_t g = ln[1];
        uint32_t b = ln[2];

        if(r > 255 || g > 255 || b > 255){
            throw std::invalid_argument("Bad rgb in cmap");
        }

        uint32_t e = 0;
        e |= r<<16;
        e |= g<<8;
        e |= b;
        values.push_back(e);
    }
}

void ColorMap::setBounds(double min, double max)
{

    this->min = std::min(min, max);
    this->max = std::max(min, max);
    this->rng = this->max-this->min;
}

uint32_t ColorMap::getARGB(double v, int alpha)
{
    ssize_t maxIdx = values.size()-1;
    double offs = ((v - min)*double(maxIdx))/rng;
    ssize_t idx = std::max(0l, std::min(ssize_t(std::round(offs)), maxIdx));
    return values[idx] | (uint32_t(alpha)<<24);
}
