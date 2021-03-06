#include "framework.h"
#include "aura/message.h"



namespace audio_mmsystem
{


   void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
   {

      if (uMsg == WOM_DONE)
      {

         LPWAVEHDR lpwavehdr = (LPWAVEHDR) dwParam1;

         auto pwaveout = (::audio_mmsystem::out *) dwInstance;

         pwaveout->m_psynththread->on_free(lpwavehdr->dwUser);

      }

   }


   out::out()
   {

      m_bDirectOutput      = true;
      m_bMessageThread     = true;
      m_estate             = state_initial;
      m_pthreadCallback    = nullptr;
      m_hwaveout           = nullptr;

   }


   out::~out()
   {

   }


   void out::install_message_routing(::channel * pchannel)
   {

      ::wave::out::install_message_routing(pchannel);

   }


   ::estatus out::init_thread()
   {

      if (!::wave::out::init_thread())
      {

         return ::error_failed;

      }

      return ::success;

   }


   void out::term_thread()
   {

      ::wave::out::term_thread();

      ::thread::term_thread();

   }


   ::estatus out::out_open_ex(thread * pthreadCallback, u32 uiSamplesPerSec, u32 uiChannelCount, u32 uiBitsPerSample,::wave::e_purpose epurpose)
   {

      sync_lock sl(mutex());

      if (m_hwaveout != nullptr && m_estate != state_initial)
      {

         return ::success;

      }

      m_imediatime = 0;

      MMRESULT mmresult = MMSYSERR_NOERROR;

      m_pthreadCallback = pthreadCallback;
      ::estatus     estatus;
      ASSERT(m_hwaveout == nullptr);
      ASSERT(m_estate == state_initial);

      m_pwaveformat->m_waveformat.wFormatTag        = WAVE_FORMAT_PCM;
      m_pwaveformat->m_waveformat.nChannels         = (WORD) uiChannelCount;
      m_pwaveformat->m_waveformat.nSamplesPerSec    = uiSamplesPerSec;
      m_pwaveformat->m_waveformat.wBitsPerSample    = (WORD) uiBitsPerSample;
      m_pwaveformat->m_waveformat.nBlockAlign       = m_pwaveformat->m_waveformat.wBitsPerSample * m_pwaveformat->m_waveformat.nChannels / 8;
      m_pwaveformat->m_waveformat.nAvgBytesPerSec   = m_pwaveformat->m_waveformat.nSamplesPerSec * m_pwaveformat->m_waveformat.nBlockAlign;
      

      auto audiowave = Audio.audiowave();

      try
      {

         mmresult = waveOutOpen(&m_hwaveout, audiowave->m_uiWaveOutDevice, wave_format(), (DWORD_PTR) &waveOutProc, (DWORD_PTR) this, CALLBACK_FUNCTION);

         if (mmresult == MMSYSERR_NOERROR)
         {

            TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutOpen: Success!!");

         }
         else
         {

            TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutOpen: ERROR %d!!", mmresult);

         }

         estatus = ::multimedia::mmsystem::translate(mmresult);

         if (estatus == ::success)
         {

            goto Opened;

         }

      }
      catch(const ::exception::exception &)
      {

         return error_failed;

      }
      catch(...)
      {

         return error_failed;

      }

      if(estatus != ::success)
      {

         TRACE(status_message(estatus));

         return estatus;

      }

Opened:

      int iBufferCount;
      int iBufferSampleCount;

      if(epurpose == ::wave::purpose_playback)
      {

         iBufferSampleCount = uiSamplesPerSec / 8;

         iBufferCount = 4;

      }
      else if(epurpose == ::wave::purpose_playground)
      {

         iBufferSampleCount = uiSamplesPerSec / 30;

         iBufferCount = 3;

      }
      else
      {

         iBufferSampleCount = uiSamplesPerSec / 20;

         iBufferCount = 4;

      }

      //int iPower2Size = 2;

      //while (iPower2Size < iBufferSampleCount)
      //{

      //   iPower2Size *= 2;

      //}

      //iBufferSampleCount = iPower2Size;

      int iBufferSize = iBufferSampleCount * uiBitsPerSample * uiChannelCount / 8;

      out_get_buffer()->PCMOutOpen(this, iBufferSize, iBufferCount,128, m_pwaveformat, m_pwaveformat);

      m_pprebuffer->open(m_pwaveformat->m_waveformat.nChannels, iBufferCount, iBufferSampleCount);

      index i;

      auto iSize = out_get_buffer()->GetBufferCount();

      for(i = 0; i < iSize; i++)
      {

         mmresult = waveOutPrepareHeader(m_hwaveout, ::multimedia::mmsystem::create_new_WAVEHDR(out_get_buffer(), i), sizeof(WAVEHDR));

         if (mmresult == MMSYSERR_NOERROR)
         {

            TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutPrepareHeader: Success!!");

         }
         else
         {

            TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutPrepareHeader: ERROR %d!!", mmresult);

         }

         estatus = ::multimedia::mmsystem::translate(mmresult);

         if (estatus != ::success)
         {

            MMRESULT mmresult2;

            while (i >= 1)
            {

               i--;

               mmresult2 = waveOutUnprepareHeader(m_hwaveout, wave_hdr(i), sizeof(WAVEHDR));

               if (mmresult2 == MMSYSERR_NOERROR)
               {

                  TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutUnprepareHeader: Cascade Success");

               }
               else
               {

                  TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutUnprepareHeader: Cascade ERROR %d!!", mmresult);

               }

            }

            MMRESULT mmresult3;

            while (i >= 1)
            {

               i--;

               mmresult3 = waveOutClose(m_hwaveout);

               if (mmresult3 == MMSYSERR_NOERROR)
               {

                  TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutClose: Cascade Success");

               }
               else
               {

                  TRACE("multimedia::audio_mmsystem::out::out_open_ex waveOutClose: Cascade ERROR %d!!", mmresult);

               }

            }


            TRACE("ERROR !! Failed to prepare output device buffers");

            return estatus;

         }

      }

      m_estate = state_opened;

      m_epurpose = epurpose;

      return ::success;

   }



