#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <string>
#include <vector>

struct OrderNode {
    std::string customerName;
    long totalPayment;
    std::string orderDetail;
    bool isInstant;
    std::string destinationHub; // ◀ MENYIMPAN LOKASI TUJUAN
    OrderNode* next;

    OrderNode(std::string name, long total, std::string detail, bool instant = false, std::string dest = "") {
        customerName = name;
        totalPayment = total;
        orderDetail = detail;
        isInstant = instant;
        destinationHub = dest; // ◀ INISIALISASI LOKASI TUJUAN
        next = nullptr;
    }
};

class OrderQueue {
private:
    OrderNode* head;
    OrderNode* tail;

public:
    OrderQueue() {
        head = nullptr;
        tail = nullptr;
    }

    ~OrderQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() {
        return head == nullptr;
    }

    // Fungsi Mengatur antrean berdasarkan prioritas pengiriman dan tujuan
    void enqueueOrderPriority(std::string name, long total, std::string detail, bool isInstant, std::string dest) {
        OrderNode* newNode = new OrderNode(name, total, detail, isInstant, dest);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            return;
        }

        if (isInstant) {
            if (!head->isInstant) {
                newNode->next = head;
                head = newNode;
            }
            else {
                OrderNode* curr = head;
                while (curr->next != nullptr && curr->next->isInstant) {
                    curr = curr->next;
                }
                newNode->next = curr->next;
                curr->next = newNode;
                if (newNode->next == nullptr) {
                    tail = newNode;
                }
            }
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void dequeue() {
        if (isEmpty()) return;
        OrderNode* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
    }

    std::string peekFrontDetail() {
        if (isEmpty()) return "";
        return head->orderDetail;
    }

    // Fungsi untuk mengambil data tujuan Hub dari antrean teratas
    std::string peekFrontDestination() {
        if (isEmpty()) return "";
        return head->destinationHub;
    }

    std::vector<std::string> getQueueSummary() {
        std::vector<std::string> summary;
        OrderNode* curr = head;
        int no = 1;
        while (curr != nullptr) {
            std::string tipe = curr->isInstant ? " [⚡ INSTAN]" : " [📦 REGULER]";
            summary.push_back(std::to_string(no) + ". " + curr->customerName + tipe +
                              " 📍 Tujuan: " + curr->destinationHub + " - Total: Rp " + std::to_string(curr->totalPayment));
            curr = curr->next;
            no++;
        }
        return summary;
    }
};

#endif // ORDER_QUEUE_H