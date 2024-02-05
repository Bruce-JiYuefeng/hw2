#include "mydatastore.h"
#include "util.h"
#include <iostream>
#include <algorithm>

MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
    // Cleanup 
    for (auto iter : usernameToUserMap) {
        delete iter.second;
    }
    for (auto iter : keywordToProductMap) {
        for (auto prod : iter.second) {
            delete prod;
        }
    }
}

void MyDataStore::addProduct(Product* p) {
    std::set<std::string> keywords = p->keywords();
    for (const std::string& key : keywords) {
        keywordToProductMap[key].insert(p);
    }
}

void MyDataStore::addUser(User* u) {
    usernameToUserMap[u->getName()] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    std::set<Product*> results;
    if (type == 0) { // AND search
        for (auto& term : terms) {
            std::set<Product*> termResults = keywordToProductMap[term];
            if (results.empty()) {
                results = termResults;
            } else {
                results = setIntersection(results, termResults);
            }
        }
    } else if (type == 1) { // OR search
        for (auto& term : terms) {
            std::set<Product*> termResults = keywordToProductMap[term];
            results = setUnion(results, termResults);
        }
    }
    return std::vector<Product*>(results.begin(), results.end());
}

void MyDataStore::dump(std::ostream& ofile) {
    ofile << "<products>" << std::endl;
    for (const auto& pair : keywordToProductMap) {
        for (const auto& prod : pair.second) {
            prod->dump(ofile);
        }
    }
    ofile << "</products>" << std::endl;

    ofile << "<users>" << std::endl;
    for (const auto& pair : usernameToUserMap) {
        pair.second->dump(ofile);
    }
    ofile << "</users>" << std::endl;
}

void MyDataStore::addToCart(const std::string& username, Product* p) {
    if(usernameToUserMap.find(username) != usernameToUserMap.end()) {
        userCarts[username].push_back(p);
    }
    else {
        std::cerr << "User not found." << std::endl;
    }
}

void MyDataStore::viewCart(const std::string& username) {
    if(userCarts.find(username) == userCarts.end()) {
        std::cerr << "User not found or cart is empty." << std::endl;
        return;
    }

    std::cout << "User Cart [" << username << "]:" << std::endl;
    int count = 1;
    for(auto* product : userCarts[username]) {
        std::cout << count << ". " << product->displayString() << std::endl;
        count++;
    }
}

void MyDataStore::buyCart(const std::string& username) {
    if(userCarts.find(username) == userCarts.end()) {
        std::cerr << "User not found or cart is empty." << std::endl;
        return;
    }

    auto& cart = userCarts[username];
    auto it = cart.begin();
    while(it != cart.end()) {
        Product* product = *it;
        if(product->getQty() > 0 && usernameToUserMap[username]->getBalance() >= product->getPrice()) {
            usernameToUserMap[username]->deductAmount(product->getPrice());
            product->subtractQty(1);
            it = cart.erase(it); // Remove the item from the cart once purchased
        }
        else {
            ++it;
        }
    }
}