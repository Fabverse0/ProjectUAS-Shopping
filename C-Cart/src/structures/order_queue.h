#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "../models/order.h"

/**
 * @brief Node for OrderQueue linked list
 */
struct OrderNode {
    Order data;
    OrderNode* next;
    OrderNode(Order o) : data(o), next(nullptr) {}
};

/**
 * @brief Queue structure to manage the order processing sequence.
 * Manual implementation using linked list (FIFO).
 */
class OrderQueue {
private:
    OrderNode* frontNode;
    OrderNode* rearNode;
    int size;

public:
    OrderQueue();
    ~OrderQueue();

    /**
     * @brief Add order to the back of the queue
     */
    void enqueue(Order order);

    /**
     * @brief Remove order from the front of the queue
     */
    void dequeue();

    /**
     * @brief Get the order at the front of the queue
     * @return Pointer to front order, or nullptr if empty
     */
    Order* front();

    /**
     * @brief Check if queue is empty
     */
    bool isEmpty() const;

    /**
     * @brief Get current number of orders in queue
     */
    int getSize() const;

    /**
     * @brief Display all orders in queue
     */
    void displayQueue() const;

    /**
     * @brief Clear all orders from queue
     */
    void clear();
};

#endif // ORDER_QUEUE_H
