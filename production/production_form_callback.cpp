﻿#include "framework.h"

namespace production
{

   form_callback::form_callback()
   {
   }

   form_callback::~form_callback()
   {
   }

   
   void form_callback::update(::user::form * pform, ::update & update)
   {

      UNREFERENCED_PARAMETER(pform);
      UNREFERENCED_PARAMETER(paction);

   }


   void form_callback::on_control_event(::user::control_event * pevent)
   {

      UNREFERENCED_PARAMETER(pevent);

   }


} // namespace production