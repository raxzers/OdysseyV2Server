#include <iostream>

#include "RAID/MemoryController.h"
int main() {
    /*boost::asio::io_service ioservice;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 2000);

    Server server(endpoint, &ioservice);
    //ServerHandler::loadSetUp();
    srand(time(nullptr));
    ioservice.run();
    return 0;*/
        MemoryController raid;
    //raid.upload("test");
    raid.checkFile("s");
}