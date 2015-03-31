#include "colorizer.h"



Colorizer::Colorizer(ushort estimatedColorCount):m_est_colorcnt(estimatedColorCount),cntr(0)
{
 clr.saturation = 0.5;
 clr.value      = 0.5;
}

Colorizer::Colorizer(ColorModels::HSV startColor, ushort estimatedColorCount):clr(startColor),m_est_colorcnt(estimatedColorCount),cntr(0)
{
}

Colorizer::~Colorizer()
{
}

#ifdef QT_CORE_LIB
#include <QColor>
QColor Colorizer::nextColor()
{

  ++cntr;
  cntr = cntr % m_est_colorcnt;

  clr.hue = (((double)cntr)/m_est_colorcnt );
  QColor result;
  result.setHsvF(clr.hue,clr.saturation,clr.value);



  return result;
}
#endif //QT_CORE_LIB
