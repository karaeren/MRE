#include <iostream>

#include "./AppComponent.hpp"
#include "./controller/MyController.hpp"
#include "oatpp/network/Server.hpp"

void run() {
    /* Register Components in scope of run() method */
    AppComponent components;

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    /* Create MyController and add all of its endpoints to router */
    auto myController = std::make_shared<MyController>();
    myController->addEndpointsToRouter(router);

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
    oatpp::network::Server server(connectionProvider, connectionHandler);

    /* Priny info about server port */
    OATPP_LOGI("MRE", "Server running on port %s", connectionProvider->getProperty("port").getData());

    /* Run server */
    server.run();
}

/**
 *  main
 */
int main(int argc, const char *argv[]) {
    // Create OATPP Environment
    oatpp::base::Environment::init();

    run();

    // Destroy the OATPP Environment
    oatpp::base::Environment::destroy();

    return 0;
}
