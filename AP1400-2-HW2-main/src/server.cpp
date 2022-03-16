#include "server.h"
#include "client.h"
#include <random>

std::shared_ptr<Client> Server::add_client(std::string id)
{
    for (const auto &[ptr_client, money] : clients)
        if (ptr_client->get_id() == id)
        {
            // generating 4-digit random integer, code copied from https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distrib(1000, 9999);
            int random_number{distrib(gen)};

            // adding 4-digit random integer to the end of client's id
            id += std::to_string(random_number);
        }

    std::shared_ptr<Client> client{std::make_shared<Client>(id, *this)};
    clients[client] = 5.0;

    return client;
}
