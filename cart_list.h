#ifndef CART_LIST_H
#define CART_LIST_H

#include <string>
#include <vector>

// STRUKTUR DATA UNTUK KERANJANG BELANJA (SINGLE LINKED LIST)
struct CartNode {
    int id;
    std::string name;
    long price;
    std::string origin; // Menyimpan lokasi asal barang yang dimasukkan keranjang
    CartNode* next;
};

class CartList {
private:
    CartNode* head;

public:
    CartList() : head(nullptr) {}

    bool isEmpty() { return head == nullptr; }

    void insertItem(int id, std::string name, long price, std::string origin) {
        CartNode* newNode = new CartNode{id, name, price, origin, nullptr};
        if (!head) {
            head = newNode;
        } else {
            CartNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    void deleteNodeAt(int index) {
        if (!head) return;
        if (index == 0) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        CartNode* temp = head;
        for (int i = 0; temp != nullptr && i < index - 1; i++) {
            temp = temp->next;
        }
        if (!temp || !temp->next) return;
        CartNode* nextNode = temp->next->next;
        delete temp->next;
        temp->next = nextNode;
    }

    void clearList() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    long calculateTotal() {
        long total = 0;
        CartNode* temp = head;
        while (temp) {
            total += temp->price;
            temp = temp->next;
        }
        return total;
    }

    std::vector<std::string> getAllItemsString() {
        std::vector<std::string> res;
        CartNode* temp = head;
        while (temp) {
            res.push_back(temp->name + " (Rp " + std::to_string(temp->price) + ") [Asal: " + temp->origin + "]");
            temp = temp->next;
        }
        return res;
    }

    // HELPER METHODS UNTUK KEBUTUHAN SPLIT-ORDER DI MAINWINDOW
    std::vector<std::string> getAllOrigins() {
        std::vector<std::string> res;
        CartNode* temp = head;
        while (temp) {
            res.push_back(temp->origin);
            temp = temp->next;
        }
        return res;
    }

    std::vector<std::string> getAllNames() {
        std::vector<std::string> res;
        CartNode* temp = head;
        while (temp) {
            res.push_back(temp->name);
            temp = temp->next;
        }
        return res;
    }

    std::vector<long> getAllPrices() {
        std::vector<long> res;
        CartNode* temp = head;
        while (temp) {
            res.push_back(temp->price);
            temp = temp->next;
        }
        return res;
    }
};

#endif // CART_LIST_H