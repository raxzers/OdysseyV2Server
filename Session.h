//
// Created by raxzers on 6/2/18.
//
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include "RequestHandler.h"
#ifndef ODYSSEYSERVER2_0_SESSION_H
#define ODYSSEYSERVER2_0_SESSION_H
using namespace boost::asio::ip;

class Session {private:
    /**
     * Buffer de memoria
     */
    char buffer[1048576];
    std::string request;

public:
    /**
     * Socket de red al cual esta conectado
     */
    tcp::socket tcp_socket;
    /**
     * Constructor de la sesion
     * @param ioservice Servicio de entradas y salidas
     */
    Session(boost::asio::io_service *ioservice);
    /**
     * Inicia la recepcion de la informacion una vez conectado
     */
    void start();
    /**
     * Maneja la entrada de datos a traves del socket
     * @param ec Codigo de error
     * @param bytes_transferred Datos recibidos
     */
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    /**
     * Maneja la salida de datos a traves del socket
     * @param ec Codigo de error
     * @param bytes_transferred Datos enviados
     */
    void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
};


#endif //ODYSSEYSERVER2_0_SESSION_H
