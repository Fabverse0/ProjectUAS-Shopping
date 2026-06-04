#ifndef ORDER_H
#define ORDER_H

#include <string>

/**
 * @brief Order model representing a completed purchase.
 */
class Order {
public:
    int orderId;
    std::string namaPemesan;
    std::string kotaTujuan;
    double totalHarga;
    std::string status;

    /**
     * @brief Default constructor
     */
    Order();

    /**
     * @brief Parameterized constructor
     */
    Order(int id, std::string nama, std::string kota, double total);

    /**
     * @brief Update the order status
     */
    void updateStatus(std::string newStatus);

    /**
     * @brief Display order data to console
     */
    void tampilkan() const;
};

#endif // ORDER_H
