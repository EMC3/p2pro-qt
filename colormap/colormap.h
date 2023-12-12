#ifndef COLORMAP_H
#define COLORMAP_H


#include <cstdint>
#include <string>
#include <vector>
class ColorMap
{
public:
    enum InternalMaps{
        PARULA, HOTIRON, PLASMA, INFERNO, MAGMA, VIRIDIS
    };

    ColorMap(std::string csvFile);
    ColorMap(InternalMaps map);
    void setBounds(double min, double max);
    uint32_t getARGB(double v, int alpha = 255);
private:
    std::vector<uint32_t> values;
    double min, max, rng;
};

#endif // COLORMAP_H
