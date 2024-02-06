#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "user.h"
#include "product.h"
#include <map>
#include <set>
#include <vector>

class MyDataStore : public DataStore {
public:
    MyDataStore();
    virtual ~MyDataStore();

    void addProduct(Product* p) override;
    void addUser(User* u) override;
    std::vector<Product*> search(std::vector<std::string>& terms, int type) override;
    void dump(std::ostream& ofile) override;

    // New functionalities
    void addToCart(const std::string& username, Product* p);
    void viewCart(const std::string& username);
    void buyCart(const std::string& username);

private:
    std::map<std::string, std::set<Product*>> keywordToProductMap;
    std::map<std::string, User*> usernameToUserMap;
    std::map<std::string, std::vector<Product*>> cart; // Maps usernames to their carts (a list of products)
};

#endif