   ::estatus     out::out_close()
   {

      sync_lock sl(mutex());

      if(m_estate == state_playing)
      {

         out_stop();

      }

      if (m_estate != state_opened)
      {

         return ::success;

      }

      ::estatus     estatus;

      index i;

      auto iSize =  out_get_buffer()->GetBufferCount();

      for(i = 0; i < iSize; i++)
      {

         if(::success != (estatus = ::multimedia::mmsystem::translate(waveOutUnprepareHeader(m_hwaveout, wave_hdr(i), sizeof(WAVEHDR)))))
         {
            TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer =%d", estatus);
         }

         delete wave_hdr(i);

      }

      estatus = ::multimedia::mmsystem::translate(waveOutClose(m_hwaveout));

      m_hwaveout = nullptr;

      ::wave::out::out_close();

      return ::success;

   }



   void out::out_filled(index iBuffer)
   {

      out_filled(wave_hdr(iBuffer));

   }


   void out::out_filled(LPWAVEHDR lpwavehdr)
   {

      sync_lock sl(mutex());

      if(out_get_state() != state_playing)
      {

         TRACE("ERROR out::BufferReady while out_get_state() != state_playing");

         return;

      }

      m_iBufferedCount++;

      ::estatus     estatus = ::multimedia::mmsystem::translate(waveOutWrite(m_hwaveout, lpwavehdr, sizeof(WAVEHDR)));

      if(estatus != ::success)
      {

         m_iBufferedCount--;

      }

   }


   ::estatus     out::out_stop()
   {

      sync_lock sl(mutex());

      if (m_estate != state_playing && m_estate != state_paused)
      {

         return error_failed;

      }

      //m_pprebuffer->stop();

      m_estate = state_stopping;

      // waveOutReset
      // The waveOutReset function stops playback on the given
      // waveform-audio_mmsystem output device and resets the current position
      // to zero. All pending playback buffers are marked as done and
      // returned to the application.
      m_estatusWave = ::multimedia::mmsystem::translate(waveOutReset(m_hwaveout));

      if(m_estatusWave == ::success)
      {

         m_estate = state_opened;

      }

      return m_estatusWave;

   }


   ::estatus     out::out_pause()
   {

      single_lock sLock(mutex(), TRUE);

      ASSERT(m_estate == state_playing);

      if(m_estate != state_playing)
         return error_failed;

      // waveOutReset
      // The waveOutReset function stops playback on the given
      // waveform-audio_mmsystem output device and resets the current position
      // to zero. All pending playback buffers are marked as done and
      // returned to the application.

      m_estatusWave = ::multimedia::mmsystem::translate(waveOutPause(m_hwaveout));

      ASSERT(m_estatusWave == ::success);

      if(m_estatusWave == ::success)
      {
         m_estate = state_paused;
      }

      return m_estatusWave;

   }

