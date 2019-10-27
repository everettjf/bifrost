//
//  bifrost_file_client.hpp
//  bifrost-server
//
//  Created by everettjf on 2019/4/25.
//  Copyright © 2019 everettjf. All rights reserved.
//

#ifndef bifrost_file_client_hpp
#define bifrost_file_client_hpp

#include <iostream>
#include "../common/bifrost_asio.hpp"
#include <fstream>
#include <sstream>
#include "../common/bifrost_config.hpp"
#include "../common/bifrost_file.hpp"
#include <thread>
#include <list>


namespace bifrost {
    class file_client : public file_handler{
    private:
        asio::io_service io_service;
        std::shared_ptr<asio::ip::tcp::socket> _socket;
    private:
        void do_connect() {
            asio::ip::tcp::resolver resolver(io_service);
            asio::ip::tcp::resolver::query query("localhost", std::to_string(config::value().file_client_connect_port));
            asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
            asio::ip::tcp::resolver::iterator end;
            
            _socket = std::make_shared<asio::ip::tcp::socket>(io_service);
            
            asio::error_code error = asio::error::host_not_found;
            while (error && endpoint_iterator != end){
                _socket->close();
                _socket->connect(*endpoint_iterator++, error);
            }
        }
    public:
        file_client() {
        }
        void run() {
            do_connect();
        }
        void close() {
            _socket->close();
        }
        bool send_file(const std::list<std::string>& files) {
            return file_handler::send_file(*_socket,files);
        }
        bool recv_file(){
            return file_handler::recv_file(*_socket);
        }
    };
}
#endif /* bifrost_file_client_hpp */
