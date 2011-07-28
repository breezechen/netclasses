
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

#include <netclasses/socket.hpp>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>

#include "../tests_config.hpp"

/***************************************************************************/

struct client_impl: boost::enable_shared_from_this<client_impl> {
   client_impl(boost::asio::io_service& ios, const std::string& ip, boost::uint16_t port)
      :socket(ios)
   {
      socket.connect(ip, port);
   }

   void write() {
      boost::shared_array<char> arr(new char[config::buffer_size]);
      strcpy(arr.get(), "data string");

      socket.async_write(arr, config::buffer_size, shared_from_this(), &client_impl::write_handler);
   }
   void read() {
      socket.async_read(config::buffer_size, shared_from_this(), &client_impl::read_handler);
   }

   void write_handler(const boost::system::error_code& e, boost::shared_array<char> arr, size_t wr) {
      fprintf(stdout, "write_handler(): %s, %d, e = %s\n", arr.get(), wr, e.message().c_str());
      fflush(stdout);
   }

   void read_handler(const boost::system::error_code& e, boost::shared_array<char> arr, size_t rd) {
      fprintf(stdout, "read_handler(): %s, %d, e = %s\n", arr.get(), rd, e.message().c_str());
      fflush(stdout);
   }

private:
   netclasses::socket socket;
};

/***************************************************************************/

int main() {
   try {
      boost::asio::io_service ios;

      boost::shared_ptr<client_impl> client(new client_impl(ios, config::ip, config::port));
      client->write();
      client->read();

      //ios.run();
   } catch ( const std::exception& e ) {
      std::cout << "(exception): " << e.what() << std::endl;
   }
}

/***************************************************************************/