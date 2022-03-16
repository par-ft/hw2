#include "server.h"
#include "client.h"

#include <random>
#include <sstream>

std::vector<std::string> pending_trxs{};

void show_wallets(const Server &server)
{
    std::cout << std::string(20, '*') << std::endl;
    for (const auto &client : server.clients)
        std::cout << client.first->get_id() << " : " << client.second << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}

std::shared_ptr<Client> Server::add_client(std::string id)
{
    if (!clients.empty())
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
                break;
            }

    std::shared_ptr<Client> ptr_client{std::make_shared<Client>(id, *this)};
    clients[ptr_client] = 5.0;

    return ptr_client;
}

std::shared_ptr<Client> Server::get_client(std::string id) const
{
    if (!clients.empty())
        for (const auto &[ptr_client, money] : clients)
            if (ptr_client->get_id() == id)
                return ptr_client;

    return std::shared_ptr<Client>{};
}

double &Server::get_wallet(std::string id) const
{
    if (!clients.empty())
        for (auto &element : clients)
            if (element.first->get_id() == id)
                return element.second;

    throw std::runtime_error{"There is no client matching this id"};
}

bool Server::parse_trx(std::string txt, std::string &sender, std::string &receiver, double &value)
{
    std::stringstream trx(txt);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(trx, segment, '-'))
    {
        seglist.push_back(segment);
    }

    if (seglist.size() != 3)
        throw std::runtime_error{"Input string is not standard"};

    sender = seglist[0];
    receiver = seglist[1];
    value = std::stod(seglist[2]);

    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const
{
    std::string sender{}, receiver{};
    double value{};

    if (!parse_trx(trx, sender, receiver, value))
    {
        std::cout << "false1\n";
        return false;
    }

    if (get_client(sender) == nullptr || get_client(receiver) == nullptr || get_wallet(sender) < value)
    {
        std::cout << "false2\n";
        return false;
    }

    if (!crypto::verifySignature(get_client(sender)->get_publickey(), trx, signature))
    {
        std::cout << "false3\n";
        return false;
    }

    pending_trxs.push_back(trx);
    return true;
}

size_t Server::mine()
{
    std::string mempool{};
    std::vector<std::string> clients{};
    std::string sender{}, receiver{}, winner{};
    double value{};
    size_t nonce{};

    /*****************************************************************************************************/
    for (const auto &trx : pending_trxs)
    /*****************************************************************************************************/
    {
        parse_trx(trx, sender, receiver, value);
        get_wallet(sender) -= value;
        get_wallet(receiver) += value;

        clients.push_back(sender);
        clients.push_back(receiver);
        mempool += trx;
    }

    bool nonce_finded{false};
    while (!nonce_finded)
    {
        for (auto &w : clients)
        {
            nonce = get_client(w)->generate_nonce();
            std::string hash{crypto::sha256(mempool + std::to_string(nonce))};
            if (hash.substr(0, 10).find("000") != std::string::npos)
            {
                get_wallet(w) += 6.25;
                nonce_finded = true;
                break;
            }
        }
    }

    pending_trxs.clear();
    return nonce;
}
