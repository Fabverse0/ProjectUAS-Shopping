// Guard preprocessor untuk mencegah include ganda pada file header
#ifndef ORDER_QUEUE_H
// Definisikan simbol ORDER_QUEUE_H sebagai tanda file sudah di-include
#define ORDER_QUEUE_H

// Include library string untuk tipe data std::string (menyimpan nama pelanggan, detail order, hub tujuan)
#include <string>
// Include library vector untuk tipe data std::vector (menyimpan kumpulan data dinamis)
#include <vector>

// Definisikan struct OrderNode untuk mewakili satu pesanan dalam queue
struct OrderNode {
    // Menyimpan nama pelanggan yang melakukan pemesanan
    std::string customerName;
    // Menyimpan total biaya pembayaran untuk pesanan ini
    long totalPayment;
    // Menyimpan detail pesanan (informasi produk, jumlah, dll dalam format string)
    std::string orderDetail;
    // Flag boolean: true jika pesanan jenis Instan (prioritas tinggi), false jika Regular
    bool isInstant;
    // Menyimpan lokasi hub tujuan pengiriman (misal: "Hub Jakarta", "Hub Merauke")
    std::string destinationHub; // ◀ MENYIMPAN LOKASI TUJUAN
    // Pointer ke node berikutnya dalam queue (linked list)
    OrderNode* next;

    // Konstruktor untuk membuat node pesanan baru dengan parameter yang diberikan
    OrderNode(std::string name, long total, std::string detail, bool instant = false, std::string dest = "") {
        // Inisialisasi member customerName dengan nama pelanggan
        customerName = name;
        // Inisialisasi member totalPayment dengan total pembayaran
        totalPayment = total;
        // Inisialisasi member orderDetail dengan detail pesanan
        orderDetail = detail;
        // Inisialisasi member isInstant dengan status jenis pesanan (default=false/Regular)
        isInstant = instant;
        // Inisialisasi member destinationHub dengan hub tujuan pengiriman ◀ INISIALISASI LOKASI TUJUAN
        destinationHub = dest; // ◀ INISIALISASI LOKASI TUJUAN
        // Inisialisasi member next dengan nullptr (belum ada node berikutnya)
        next = nullptr;
    }
};

// Definisikan class OrderQueue untuk merepresentasikan antrian pesanan
class OrderQueue {
private:
    // Pointer ke node pertama (depan) dalam queue (yang akan diproses terlebih dahulu)
    OrderNode* head;
    // Pointer ke node terakhir (belakang) dalam queue (tempat pesanan baru ditambahkan)
    OrderNode* tail;

public:
    // Konstruktor: inisialisasi queue kosong
    OrderQueue() {
        // Set head dengan nullptr karena queue masih kosong
        head = nullptr;
        // Set tail dengan nullptr karena queue masih kosong
        tail = nullptr;
    }

    // Destruktor: hapus semua node pesanan saat object dihancurkan
    ~OrderQueue() {
        // Selama queue tidak kosong, lakukan dequeue hingga semua node terhapus
        while (!isEmpty()) {
            // Panggil fungsi dequeue untuk menghapus node dari depan queue
            dequeue();
        }
    }

    // Fungsi untuk mengecek apakah queue kosong (tidak ada pesanan dalam antrian)
    bool isEmpty() {
        // Kembalikan true jika head adalah nullptr (kosong), false jika ada pesanan
        return head == nullptr;
    }

