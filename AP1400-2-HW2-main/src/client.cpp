#include "client.h"
#include "server.h"

Client::Client(std::string id, const Server &server) : id{id}, server{&server}
{
    // setting public_key and private_key
    crypto::generate_key(public_key, private_key);
}
