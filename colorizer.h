#ifndef COLORIZER_H
#define COLORIZER_H

#include "defs.h"

namespace ColorModels{

   struct HSV{
        double hue; // 0 - 1;
        double saturation; //0-1
        double value; //brightness 0-1

        HSV():hue(0),saturation(0),value(0){}

    };

    struct RGB{
        uchar red; //0-255
        uchar green;//0-255
        uchar blue;//0-255
        RGB():red(0),green(0),blue(0){}
    };
}


class Colorizer
{
    ColorModels::HSV  clr;
    ushort m_est_colorcnt;

    ushort cntr;

public:
    Colorizer(ushort estimatedColorCount = 255);
    Colorizer( ColorModels::HSV  startColor, ushort estimatedColorCount = 255);

    ~Colorizer();

    ushort estimatedColorCount() const{
        return m_est_colorcnt;
    }

    void setEstimatedColorCount(ushort value){
      m_est_colorcnt = value;
    }

    const ColorModels::HSV &  colorHSV() const {
        return clr;
    }
    void setHSV(const ColorModels::HSV  &value) {
        clr = value;
    }
#ifdef QT_CORE_LIB
    QColor nextColor();
#endif //QT_CORE_LIB
};

#endif // COLORIZER_H