    // Fungsi untuk menambahkan pesanan ke queue dengan sistem prioritas
    void enqueueOrderPriority(std::string name, long total, std::string detail, bool isInstant, std::string dest) {
        // Buat node pesanan baru dengan parameter yang diberikan
        OrderNode* newNode = new OrderNode(name, total, detail, isInstant, dest);

        // Jika queue kosong (head == nullptr), maka node baru adalah satu-satunya node
        if (head == nullptr) {
            // Set head menunjuk ke node baru (menjadi pesanan pertama)
            head = newNode;
            // Set tail menunjuk ke node baru (menjadi pesanan terakhir juga)
            tail = newNode;
            // Keluar dari fungsi, pesanan sudah ditambahkan
            return;
        }

        // Jika pesanan baru adalah Instan (prioritas tinggi)
        if (isInstant) {
            // Cek apakah head (pesanan paling depan) bukan Instan
            if (!head->isInstant) {
                // Jika ya, sisipkan node baru di depan head (menjadi head baru)
                newNode->next = head;
                // Update head untuk menunjuk ke node baru
                head = newNode;
            }
            // Jika head adalah Instan
            else {
                // Traverse queue untuk menemukan posisi insertion yang tepat
                OrderNode* curr = head;
                // Selama node berikutnya ada dan merupakan Instan juga
                while (curr->next != nullptr && curr->next->isInstant) {
                    // Geser pointer curr ke node berikutnya
                    curr = curr->next;
                }
                // Setelah loop, node baru akan disisipkan setelah curr
                newNode->next = curr->next;
                curr->next = newNode;
                // Jika node baru menjadi terakhir dalam queue
                if (newNode->next == nullptr) {
                    // Update tail untuk menunjuk ke node baru
                    tail = newNode;
                }
            }
        } 
        // Jika pesanan baru adalah Regular (prioritas normal/rendah)
        else {
            // Tambahkan pesanan Regular di akhir queue (tidak ada prioritas)
            tail->next = newNode;
            // Update tail untuk menunjuk ke node baru
            tail = newNode;
        }
    }

    // Fungsi untuk menghapus pesanan dari depan queue (FIFO - First In First Out)
    void dequeue() {
        // Jika queue kosong, tidak ada yang bisa dihapus
        if (isEmpty()) return;
        // Simpan pointer ke node yang akan dihapus (head saat ini)
        OrderNode* temp = head;
        // Update head untuk menunjuk ke node berikutnya
        head = head->next;
        // Jika head menjadi nullptr setelah update, queue kini kosong
        if (head == nullptr) {
            // Set tail juga nullptr untuk konsistensi (queue kosong)
            tail = nullptr;
        }
        // Hapus node yang sudah di-dequeue dari memori
        delete temp;
    }

    // Fungsi untuk melihat detail pesanan yang paling depan dalam queue
    std::string peekFrontDetail() {
        // Jika queue kosong, kembalikan string kosong ""
        if (isEmpty()) return "";
        // Kembalikan orderDetail dari node depan (head)
        return head->orderDetail;
    }

    // Fungsi untuk mengambil data hub tujuan dari pesanan paling depan dalam queue
    std::string peekFrontDestination() {
        // Jika queue kosong, kembalikan string kosong ""
        if (isEmpty()) return "";
        // Kembalikan destinationHub dari node depan (head) ◀ KEMBALIKAN HUB TUJUAN
        return head->destinationHub;
    }

    // Fungsi untuk mendapatkan ringkasan/summary seluruh pesanan dalam queue
    std::vector<std::string> getQueueSummary() {
        // Buat vector kosong untuk menyimpan informasi setiap pesanan
        std::vector<std::string> summary;
        // Inisialisasi pointer curr untuk traverse dari head (depan queue)
        OrderNode* curr = head;
        // Inisialisasi nomor urut pesanan dimulai dari 1
        int no = 1;
        // Selama curr tidak nullptr (masih ada pesanan dalam queue)
        while (curr != nullptr) {
            // Buat string tipe pesanan: "[⚡ INSTAN]" jika Instan, "[📦 REGULER]" jika Regular
            std::string tipe = curr->isInstant ? " [⚡ INSTAN]" : " [📦 REGULER]";
            // Buat string ringkasan: nomor, nama pelanggan, tipe, hub tujuan, dan total pembayaran
            summary.push_back(std::to_string(no) + ". " + curr->customerName + tipe +
                              " 📍 Tujuan: " + curr->destinationHub + " - Total: Rp " + std::to_string(curr->totalPayment));
            // Geser pointer curr ke node berikutnya
            curr = curr->next;
            // Increment nomor urut pesanan untuk pesanan berikutnya
            no++;
        }
        // Kembalikan vector berisi ringkasan semua pesanan dalam queue
        return summary;
    }
};

// Akhir guard preprocessor: menutup pencegahan include ganda
#endif // ORDER_QUEUE_H