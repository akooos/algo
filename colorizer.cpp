#include "colorizer.h"



void Colorizer::generateClr()
{

    switch(genmethod){
    case EqDivision:{


        clr.hue = clr.hue +  (1.0f/m_est_colorcnt );
    }
    case EqDivisionWithRandomSaturation:{

        if ( clr.hue > 1)
            clr.hue = 0;

        clr.hue = clr.hue + (1.0f/m_est_colorcnt );

        clr.saturation = rand() % 1000 /1000.f; //0-1 között

    }

   }

}

Colorizer::Colorizer(ushort estimatedColorCount):m_est_colorcnt(estimatedColorCount),genmethod(Colorizer::EqDivisionWithRandomSaturation)
{
 clr.saturation = 0.5;
 clr.value      = 0.5;
}

Colorizer::Colorizer(ColorModels::HSV startColor, ushort estimatedColorCount):clr(startColor),m_est_colorcnt(estimatedColorCount),genmethod(Colorizer::EqDivisionWithRandomSaturation)
{

    clr.hue = 0;

}

Colorizer::~Colorizer()
{
}

ColorModels::HSV Colorizer::nextColor()
{
    generateClr();
    return clr;
}

#ifdef QT_CORE_LIB
#include <QColor>
QColor Colorizer::nextQColor()
{

  generateClr();

  QColor result;
  result.setHsvF(clr.hue,clr.saturation,clr.value);

  return result;
}
#endif //QT_CORE_LIB
