#include "client.h"
#include "server.h"

#include <random>

Client::Client(std::string id, const Server &server) : id{id}, server{&server}
{
    // setting public_key and private_key
    crypto::generate_key(public_key, private_key);
}

double Client::get_wallet() const
{
    return server->get_wallet(id);
}

std::string Client::sign(std::string txt) const
{
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) const
{
    std::string trx{id + "-" + receiver + "-" + std::to_string(value)};
    std::string signature{sign(trx)};

    return server->add_pending_trx(trx, signature);
}

size_t Client::generate_nonce() const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, 9999);
    return static_cast<size_t>(distrib(gen));
}
