#pragma once


namespace user_service
{


   class CLASS_DECL_APP_CORE_USER_SERVICE socket_handler :
      virtual public ::sockets::socket_handler
   {
   public:


      socket_handler(::layered * pobjectContext);


   };


} // namespace user_service



