﻿#pragma once


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class CLASS_DECL_MUSIC_MIDI_MMSYSTEM file :
            virtual public ::music::midi::file
         {
         public:


            imedia_time m_tkLastOp;


            file(::object * pobject);
            virtual ~file();


            ::estatus     WorkStreamRender(LPMIDIHDR lpmh, imedia_position tkMax, u32 cbPrerollNomimalMax);

            using ::music::midi::file::WorkSeek;

            ::estatus     WorkSeek(imedia_position tkPosition, LPMIDIHDR lpmh);

            ::estatus     StreamEvent(imedia_position tkDelta, ::music::midi::event * Event, LPMIDIHDR lpmh, imedia_position tkMax, u32 cbPrerollNomimalMax);

            ::estatus     StreamEventF1(imedia_position tkDelta, array < ::music::midi::event *, ::music::midi::event * > & eventptra, LPMIDIHDR lpmh, imedia_position tkMax, u32 cbPrerollNomimalMax);

            ::estatus     InsertParmData(imedia_position tkDelta, LPMIDIHDR lpmh);

            ::estatus     InsertPadEvent(imedia_position tkDelta, LPMIDIHDR lpmh);


         };


      } // namespace mmsystem


   } // namespace midi


} // namespace music




