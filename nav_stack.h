#ifndef NAV_STACK_H
#define NAV_STACK_H

// =========================================================
// STRUKTUR DATA: STACK (Riwayat Navigasi Halaman)
// Implementasi berbasis Linked List (bukan array)
// Push  = saat user pindah halaman
// Pop   = saat user menekan tombol Back
// Peek  = melihat halaman sebelumnya tanpa keluar dari stack
// =========================================================

struct NavNode {
    int pageIndex;       // Index halaman di stackedWidget
    std::string pageLabel; // Label nama halaman (untuk display)
    NavNode* next;

    NavNode(int idx, std::string label)
        : pageIndex(idx), pageLabel(label), next(nullptr) {}
};

class NavStack {
private:
    NavNode* top;
    int stackSize;

public:
    NavStack() : top(nullptr), stackSize(0) {}

    ~NavStack() {
        while (!isEmpty()) pop();
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int size() const {
        return stackSize;
    }

    // Push: catat halaman saat ini sebelum berpindah
    void push(int pageIndex, const std::string& label) {
        NavNode* newNode = new NavNode(pageIndex, label);
        newNode->next = top;
        top = newNode;
        stackSize++;
    }

    // Pop: ambil halaman terakhir yang dikunjungi (untuk Back)
    int pop() {
        if (isEmpty()) return -1;
        NavNode* temp = top;
        int idx = temp->pageIndex;
        top = top->next;
        delete temp;
        stackSize--;
        return idx;
    }

    // Peek: lihat halaman teratas tanpa pop
    int peek() const {
        if (isEmpty()) return -1;
        return top->pageIndex;
    }

    std::string peekLabel() const {
        if (isEmpty()) return "";
        return top->pageLabel;
    }

    // Untuk debugging: tampilkan seluruh isi stack
    std::vector<std::string> getHistoryList() const {
        std::vector<std::string> history;
        NavNode* curr = top;
        while (curr != nullptr) {
            history.push_back(curr->pageLabel + " [idx:" + std::to_string(curr->pageIndex) + "]");
            curr = curr->next;
        }
        return history;
    }

    // Reset stack (misal saat logout)
    void clear() {
        while (!isEmpty()) pop();
    }
};

#endif // NAV_STACK_H