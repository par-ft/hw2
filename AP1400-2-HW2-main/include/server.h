#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

/*****************************************************************************************************/
extern std::vector<std::string> pending_trxs;
/*****************************************************************************************************/

/*****************************************************************************************************/
// forward declaration
class Client;
/*****************************************************************************************************/

class Server
{
public:
    // default constructor
    Server() = default;

    // If this id already exists, the server should add a
    // random 4 digit number at the end of it automatically
    // each client should be assigned with 5 coins at the begining
    std::shared_ptr<Client> add_client(std::string id);

    // Using this function you can get a pointer to a Client using its id
    std::shared_ptr<Client> get_client(std::string id) const;

    // Using this function will return the wallet
    // value of the client with username id
    double &get_wallet(std::string id) const;

    /*****************************************************************************************************/
    static bool parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value);
    /*****************************************************************************************************/

    bool add_pending_trx(std::string trx, std::string signature) const;

    size_t mine();

    /*****************************************************************************************************/
    friend void show_wallets(const Server &server);
    /*****************************************************************************************************/

private:
    /*****************************************************************************************************/
    // This member variable will map each client to its wallet
    mutable std::map<std::shared_ptr<Client>, double> clients;
    /*****************************************************************************************************/
};

#endif // SERVER_H
