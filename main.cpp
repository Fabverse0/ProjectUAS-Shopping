// Include header MainWindow yang berisi definisi class aplikasi utama
#include "mainwindow.h"
// Include header QApplication yang merupakan objek aplikasi utama Qt
#include <QApplication>

// Fungsi main: entry point program aplikasi desktop Qt
int main(int argc, char *argv[]) {
    // Buat objek QApplication untuk mengelola control flow dan setting GUI aplikasi
    QApplication a(argc, argv);
    // Buat objek MainWindow (jendela utama aplikasi e-commerce)
    MainWindow w;
    // Tampilkan jendela aplikasi ke layar pengguna
    w.show();
    // Jalankan event loop aplikasi (menunggu input user dan memproses events)
    return a.exec();
}