#pragma once


namespace production
{

   class view;


   class CLASS_DECL_CA2_PRODUCTION production : 
      virtual public ::thread 
   {
   public:

      enum e_version
      {
         version_backup,
         version_basis,
         version_stage,
      };


      class release :
         virtual public thread
      {
      public:
         

         production *   m_pproduction;
         string         m_strRelease;
         string         m_strServer;


         release(production * pproduction,const char * pszRelease,const char * pszServer);

         virtual bool init_thread() override;

         virtual ::estatus     run() override;

         virtual ::estatus     raw_run();

      };


      class compress_thread :
         virtual public thread
      {
      public:
         compress_thread(production * pproduction, manual_reset_event * peventFinished);

         manual_reset_event * m_pevFinished;
         production *   m_pproduction;
         u32                m_dwThreadAffinityMask;
         virtual ::estatus     run() override;
      };

      string_array                 m_straMirror;
      string_array                 m_straMirrorStatus;
      ::mutex                   m_mutexStatus;
      string_array                 m_straStatus;
      bool                    m_bLoop;
      i32                     m_iLoop;
      i32                     m_iLoopCount;
      string                  m_strSubversionRevision;
      string_array                 m_straCompress;
      ::mutex                   m_mutexCompress;
      string                  m_strBuildTook;
      string                  m_strSpc;
      string                  m_strSignTool;
      string                  m_strSignPass;
      bool                    m_bReleased;
      string                  m_strStartTime;
      string                  m_strStdPostColor;
      string                  m_strBackPostColor;
      string                  m_strEmpPostColor;
      string                  m_strConfiguration;
      string                  m_strStatusEmail;
      i32                 m_iGlobalRetry;
      string                  m_strTry;
      string                  m_strDownloadSite;

      string                  m_strTwitterConsumerKey;
      string                  m_strTwitterConsumerSecret;

      ::mutex                   m_mutexRelease;
      string_array                 m_straRelease;
      i32                 m_iRelease;

      e_version               m_eversion;

      ::datetime::time        m_timeStart;
      ::datetime::time        m_timeEnd;

      string_array                 m_straStageDirAcceptedFileExtensions;

      i32                     m_iStep;
                              
      __pointer(::user::impact)                m_pview;
      bool                    m_bFinished;
      tick m_tickStart;
      tick m_tickEnd;
      string_array                 m_straCC;
      string_array                 m_straBB;
      string                  m_strBuild;
      string                  m_strFormatBuild;
      bool                    m_bEndProduction;
                              
      string                  m_strTag;
      ::file::path            m_strTagPath;
                              
      string                  m_strStatus;
      ::file::path            m_strBase;
      ::file::listing         m_straRoot;
      strsize                 m_iBaseLen;
      ::file::listing         m_straFiles;



      string_array                 m_straManifest;
      string_array                 m_straSignature;
      ::file::listing         m_straPath;


      bool                    m_bClean;
      bool                    m_bBuild;
                              
                              
      ::file::path            m_pathVrel;
      ::file::path            m_strCCAuth;
      ::file::path            m_strCCVrel;
      string                  m_strCCVrelNew;
      string                  m_strIndexMd5;

      bool                    m_bEndStatus;
      manual_reset_event      m_evFinish;


      production(::layered * pobjectContext);
      virtual ~production();



      void start_production(e_version eversion);
      void start_loop(e_version eversion, i32 iLoopCount);
      void step();

      void defer_quit();

      void add_status(const char * psz);
      void change_status(const char * psz);

      string version_to_international_datetime(const char * psz);


      virtual ::estatus     run() override;


      virtual ::estatus     produce();

      bool get_file_list(const char * pszBase, const char * pszDir, ::file::listing & stra, bool bFileSet = false);
      // bool ftp_put_dir(CFtpConnection & ftpconn, const char * pszBase, const char * pszDir, i32 iMode);

      virtual void compress();
      virtual bool compress_next();

      virtual void generate_appmatter_spa();
      virtual void generate_appmatter_spa(const ::file::path & pszRoot);
      virtual void generate_appmatter_spa_folder(const ::file::path & pszRoot,const ::file::path & pszRelative);
      virtual void generate_appmatter_spa_locale(const ::file::path & pszRoot,const ::file::path & pszRelative);
      virtual void generate_appmatter_spa_style(const ::file::path & pszRoot,const ::file::path & pszRelative);
      virtual void generate_appmatter_spa(const ::file::path & pszRoot,const ::file::path & pszRelative);

      virtual void release_production();
      virtual bool release_npca2(const char * pszPlatform);
      virtual bool release_iexca2(const char * pszPlatform);
      virtual bool release_crxca2(const char * pszPlatform);

      virtual void add_path(const ::file::path & pszDir, const ::file::path & pszRelative);

      bool sync_source(const char * psz, const char * pszRevision);
      bool commit_for_new_build_and_new_release();
      bool commit_source(const char * psz);


      virtual bool twitter_auth();
      virtual string twitter_twit(const char * pszMessage);

      virtual bool facebook_auth();
      virtual string facebook_status(const char * pszMessage);

      void compress(const ::file::path & lpszRelative);

      void OnUpdateRelease();

      string xpi_digest(memory & mem);

      void xpi_sign_dir(const char * pszPlatform, const ::file::path & pszDir);
      void xpi_section(const char * pszManifest, const char * pszSignature);

      bool create_xpi(const char * pszPlatform, bool bSigned = true);
      bool create_unsigned_xpi(const char * pszPlatform);
      bool create_signed_xpi(const char * pszPlatform);

      void build(const char * psz);

      virtual void update_rc_file_version(const char * pszUrl);

      virtual string stage_platform(string strPlatform);

   };


} // namespace production


