
// author: niXman
// e-mail: i.nixman@gmail.com
// date: 28.05.2011
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _netclasses_server_hpp_included_
#define _netclasses_server_hpp_included_

#include <list>
#include <algorithm>

#include <netclasses/socket.hpp>
#include <netclasses/acceptor.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

namespace netclasses {

/***************************************************************************/

struct global_context_base {};

namespace detail {

/***************************************************************************/

struct server_base {
   typedef boost::function<bool(const boost::asio::ip::tcp::endpoint&)> on_connect_predicate;

   server_base(
      boost::asio::io_service& ios,
      const std::string& ip,
      boost::uint16_t port,
      on_connect_predicate on_connect
   ): _acceptor(ios, ip, port),
      _on_connect(on_connect),
      _started(false)
   {}

   void start() {
      if ( !_started ) {
         _started = true;
         _acceptor.async_accept(this, &server_base::on_accept_handler);
      }
   }

   void stop() {
      _acceptor.stop_accept();
      _started = false;
   }

protected:
   virtual void build(socket_ptr) = 0;

private:
   void on_accept_handler(
      socket_ptr socket,
      const boost::asio::ip::tcp::endpoint& ep,
      const boost::system::error_code& e)
   {
      if ( !e ) {
         if ( _on_connect && _on_connect(ep) ) {
            build(socket);
         }
      }
   }

private:
   acceptor _acceptor;
   on_connect_predicate _on_connect;
   bool _started;
};

/***************************************************************************/

/** without global_context */
template<bool hasGC, typename Impl, typename Void>
struct server_impl: server_base {
   server_impl(
      boost::asio::io_service& ios,
      const std::string& ip,
      boost::uint16_t port,
      server_base::on_connect_predicate on_connect
   ): server_base(ios, ip, port, on_connect)
   {}

protected:
   virtual void build(socket_ptr socket) {
      boost::shared_ptr<Impl> impl(new Impl(socket));
      impl->start();
   }
};

/** with global_context */
template<typename GC, typename Impl>
struct server_impl<true, GC, Impl>: server_base {
   server_impl(
      GC& global_context,
      boost::asio::io_service& ios,
      const std::string& ip,
      boost::uint16_t port,
      server_base::on_connect_predicate on_connect
   ): server_base(ios, ip, port, on_connect),
      _global_context(global_context)
   {}

protected:
   virtual void build(socket_ptr socket) {
      boost::shared_ptr<Impl> impl(new Impl(socket, _global_context));
      impl->start();
   }

private:
   GC& _global_context;
};

} // namespace detail

/***************************************************************************/

template<typename T0, typename T1 = void>
struct server: detail::server_impl<boost::is_base_of<global_context_base, T0>::value, T0, T1>, private boost::noncopyable {
   server(
      T0& global_context,
      boost::asio::io_service& ios,
      const std::string& ip,
      boost::uint16_t port,
      detail::server_base::on_connect_predicate on_connect
   ): detail::server_impl<boost::is_base_of<global_context_base, T0>::value, T0, T1>(global_context, ios, ip, port, on_connect)
   {}
   server(
      boost::asio::io_service& ios,
      const std::string& ip,
      boost::uint16_t port,
      detail::server_base::on_connect_predicate on_connect
   ): detail::server_impl<boost::is_base_of<global_context_base, T0>::value, T0, T1>(ios, ip, port, on_connect)
   {}
};

/***************************************************************************/

} // namespace netclasses

#endif // _netclasses_server_hpp_included_