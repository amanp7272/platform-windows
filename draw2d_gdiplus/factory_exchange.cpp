#include "framework.h"


extern "C"
void draw2d_gdiplus_factory_exchange()
{

   create_factory < ::draw2d_gdiplus::image, ::image >();
   create_factory < ::draw2d_gdiplus::graphics, ::draw2d::graphics >();
   create_factory < ::draw2d_gdiplus::bitmap, ::draw2d::bitmap >();
   create_factory < ::draw2d_gdiplus::pen, ::draw2d::pen >();
   create_factory < ::draw2d_gdiplus::brush, ::draw2d::brush >();
   create_factory < ::draw2d_gdiplus::region, ::draw2d::region >();
   create_factory < ::draw2d_gdiplus::font, ::draw2d::font >();
   create_factory < ::draw2d_gdiplus::path, ::draw2d::path >();

}



