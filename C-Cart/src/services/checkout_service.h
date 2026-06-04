#ifndef CHECKOUT_SERVICE_H
#define CHECKOUT_SERVICE_H

#include "../structures/order_queue.h"
#include <string>

/**
 * @brief Service class to handle the checkout process and order queuing.
 */
class CheckoutService {
private:
    OrderQueue queue;
    int nextOrderId;

public:
    CheckoutService();

    /**
     * @brief Create a new order and add it to the queue
     */
    void createOrder(std::string namaPemesan, std::string kotaTujuan, double totalHarga);

    /**
     * @brief Process the front order in the queue
     * Changes status: Menunggu -> Diproses -> Selesai (then dequeued)
     */
    void processNextOrder();

    /**
     * @brief Get access to the order queue
     */
    OrderQueue& getQueue();

    /**
     * @brief Check if the order queue is empty
     */
    bool isQueueEmpty() const;

    /**
     * @brief Display all current orders in the queue
     */
    void displayOrders() const;
};

#endif // CHECKOUT_SERVICE_H