   ::estatus     out::out_restart()
   {

      sync_lock sl(mutex());

      ASSERT(m_estate == state_paused);

      if(m_estate != state_paused)
         return error_failed;

      // waveOutReset
      // The waveOutReset function stops playback on the given
      // waveform-audio_mmsystem output device and resets the current position
      // to zero. All pending playback buffers are marked as done and
      // returned to the application.
      m_estatusWave = ::multimedia::mmsystem::translate(waveOutRestart(m_hwaveout));

      ASSERT(m_estatusWave == ::success);

      if(m_estatusWave == ::success)
      {

         m_estate = state_playing;

      }

      return m_estatusWave;

   }


   imedia_time out::out_get_time()
   {

      return m_timeStart + device_out_get_time();

   }


   imedia_time out::device_out_get_time()
   {

      sync_lock sl(mutex());

      ::estatus                    estatus;

      MMTIME mmt = {};

      mmt.wType = TIME_BYTES;

      if(m_hwaveout != nullptr)
      {

         estatus = ::multimedia::mmsystem::translate(waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt)));

         try
         {

            if (::success != estatus)
            {

               TRACE( "waveOutGetPosition() returned %lu", (u32)estatus);

               //      return MCIERR_DEVICE_NOT_READY;

               return 0;

            }

         }
         catch(...)
         {

            //return MCIERR_DEVICE_NOT_READY;

            return 0;

         }

         if(mmt.wType == TIME_BYTES)
         {

            double d = (mmt.u.cb* 8.0)/ (m_pwaveformat->m_waveformat.wBitsPerSample * m_pwaveformat->m_waveformat.nChannels * m_pwaveformat->m_waveformat.nSamplesPerSec);

            return (double) d;

         }
         else
         {

            //*pTicks += mmt.u.ticks;

            return (u32) mmt.u.ms;

         }

      }
      else
      {

         return 0;

      }

   }


   //imedia_time out::out_get_time()
   //{

   //   return m_positionStart + device_out_get_time();

   //}


   //imedia_time out::device_out_get_time()
   //{

   //   sync_lock sl(mutex());

   //   ::estatus                    estatus;

   //   MMTIME                  mmt = {};

   //   mmt.wType = TIME_BYTES;

   //   if(m_hwaveout != nullptr)
   //   {

   //      estatus = ::multimedia::mmsystem::translate(waveOutGetPosition(m_hwaveout, &mmt, sizeof(mmt)));

   //      try
   //      {

   //         if (::success != estatus)
   //         {

   //            TRACE( "waveOutGetPosition() returned %lu", (u32)estatus);

   //            return 0;

   //         }

   //      }
   //      catch(...)
   //      {

   //         return 0;

   //      }

   //      if(mmt.wType == TIME_MS)
   //      {

   //         imedia_time position = (u32) mmt.u.ms;

   //         position *= m_pwaveformat->nSamplesPerSec;

   //         position /= 8 * 1000;

   //         return position;

   //      }
   //      else if (mmt.wType == TIME_BYTES)
   //      {

   //         return (mmt.u.cb * 8) / (m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels);

   //      }
   //      else
   //      {

   //         return 0;

   //      }

   //   }
   //   else
   //   {

   //      return 0;

   //   }

   //}


   void out::out_free(index iBuffer)
   {

      ::wave::out::out_free(iBuffer);

   }


   void out::out_on_playback_end()
   {

      out::e_state estate = out_get_state();

      out_stop();

      m_pprebuffer->m_pstreameffectOut.release();

      m_pplayer->post_event(::wave::player::player_event_playback_end);

   }


   WAVEFORMATEX * out::wave_format()
   {

      ::multimedia::mmsystem::translate(m_waveformatex, m_pwaveformat);

      return &m_waveformatex;

   }


   HWAVEOUT out::out_get_safe_HWAVEOUT()
   {

      if (this == nullptr)
      {

         return nullptr;

      }

      return m_hwaveout;

   }


   void * out::get_os_data()
   {

      return m_hwaveout;

   }


   LPWAVEHDR out::wave_hdr(index iBuffer)
   {

      return ::multimedia::mmsystem::get_os_data(out_get_buffer(), iBuffer);

   }


} // namespace audio_mmsystem



