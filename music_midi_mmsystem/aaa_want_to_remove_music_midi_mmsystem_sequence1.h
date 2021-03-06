﻿#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((u32)(WORD)((BYTE)VERSION_MINOR | (((WORD)(BYTE)VERSION_MAJOR) << 8)))

#define MMSG_DONE                   (WM_USER+20)


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class file;
         class sequence;
         class sequence_thread;





         class CLASS_DECL_MUSIC_MIDI_MMSYSTEM sequence :
            virtual public ::music::midi::sequence
         {
         public:


            __pointer(midi)                   m_pmidi;
            __pointer(file)                   m_pfile;
            __pointer(sequence_thread)        m_psequencethread;

            u32                        m_cbPreroll;         

            u32                        m_cbPrerollNominalMax;

            buffer_array               m_buffera;

            HMIDISTRM                  m_hstream;

            midi_callback_data         m_midicallbackdata;

            i32                        m_iBuffersInMMSYSTEM;



            sequence(midi * pmidi, const string & strDevice);
            virtual ~sequence();

            virtual i64 add_ref(OBJ_REF_DBG_PARAMS)
            {

               return ::object::add_ref(OBJ_REF_DBG_ARGS);

            }

            virtual i64 dec_ref(OBJ_REF_DBG_PARAMS)
            {

               return ::object::dec_ref(OBJ_REF_DBG_ARGS);

            }



            void MuteAll(bool bMute = true, i32 iExcludeTrack = -1);
            void MuteTrack(i32 iIndex, bool bMute = true);


            virtual i32 GetDefaultCodePage();

            void Prepare(::ikaraoke::data & data);
            void Prepare(i32 iTrack, ::ikaraoke::data & data);
            void Prepare(
            string2a & str2a,
            imedia_position_2darray & tka2DTokensTicks,
            i32 iMelodyTrack,
            int2a & ia2TokenLine,
            ::ikaraoke::data & data);

            void SetLevelMeter(i32 iLevel);
            ::estatus     CloseStream();
            ::estatus     close_device();
            //bool SetMidiOutDevice(u32 uiDevice);
            i32 SetKeyShift(i32 iKeyShift);
            i32 GetKeyShift();

            void OnMidiPlaybackEnd(::music::midi::sequence::event * pevent);
            virtual ::estatus     SetTempoShift(double dTempoShift);

            void OnPositionCB(LPMIDIHDR lpmidihdr);
            void OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr);
            virtual void GetTimeLength(imedia_time & time);
            virtual void GetPositionLength(imedia_time & position);

            using ::ikaraoke::karaoke::TimeToPosition;
            using ::ikaraoke::karaoke::PositionToTime;

            virtual imedia_time TimeToPosition(imedia_time time);
            virtual imedia_time PositionToTime(imedia_time position);

            virtual bool IsOpened();

            virtual bool IsNull();
            virtual void SetTempoChangeFlag(bool bSet = true);
            virtual bool IsChangingTempo();

            virtual double GetTempoShift();
            //virtual void GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd);
            //virtual bool is_in_operation();
            //virtual bool WasInSpecialModeV001();

            virtual void OnEvent(::music::midi::sequence::event * pevent);


            ::estatus     close_file() override;

            ::estatus     Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow);
            ::estatus     Start();

            ::estatus     Pause();

            ::estatus     Restart();

            ::estatus     Stop();

            virtual imedia_time get_position_ticks() override;
            virtual void karaoke_get_time(imedia_time  & time) override;

            virtual ::estatus     get_ticks(imedia_time & time) override;
            virtual ::estatus     get_millis(imedia_time & time) override;


            bool IsPlaying();

            bool IsSettingPosition();
            void SetSettingPositionFlag(bool bSet = TRUE);

            imedia_time GetQuarterNote();



            using ::music::midi::sequence::create_new_event;
            virtual ::music::midi::sequence::event * create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr);

            virtual ::estatus     SendGMReset();

            virtual ::estatus     mm_start();

            virtual void music_midi_on_playback_end();


         };


      } // namespace mmsystem


   } // namespace midi


} // namespace music







