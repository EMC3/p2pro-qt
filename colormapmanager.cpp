#include "colormapmanager.h"

ColorMapManager * ColorMapManager::inst = nullptr;

ColorMapManager::ColorMapManager() {

    cmaps.emplace_back(std::make_pair("Hotiron", ColorMap::HOTIRON));
    cmaps.emplace_back(std::make_pair("Jet", ":/misc/jet.csv"));
    cmaps.emplace_back(std::make_pair("Gray", ":/misc/gray.csv"));
    cmaps.emplace_back(std::make_pair("Plasma", ColorMap::PLASMA));
    cmaps.emplace_back(std::make_pair("Inferno", ColorMap::INFERNO));
    cmaps.emplace_back(std::make_pair("Magma", ColorMap::MAGMA));
    cmaps.emplace_back(std::make_pair("Viridis", ColorMap::VIRIDIS));
    cmaps.emplace_back(std::make_pair("Parula", ColorMap::PARULA));

}

ColorMap &ColorMapManager::getColorMap(std::string name)
{
    for(auto & cm : cmaps){
        if(cm.first == name){
            return cm.second;
        }
    }
    return cmaps[0].second;
}

int ColorMapManager::getIndex(std::string name)
{
    int idx = 1;
    for(auto & cm : cmaps){
        if(cm.first == name){
            return idx;
        }
        idx++;
    }
    return 0;
}

ColorMapManager *ColorMapManager::get()
{
    if(inst == nullptr)inst = new ColorMapManager();
    return inst;
}


