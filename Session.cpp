//
// Created by raxzers on 6/2/18.
//

#include "Session.h"
Session::Session(boost::asio::io_service *ioservice) : tcp_socket(*ioservice){}

void Session::start() {
    std::cout<<"Cliente Conectado"<<std::endl;
    tcp_socket.async_read_some(boost::asio::buffer(buffer), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Session::read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    request.append(buffer, bytes_transferred);
    std::istringstream is(request);
    memset(buffer, 0, bytes_transferred);
    if(!ec){
        boost::property_tree::ptree pt;
        try{
            boost::property_tree::read_xml(is, pt);
            std::string response = RequestHandler::handle(pt);
            boost::asio::async_write(tcp_socket, boost::asio::buffer(response),
                                     boost::bind(&Session::write_handler, this, boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }catch(const boost::property_tree::xml_parser::xml_parser_error& ex){

            tcp_socket.async_read_some(boost::asio::buffer(buffer), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
        catch(boost::property_tree::ptree_bad_path){
            std::cout<<"not xml yet"<<std::endl;
            boost::asio::async_write(tcp_socket, boost::asio::buffer("\r\n\r\n"),
                                     boost::bind(&Session::write_handler, this, boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }
    }else if(ec.value() == 2){
        std::cout<<"Cliente desconectado"<<std::endl;
    }else{
        std::cout<<"Codigo de error: "<<ec<<std::endl;

        tcp_socket.async_read_some(boost::asio::buffer(buffer), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
}

void Session::write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if(!ec){
        std::cout<<"Request finalizado\n"<<std::endl;
        tcp_socket.async_read_some(boost::asio::buffer(buffer), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }else{
        std::cout<<"Error de escritura: "<<ec<<std::endl;
    }
}