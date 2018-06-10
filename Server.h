//
// Created by raxzers on 6/2/18.
//
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>
#include "Session.h"

#ifndef ODYSSEYSERVER2_0_SERVER_H
#define ODYSSEYSERVER2_0_SERVER_H

using namespace boost::asio::ip;
class Server {

private:
    /**
     * Servicio de entradas y salidas
     */
    boost::asio::io_service  *ioservice;
    /**
     * Aceptor de conexiones TCP
     */
    tcp::acceptor tcp_acceptor;


public:
    /**
     * Constructor del servidor
     * @param endpoint Direccion ip en la que va a escuchar
     * @param io_service Servicio de entradas y salidas
     */
    Server(const tcp::endpoint &endpoint, boost::asio::io_service *io_service);
    /**
     * Inicia proceso de escucha
     */
    void start_accept();
    /**
     * Maneja las conexiones entrantes
     * @param ec Codigo de error en la transferencia d edatos
     * @param session Sesion de transferencia
     */
    void accept_handler(const boost::system::error_code &ec, Session *session);
};





#endif //ODYSSEYSERVER2_0_SERVER_H
