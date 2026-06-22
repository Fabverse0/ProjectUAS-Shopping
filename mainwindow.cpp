#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "nav_stack.h"
#include "binary_search.h"
#include "bst.h"
#include "avl_tree.h"
#include "hash_table.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QRadioButton>
#include <QDateTime>
#include <map>

// ====================================================================
// FILE: mainwindow.cpp
// KETERANGAN: Implementasi class MainWindow - logic aplikasi shopping
//             Mencakup UI initialization, event handlers, dan business logic
// ====================================================================

#include "mainwindow.h"           // Header file MainWindow class
#include "./ui_mainwindow.h"      // Auto-generated UI class dari Qt Designer
#include "nav_stack.h"            // Stack untuk riwayat navigasi (No.4)
#include "binary_search.h"        // Binary Search untuk pencarian produk (No.6)
#include "bst.h"                  // BST untuk sorting harga katalog (No.7)
#include "avl_tree.h"             // AVL Tree untuk indexed produk by ID (No.8)
#include "hash_table.h"           // Hash Table untuk O(1) lookup (No.9)
#include <QInputDialog>           // Dialog untuk input dari user
#include <QMessageBox>            // Dialog untuk pesan info/warning/error
#include <QDialog>                // Class dasar untuk custom dialog
#include <QTableWidget>           // Widget tabel untuk menampilkan data
#include <QHeaderView>            // Class untuk manage header di tabel
#include <QRadioButton>           // Widget radio button (pilihan tunggal)
#include <QDateTime>              // Class untuk handling date & time
#include <map>                    // Container map (key-value pair)

// ====================================================================
// STRUCT & HELPER: Kode Diskon Voucher
// ====================================================================
// Struktur data untuk menyimpan informasi voucher diskon
struct KodeDiskon {
    QString kode;              // Kode voucher (misal: "CCART50")
    int persen;                // Persentase diskon (0-100)
    QString deskripsi;         // Deskripsi promo untuk user
};

// Database static voucher yang valid dalam sistem
static const std::vector<KodeDiskon> daftarDiskon = {
    { "CCART50",   50, "Diskon 50% semua produk" },
    { "HEMAT20",   20, "Diskon 20% semua produk" },
    { "WELCOME10", 10, "Diskon 10% untuk pelanggan baru" },
    { "FLASH30",   30, "Flash sale diskon 30%" },
    { "GRATIS",   100, "Gratis 100% (produk + ongkir)" },
    };

// Fungsi helper: cari kode diskon dalam daftar
// Return: pair<bool, pointer> — bool = ditemukan, pointer = data voucher
static std::pair<bool,const KodeDiskon*> cariKodeDiskon(const QString& kode) {
    for (auto& d : daftarDiskon)
        if (d.kode == kode.trimmed().toUpper()) return { true, &d };  // Ditemukan
    return { false, nullptr };  // Tidak ditemukan
}

// ====================================================================
// FUNGSI HELPER: Hitung Waktu Pengiriman Dua Arah (Simetris)
// KETERANGAN: Waktu pengiriman sama untuk rute A→B dan B→A
//             Untuk simulasi logistik realistis marketplace
// ====================================================================
int hitungWaktuDuaArah(const QString& kotaA, const QString& kotaB) {
    QString a = kotaA.trimmed();
    QString b = kotaB.trimmed();

    // Sama hub = 12 jam (proses & packing di gudang — seperti marketplace nyata)
    if (a == b) return 12;

    // Jakarta <-> Bekasi : 1 hari (24 jam) — kota tetangga, ada proses sortir
    if ((a=="Hub Jakarta"   && b=="Sub-Hub Bekasi") || (a=="Sub-Hub Bekasi" && b=="Hub Jakarta"))   return 24;

    // Jakarta <-> Surabaya : 3 hari (72 jam) — antar kota besar Jawa
    if ((a=="Hub Jakarta"   && b=="Hub Surabaya")   || (a=="Hub Surabaya"   && b=="Hub Jakarta"))   return 72;

    // Surabaya <-> Merauke : 7 hari (168 jam) — kargo laut/udara ke Papua
    if ((a=="Hub Surabaya"  && b=="Hub Merauke")    || (a=="Hub Merauke"    && b=="Hub Surabaya"))  return 168;

    // Transit: Jakarta <-> Merauke (via Surabaya) = 72 + 168 = 240 jam (10 hari)
    if ((a=="Hub Jakarta"   && b=="Hub Merauke")    || (a=="Hub Merauke"    && b=="Hub Jakarta"))   return 240;

    // Transit: Bekasi <-> Surabaya (via Jakarta) = 24 + 72 = 96 jam (4 hari)
    if ((a=="Sub-Hub Bekasi"&& b=="Hub Surabaya")   || (a=="Hub Surabaya"   && b=="Sub-Hub Bekasi"))return 96;

    // Transit: Bekasi <-> Merauke (via Jakarta + Surabaya) = 24 + 72 + 168 = 264 jam (11 hari)
    if ((a=="Sub-Hub Bekasi"&& b=="Hub Merauke")    || (a=="Hub Merauke"    && b=="Sub-Hub Bekasi"))return 264;

    return 72; // fallback default untuk rute tidak dikenali
}

// ====================================================================
// FUNGSI HELPER: Dapatkan Urutan Hub dalam Jalur Pengiriman
// KETERANGAN: Menghitung urutan hub yang dilalui dari asal ke tujuan
//             (misal Jakarta→Surabaya→Merauke)
// ====================================================================
static std::vector<QString> getJalurHub(const QString& asal, const QString& tujuan) {
    // Definisi urutan jaringan logistik: Bekasi - Jakarta - Surabaya - Merauke
    const std::vector<QString> rantai = {"Sub-Hub Bekasi","Hub Jakarta","Hub Surabaya","Hub Merauke"};
    int idxA = -1, idxB = -1;

    // Cari index asal dan tujuan dalam rantai hub
    for (int i = 0; i < (int)rantai.size(); ++i) {
        if (rantai[i] == asal)   idxA = i;
        if (rantai[i] == tujuan) idxB = i;
    }

    std::vector<QString> jalur;
    // Jika asal/tujuan tidak ada dalam jaringan, return sebagai fallback
    if (idxA < 0 || idxB < 0) { jalur.push_back(asal); jalur.push_back(tujuan); return jalur; }

    // Tentukan arah: 1 untuk maju (asal→tujuan), -1 untuk mundur
    int step = (idxA <= idxB) ? 1 : -1;

    // Build jalur dari asal ke tujuan (inclusive)
    for (int i = idxA; i != idxB + step; i += step) jalur.push_back(rantai[i]);
    return jalur;
}

// ====================================================================
// FUNGSI HELPER: Waktu Tempuh Antara Dua Hub Bertetangga
// KETERANGAN: Hanya untuk hub yang berdekatan langsung dalam jaringan
//             Jika tidak bertetangga, return 0 (gunakan getJalurHub untuk transit)
// ====================================================================
static int waktuAntarHub(const QString& a, const QString& b) {
    if (a == b) return 0;  // Sudah di hub ini, tidak perlu waktu tempuh
    if ((a=="Sub-Hub Bekasi"&&b=="Hub Jakarta")||(a=="Hub Jakarta"&&b=="Sub-Hub Bekasi")) return 24;
    if ((a=="Hub Jakarta"&&b=="Hub Surabaya")||(a=="Hub Surabaya"&&b=="Hub Jakarta"))     return 72;
    if ((a=="Hub Surabaya"&&b=="Hub Merauke")||(a=="Hub Merauke"&&b=="Hub Surabaya"))     return 168;
    return 0;  // Hub tidak bertetangga
}


// ====================================================================
// KONSTRUKTOR: Inisialisasi UI dan Business Logic
// ====================================================================
// Constructor dipanggil sekali saat MainWindow object dibuat
// Tanggung jawab:
// 1. Setup UI dari designer file
// 2. Create semua widget dinamis (tidak dibuat di designer)
// 3. Setup layout dan styling
// 4. Koneksi signal-slot untuk event handling
// 5. Inisialisasi data struktur (katalog, tree, hash table, dll)
// 6. Setup timer untuk promo banner rotation
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  // Inherit dari QMainWindow
    , ui(new Ui::MainWindow)  // Initialize UI object dari auto-generated file
{
    ui->setupUi(this);  // Setup semua widget dari designer ke window
    kategoriAktif = "Elektronik";  // Default kategori produk ditampilkan adalah Elektronik

    // =========================================================
    // ROOT STACK: Switch antara Login Page (index 0) dan Shell Utama (index 1)
    // =========================================================
    // QStackedWidget memungkinkan multiple pages dalam satu space visual
    // index 0 = halaman login, index 1 = aplikasi main setelah login
    rootStack = new QStackedWidget(this);
    setCentralWidget(rootStack);  // Set rootStack sebagai central widget window

    // =========================================================
    // PAGE LOGIN — Halaman login awal aplikasi
    // =========================================================
    // Setup halaman login dengan styling dan layout
    pageLogin = new QWidget();
    QVBoxLayout *loginOuter = new QVBoxLayout(pageLogin);
    loginOuter->setAlignment(Qt::AlignCenter);

    QFrame *loginCard = new QFrame(pageLogin);
    loginCard->setFixedWidth(340);
    loginCard->setStyleSheet("QFrame { background:#ffffff; border-radius:12px; border:1px solid #dce1e7; }");
    QVBoxLayout *loginLayout = new QVBoxLayout(loginCard);
    loginLayout->setSpacing(12);
    loginLayout->setContentsMargins(28,28,28,28);

    lblLoginTitle = new QLabel("🛒 CSHOP LOGIN", loginCard);
    lblLoginTitle->setAlignment(Qt::AlignCenter);
    lblLoginTitle->setStyleSheet("font-size:20px; font-weight:bold; color:#2c3e50; border:none;");

    QLabel *lblSub = new QLabel("Masuk sebagai Pelanggan atau Admin", loginCard);
    lblSub->setAlignment(Qt::AlignCenter);
    lblSub->setStyleSheet("color:#7f8c8d; font-size:12px; border:none;");

    lblLoginUser = new QLabel("Username:", loginCard);
    lblLoginUser->setStyleSheet("border:none; font-weight:bold;");
    txtLoginUser = new QLineEdit(loginCard);
    txtLoginUser->setPlaceholderText("contoh: pelanggan1  atau  admin");
    txtLoginUser->setStyleSheet("padding:8px; border:1px solid #bdc3c7; border-radius:6px;");

    lblLoginPass = new QLabel("Password:", loginCard);
    lblLoginPass->setStyleSheet("border:none; font-weight:bold;");
    txtLoginPass = new QLineEdit(loginCard);
    txtLoginPass->setEchoMode(QLineEdit::Password);
    txtLoginPass->setPlaceholderText("Masukkan password");
    txtLoginPass->setStyleSheet("padding:8px; border:1px solid #bdc3c7; border-radius:6px;");

    btnLoginMasuk = new QPushButton("🔐 Masuk", loginCard);
    btnLoginMasuk->setStyleSheet("background:#2980b9; color:white; font-weight:bold; padding:10px; border-radius:6px; border:none;");

    lblLoginInfo = new QLabel(
        "<b>Akun tersedia:</b><br>"
        "👤 pelanggan1 / pass123<br>"
        "👤 pelanggan2 / pass456<br>"
        "🔧 admin / admin123",
        loginCard);
    lblLoginInfo->setAlignment(Qt::AlignCenter);
    lblLoginInfo->setStyleSheet("color:#7f8c8d; font-size:11px; background:#f8f9fa; border-radius:6px; padding:8px; border:1px solid #ecf0f1;");

    loginLayout->addWidget(lblLoginTitle);
    loginLayout->addWidget(lblSub);
    loginLayout->addSpacing(6);
    loginLayout->addWidget(lblLoginUser);
    loginLayout->addWidget(txtLoginUser);
    loginLayout->addWidget(lblLoginPass);
    loginLayout->addWidget(txtLoginPass);
    loginLayout->addWidget(btnLoginMasuk);
    loginLayout->addSpacing(4);
    loginLayout->addWidget(lblLoginInfo);

    loginOuter->addWidget(loginCard);
    rootStack->addWidget(pageLogin);  // index 0 di rootStack

    // =========================================================
    // SHELL UTAMA (post-login)
    // =========================================================
    shellWidget = new QWidget();
    shellLayout = new QHBoxLayout(shellWidget);
    shellLayout->setSpacing(0);
    shellLayout->setContentsMargins(0,0,0,0);

    // --- SIDEBAR ---
    sidebarFrame = new QFrame(shellWidget);
    sidebarFrame->setFixedWidth(200);
    sidebarFrame->setStyleSheet("QFrame { background:#2c3e50; } QPushButton { text-align:left; padding:10px 14px; border:none; color:#ecf0f1; border-radius:0; } QPushButton:hover { background:#34495e; }");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setSpacing(2);
    sidebarLayout->setContentsMargins(0,10,0,10);

    lblMenu = new QLabel("🛒 CSHOP", sidebarFrame);
    lblMenu->setStyleSheet("font-weight:bold; font-size:16px; color:#ecf0f1; padding:8px 14px 4px 14px;");
    lblWelcome = new QLabel("", sidebarFrame);
    lblWelcome->setStyleSheet("font-size:11px; color:#95a5a6; padding:0 14px 12px 14px;");
    lblWelcome->setWordWrap(true);

    btnKatalog       = new QPushButton("📦 Katalog Produk",     sidebarFrame);
    btnKeranjang     = new QPushButton("🛒 Keranjang Belanja",   sidebarFrame);
    btnAntrean       = new QPushButton("📋 Antrean Pesanan",     sidebarFrame);
    btnRute          = new QPushButton("🗺 Jaringan Rute",       sidebarFrame);
    btnAdminDashboard= new QPushButton("🖥 Dashboard Admin",     sidebarFrame);
    btnManajemenProduk=new QPushButton("✏️ Kelola Produk",       sidebarFrame);
    btnHashLookup    = new QPushButton("🔎 Quick Lookup (Hash)", sidebarFrame);
    btnTrackPelanggan= new QPushButton("📡 Lacak Paket Saya",    sidebarFrame);

    btnLogout = new QPushButton("🚪 Logout", sidebarFrame);
    btnLogout->setStyleSheet("text-align:left; padding:10px 14px; border:none; color:#e74c3c; border-radius:0;");
    btnLogout->setObjectName("btnLogout");

    // Tombol Back (Stack Navigasi)
    btnBack = new QPushButton("⬅ Back", sidebarFrame);
    btnBack->setStyleSheet("text-align:left; padding:10px 14px; border:none; color:#f39c12; border-radius:0;");
    btnBack->setEnabled(false); // disabled sampai ada riwayat

    sidebarLayout->addWidget(lblMenu);
    sidebarLayout->addWidget(lblWelcome);
    sidebarLayout->addWidget(btnKatalog);
    sidebarLayout->addWidget(btnKeranjang);
    sidebarLayout->addWidget(btnAntrean);
    sidebarLayout->addWidget(btnRute);
    sidebarLayout->addWidget(btnAdminDashboard);
    sidebarLayout->addWidget(btnManajemenProduk);
    sidebarLayout->addWidget(btnHashLookup);
    sidebarLayout->addWidget(btnTrackPelanggan);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnBack);
    sidebarLayout->addWidget(btnLogout);

    // --- STACKED PAGES ---
    stackedWidget = new QStackedWidget(shellWidget);

    // ===================== PAGE 0: KATALOG =====================
    pageKatalog = new QWidget();
    QVBoxLayout *katalogLayout = new QVBoxLayout(pageKatalog);

    // CIRCULAR LINKED LIST: isi node-node promo kode diskon
    promoBanner.addPromo("🏷 Kode: CCART50   - Diskon 50% semua produk di keranjang!");
    promoBanner.addPromo("💸 Kode: HEMAT20   - Hemat 20% untuk semua produk pilihan!");
    promoBanner.addPromo("🎉 Kode: WELCOME10 - Diskon 10% khusus pelanggan baru!");
    promoBanner.addPromo("⚡ Kode: FLASH30   - Flash sale! Diskon 30% hari ini saja!");
    promoBanner.addPromo("🎁 Kode: GRATIS    - Produk + ongkir GRATIS 100%! Buruan!");
    // Setelah node terakhir, circular list otomatis kembali ke node pertama

    lblPromoBanner = new QLabel(QString::fromStdString(promoBanner.getTeks()), pageKatalog);
    lblPromoBanner->setStyleSheet("background:#ebf5fb; color:#2e86c1; font-weight:bold; padding:10px; border-radius:5px;");
    lblPromoBanner->setAlignment(Qt::AlignCenter);
    katalogLayout->addWidget(lblPromoBanner);

    // Timer auto-rotasi banner setiap 3 detik (Circular Linked List)
    promoTimer = new QTimer(this);
    connect(promoTimer, &QTimer::timeout, this, &MainWindow::rotasiPromoBanner);
    promoTimer->start(3000);

    QHBoxLayout *kategoriTabLayout = new QHBoxLayout();
    btnKatElektronik = new QPushButton("🔌 Elektronik", pageKatalog);
    btnKatPakaian    = new QPushButton("👕 Pakaian",    pageKatalog);
    btnKatPerabotan  = new QPushButton("🪑 Perabotan",  pageKatalog);
    btnKatElektronik->setStyleSheet("background:#34495e; color:white; font-weight:bold; min-height:25px;");
    btnKatPakaian->setStyleSheet("background:#ecf0f1; color:black; min-height:25px;");
    btnKatPerabotan->setStyleSheet("background:#ecf0f1; color:black; min-height:25px;");
    kategoriTabLayout->addWidget(btnKatElektronik);
    kategoriTabLayout->addWidget(btnKatPakaian);
    kategoriTabLayout->addWidget(btnKatPerabotan);
    katalogLayout->addLayout(kategoriTabLayout);

    groupProduk = new QGroupBox("Informasi Produk (Double Linked List)", pageKatalog);
    QFormLayout *formProdukLayout = new QFormLayout(groupProduk);
    lblProductIdVal       = new QLabel("-", groupProduk);
    lblProductNameVal     = new QLabel("Belum ada data", groupProduk);
    lblProductCategoryVal = new QLabel("-", groupProduk);
    lblProductPriceVal    = new QLabel("Rp 0", groupProduk);
    QLabel *lblOriginTitle = new QLabel("Dikirim Dari (Gudang Asal):", groupProduk);
    lblOriginTitle->setStyleSheet("font-weight:bold; color:#e67e22;");
    lblPotonganDiskon = new QLabel("-", groupProduk);
    lblPotonganDiskon->setStyleSheet("font-weight:bold; color:#e67e22;");
    formProdukLayout->addRow("ID Produk :",    lblProductIdVal);
    formProdukLayout->addRow("Nama Barang :",  lblProductNameVal);
    formProdukLayout->addRow("Kategori :",     lblProductCategoryVal);
    formProdukLayout->addRow("Harga Satuan :", lblProductPriceVal);
    formProdukLayout->addRow(lblOriginTitle,   lblPotonganDiskon);
    katalogLayout->addWidget(groupProduk);

    QHBoxLayout *navButtonLayout = new QHBoxLayout();
    btnPrevCatalog = new QPushButton("◀ Previous", pageKatalog);
    btnAddToCart   = new QPushButton("➕ Tambah ke Keranjang", pageKatalog);
    btnNextCatalog = new QPushButton("Next ▶", pageKatalog);
    btnAddToCart->setStyleSheet("background:#2ecc71; color:white; font-weight:bold; min-height:30px;");
    navButtonLayout->addWidget(btnPrevCatalog);
    navButtonLayout->addWidget(btnAddToCart);
    navButtonLayout->addWidget(btnNextCatalog);
    katalogLayout->addLayout(navButtonLayout);

    QHBoxLayout *algoButtonLayout = new QHBoxLayout();
    btnUrutHarga = new QPushButton("📊 Urut Harga (BST)", pageKatalog);
    btnSearchId  = new QPushButton("🔍 Cari Produk Global", pageKatalog);
    algoButtonLayout->addWidget(btnUrutHarga);
    algoButtonLayout->addWidget(btnSearchId);
    katalogLayout->addLayout(algoButtonLayout);
    katalogLayout->addStretch();
    stackedWidget->addWidget(pageKatalog);  // index 0

    // ===================== PAGE 1: KERANJANG =====================
    pageKeranjang = new QWidget();
    QVBoxLayout *keranjangLayout = new QVBoxLayout(pageKeranjang);
    lblCartTitle = new QLabel("🛒 Keranjang Belanja (Split-Order Multi-Toko)", pageKeranjang);
    lblCartTitle->setStyleSheet("font-size:16px; font-weight:bold; color:#2c3e50;");
    keranjangLayout->addWidget(lblCartTitle);

    listCart = new QListWidget(pageKeranjang);
    listCart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    keranjangLayout->addWidget(listCart);

    QHBoxLayout *diskonLayout = new QHBoxLayout();
    QLabel *lblKodeDiskon = new QLabel("Kode Promo:", pageKeranjang);
    txtKodeDiskon = new QLineEdit(pageKeranjang);
    txtKodeDiskon->setPlaceholderText("Contoh: CCART50");
    txtKodeDiskon->setMaximumWidth(140);
    btnPakaiDiskon = new QPushButton("Terapkan", pageKeranjang);
    diskonLayout->addWidget(lblKodeDiskon);
    diskonLayout->addWidget(txtKodeDiskon);
    diskonLayout->addWidget(btnPakaiDiskon);
    diskonLayout->addStretch();
    keranjangLayout->addLayout(diskonLayout);

    lblTotalBelanja = new QLabel("Total: Rp 0", pageKeranjang);
    lblTotalBelanja->setStyleSheet("font-size:14px; font-weight:bold; color:#e74c3c; margin:5px 0;");
    lblTotalBelanja->setAlignment(Qt::AlignRight);
    keranjangLayout->addWidget(lblTotalBelanja);

    QHBoxLayout *wilayahLayout = new QHBoxLayout();
    QLabel *lblWilayah = new QLabel("📍 Alamat Tujuan:", pageKeranjang);
    lblWilayah->setStyleSheet("font-weight:bold; color:#2c3e50;");
    comboWilayahHub = new QComboBox(pageKeranjang);
    comboWilayahHub->addItem("Hub Jakarta");
    comboWilayahHub->addItem("Sub-Hub Bekasi");
    comboWilayahHub->addItem("Hub Surabaya");
    comboWilayahHub->addItem("Hub Merauke");
    comboWilayahHub->setMaximumWidth(200);
    wilayahLayout->addWidget(lblWilayah);
    wilayahLayout->addWidget(comboWilayahHub);
    wilayahLayout->addStretch();
    keranjangLayout->addLayout(wilayahLayout);

    QHBoxLayout *opsiKirimLayout = new QHBoxLayout();
    lblOpsiKirimTitle = new QLabel("⚡ Layanan Pengiriman:", pageKeranjang);
    lblOpsiKirimTitle->setStyleSheet("font-weight:bold; color:#16a085;");
    comboPengiriman = new QComboBox(pageKeranjang);
    comboPengiriman->addItem("📦 Reguler Standard (+Rp 10.000/Sub-Paket)");
    comboPengiriman->addItem("⚡ Instan Kilat (+Rp 25.000/Sub-Paket)");
    comboPengiriman->setMaximumWidth(320);
    comboPengiriman->setVisible(false); // disembunyikan, hanya dipakai 1 paket
    btnAturLayanan = new QPushButton("⚙ Atur Layanan Per Paket", pageKeranjang);
    btnAturLayanan->setStyleSheet("background:#16a085; color:white; font-weight:bold; padding:6px 14px; border-radius:5px;");
    opsiKirimLayout->addWidget(lblOpsiKirimTitle);
    opsiKirimLayout->addWidget(comboPengiriman);
    opsiKirimLayout->addWidget(btnAturLayanan);
    opsiKirimLayout->addStretch();
    keranjangLayout->addLayout(opsiKirimLayout);

    lblRingkasanLayanan = new QLabel("Belum ada produk di keranjang.", pageKeranjang);
    lblRingkasanLayanan->setStyleSheet("color:#7f8c8d; font-size:11px; font-style:italic; padding:2px 0;");
    lblRingkasanLayanan->setWordWrap(true);
    keranjangLayout->addWidget(lblRingkasanLayanan);

    lblHasilBFS = new QLabel("", pageKeranjang);
    lblHasilBFS->setStyleSheet("color:#7f8c8d; font-size:11px; font-style:italic; margin-top:5px;");
    keranjangLayout->addWidget(lblHasilBFS);

    QHBoxLayout *cartButtonLayout = new QHBoxLayout();
    btnHapusCart = new QPushButton("❌ Hapus Item Terpilih", pageKeranjang);
    btnCheckout  = new QPushButton("🚀 Checkout (Auto Split Paket)", pageKeranjang);
    btnCheckout->setStyleSheet("background:#e67e22; color:white; font-weight:bold; min-height:35px;");
    cartButtonLayout->addWidget(btnHapusCart);
    cartButtonLayout->addWidget(btnCheckout);
    keranjangLayout->addLayout(cartButtonLayout);
    keranjangLayout->addStretch();
    stackedWidget->addWidget(pageKeranjang);  // index 1

    // ===================== PAGE 2: ANTREAN (ADMIN) =====================
    pageAntrean = new QWidget();
    QVBoxLayout *antreanLayout = new QVBoxLayout(pageAntrean);
    lblQueueTitle = new QLabel("📦 Antrean Sub-Paket Siap Kirim (Queue Priority) — ADMIN", pageAntrean);
    lblQueueTitle->setStyleSheet("font-size:15px; font-weight:bold; color:#2c3e50;");
    antreanLayout->addWidget(lblQueueTitle);
    listQueue = new QListWidget(pageAntrean);
    listQueue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    antreanLayout->addWidget(listQueue);
    txtDetailAntrean = new QTextEdit(pageAntrean);
    txtDetailAntrean->setReadOnly(true);
    txtDetailAntrean->setPlaceholderText("Detail nota sub-paket akan muncul di sini...");
    antreanLayout->addWidget(txtDetailAntrean);
    btnProsesOrder = new QPushButton("⚙ Proses & Serahkan Paket Teratas ke Kurir (Dequeue)", pageAntrean);
    btnProsesOrder->setStyleSheet("background:#2ecc71; color:white; font-weight:bold; min-height:30px;");
    antreanLayout->addWidget(btnProsesOrder);
    antreanLayout->addStretch();
    stackedWidget->addWidget(pageAntrean);  // index 2

    // ===================== PAGE 3: RUTE GRAPH (ADMIN) =====================
    pageRute = new QWidget();
    QVBoxLayout *ruteLayout = new QVBoxLayout(pageRute);
    lblGraphTitle = new QLabel("🗺 Jaringan Rute Logistik (Graph) — ADMIN", pageRute);
    lblGraphTitle->setStyleSheet("font-size:15px; font-weight:bold; color:#2c3e50;");
    ruteLayout->addWidget(lblGraphTitle);
    txtGraphNetwork = new QTextEdit(pageRute);
    txtGraphNetwork->setReadOnly(true);
    txtGraphNetwork->setMaximumHeight(200);
    ruteLayout->addWidget(txtGraphNetwork);

    // Kontrol BFS
    QHBoxLayout *bfsLayout = new QHBoxLayout();
    QLabel *lblBfsFrom = new QLabel("BFS Dari:", pageRute);
    QComboBox *comboBfsFrom = new QComboBox(pageRute);
    QLabel *lblBfsTo = new QLabel("→ Ke:", pageRute);
    QComboBox *comboBfsTo = new QComboBox(pageRute);
    for (auto& hub : {"Hub Jakarta","Sub-Hub Bekasi","Hub Surabaya","Hub Merauke"}) {
        comboBfsFrom->addItem(hub);
        comboBfsTo->addItem(hub);
    }
    comboBfsTo->setCurrentIndex(3); // default tujuan: Merauke
    QPushButton *btnRunBFS = new QPushButton("▶ Jalankan BFS", pageRute);
    btnRunBFS->setStyleSheet("background:#2980b9; color:white; font-weight:bold;");
    bfsLayout->addWidget(lblBfsFrom); bfsLayout->addWidget(comboBfsFrom);
    bfsLayout->addWidget(lblBfsTo);   bfsLayout->addWidget(comboBfsTo);
    bfsLayout->addWidget(btnRunBFS);
    ruteLayout->addLayout(bfsLayout);

    // Kontrol DFS
    QHBoxLayout *dfsLayout = new QHBoxLayout();
    QLabel *lblDfsFrom = new QLabel("DFS Mulai dari:", pageRute);
    QComboBox *comboDfsFrom = new QComboBox(pageRute);
    for (auto& hub : {"Hub Jakarta","Sub-Hub Bekasi","Hub Surabaya","Hub Merauke"})
        comboDfsFrom->addItem(hub);
    QPushButton *btnRunDFS = new QPushButton("▶ Jalankan DFS", pageRute);
    btnRunDFS->setStyleSheet("background:#8e44ad; color:white; font-weight:bold;");
    dfsLayout->addWidget(lblDfsFrom); dfsLayout->addWidget(comboDfsFrom);
    dfsLayout->addWidget(btnRunDFS);  dfsLayout->addStretch();
    ruteLayout->addLayout(dfsLayout);

    lblHasilDFS = new QLabel("Klik tombol BFS atau DFS di atas untuk menjalankan simulasi.", pageRute);
    lblHasilDFS->setStyleSheet("padding:10px; background:#f8f9f9; border-left:5px solid #9b59b6; font-weight:bold; font-size:12px; color:#2c3e50;");
    lblHasilDFS->setWordWrap(true);
    ruteLayout->addWidget(lblHasilDFS);

    ruteLayout->addStretch();
    stackedWidget->addWidget(pageRute);  // index 3

    // Koneksikan BFS/DFS/AVL buttons (setelah semua widget ada)
    connect(btnRunBFS, &QPushButton::clicked, this, [this, comboBfsFrom, comboBfsTo](){
        std::string dari = comboBfsFrom->currentText().toStdString();
        std::string ke   = comboBfsTo->currentText().toStdString();
        QString hasil = QString::fromStdString(petaLogistik.runBFS(dari, ke));
        lblHasilDFS->setText(hasil);
    });
    connect(btnRunDFS, &QPushButton::clicked, this, [this, comboDfsFrom](){
        std::string dari = comboDfsFrom->currentText().toStdString();
        QString hasil = QString::fromStdString(petaLogistik.runDFS(dari));
        lblHasilDFS->setText(hasil);
    });

    // ===================== PAGE 4: DASHBOARD ADMIN =====================
    pageAdminDashboard = new QWidget();
    QVBoxLayout *adminLayout = new QVBoxLayout(pageAdminDashboard);
    lblAdminTitle = new QLabel("🖥 Dashboard Admin — Manajemen Status Pengiriman Paket", pageAdminDashboard);
    lblAdminTitle->setStyleSheet("font-size:15px; font-weight:bold; color:#2c3e50;");
    adminLayout->addWidget(lblAdminTitle);

    QLabel *lblAdminInfo = new QLabel(
        "ℹ️  Pilih paket dari daftar → dropdown otomatis menampilkan HANYA hub berikutnya di rute paket tersebut "
        "(misal: Jakarta→Surabaya→Merauke, admin tidak bisa pilih Bekasi). "
        "Waktu berkurang sesuai jarak nyata antar-hub.",
        pageAdminDashboard);
    lblAdminInfo->setStyleSheet(
        "color:#7f8c8d; font-size:12px; padding:6px 8px; "
        "background:#f0f3f4; border-radius:4px; border-left:3px solid #8e44ad;");
    lblAdminInfo->setWordWrap(true);
    adminLayout->addWidget(lblAdminInfo);

    listAdminOrders = new QListWidget(pageAdminDashboard);
    listAdminOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listAdminOrders->setMaximumHeight(150);
    adminLayout->addWidget(listAdminOrders);

    txtAdminDetail = new QTextEdit(pageAdminDashboard);
    txtAdminDetail->setReadOnly(true);
    txtAdminDetail->setMaximumHeight(120);
    adminLayout->addWidget(txtAdminDetail);

    QHBoxLayout *updateLayout = new QHBoxLayout();
    QLabel *lblStatus = new QLabel("Hub Tujuan Berikutnya:", pageAdminDashboard);
    lblStatus->setStyleSheet("font-weight:bold;");
    comboAdminStatus = new QComboBox(pageAdminDashboard);
    comboAdminStatus->setPlaceholderText("← Pilih paket dulu");
    comboAdminStatus->setMinimumWidth(280);
    // Opsi diisi dinamis saat admin memilih paket dari list (hanya hub di rute paket tsb)
    btnAdminUpdate = new QPushButton("✅ Update Lokasi & Kurangi Waktu Tempuh", pageAdminDashboard);
    btnAdminUpdate->setStyleSheet("background:#8e44ad; color:white; font-weight:bold; min-height:30px; padding:0 12px;");
    btnAdminUpdate->setEnabled(false); // aktif setelah paket dipilih
    updateLayout->addWidget(lblStatus);
    updateLayout->addWidget(comboAdminStatus);
    updateLayout->addWidget(btnAdminUpdate);
    updateLayout->addStretch();
    adminLayout->addLayout(updateLayout);

    // =========================================================
    // No.8: AVL TREE — Panel Pencarian & Visualisasi (Admin)
    // =========================================================
    QGroupBox *grpAVL = new QGroupBox("🌳 AVL Tree — Manajemen Produk by ID (O(log N) Search)", pageAdminDashboard);
    grpAVL->setStyleSheet("QGroupBox { font-weight:bold; color:#16a085; border:2px solid #16a085; border-radius:6px; margin-top:8px; padding-top:6px; }");
    QVBoxLayout *avlLayout = new QVBoxLayout(grpAVL);

    QLabel *lblAVLInfo = new QLabel(
        "AVL Tree menyimpan seluruh produk by ID dengan auto-rebalance. "
        "Pencarian dijamin O(log N) karena balance factor setiap node ∈ {-1, 0, 1}.",
        grpAVL);
    lblAVLInfo->setStyleSheet("color:#7f8c8d; font-size:11px;");
    lblAVLInfo->setWordWrap(true);
    avlLayout->addWidget(lblAVLInfo);

    QHBoxLayout *avlSearchLayout = new QHBoxLayout();
    QLabel *lblAVLSearch = new QLabel("Cari Produk by ID:", grpAVL);
    lblAVLSearch->setStyleSheet("font-weight:bold; color:#2c3e50;");
    QLineEdit *txtAVLSearch = new QLineEdit(grpAVL);
    txtAVLSearch->setPlaceholderText("Masukkan ID Produk (contoh: 101, 202, 303)");
    txtAVLSearch->setMaximumWidth(260);
    QPushButton *btnAVLSearch = new QPushButton("🔍 Cari (AVL)", grpAVL);
    btnAVLSearch->setStyleSheet("background:#16a085; color:white; font-weight:bold; padding:4px 12px;");
    QPushButton *btnAVLVisualize = new QPushButton("🌳 Lihat Struktur AVL", grpAVL);
    btnAVLVisualize->setStyleSheet("background:#2c3e50; color:white; font-weight:bold; padding:4px 12px;");
    avlSearchLayout->addWidget(lblAVLSearch);
    avlSearchLayout->addWidget(txtAVLSearch);
    avlSearchLayout->addWidget(btnAVLSearch);
    avlSearchLayout->addWidget(btnAVLVisualize);
    avlSearchLayout->addStretch();
    avlLayout->addLayout(avlSearchLayout);

    QTextEdit *txtAVLResult = new QTextEdit(grpAVL);
    txtAVLResult->setReadOnly(true);
    txtAVLResult->setMaximumHeight(80);
    txtAVLResult->setFont(QFont("Courier", 9));
    txtAVLResult->setStyleSheet("background:#f4f6f7; color:#2c3e50;");
    txtAVLResult->setPlaceholderText("Hasil pencarian AVL Tree akan muncul di sini...");
    avlLayout->addWidget(txtAVLResult);

    adminLayout->addWidget(grpAVL);
    adminLayout->addStretch();
    stackedWidget->addWidget(pageAdminDashboard);  // index 4

    // Koneksi AVL Search button
    connect(btnAVLSearch, &QPushButton::clicked, this, [this, txtAVLSearch, txtAVLResult](){
        QString input = txtAVLSearch->text().trimmed();
        if (input.isEmpty()) {
            txtAVLResult->setText("⚠ Masukkan ID produk terlebih dahulu.");
            return;
        }
        bool ok;
        int targetId = input.toInt(&ok);
        if (!ok) {
            txtAVLResult->setText("⚠ ID harus berupa angka bulat (contoh: 101, 202).");
            return;
        }

        // Lakukan pencarian O(log N) di AVL Tree
        AVLNode* found = avlTree.search(targetId);
        if (found) {
            QString result = QString(
                                 "✅ DITEMUKAN via AVL Tree Search O(log N)\n"
                                 "ID       : %1\n"
                                 "Nama     : %2\n"
                                 "Kategori : %3\n"
                                 "Harga    : Rp %4\n"
                                 "Asal Hub : %5\n"
                                 "Tree Height: %6 (garantir O(log N) = max %7 langkah)"
                                 ).arg(found->id)
                                 .arg(QString::fromStdString(found->name))
                                 .arg(QString::fromStdString(found->category))
                                 .arg(found->price)
                                 .arg(QString::fromStdString(found->origin))
                                 .arg(avlTree.getTreeHeight())
                                 .arg(avlTree.getTreeHeight());
            txtAVLResult->setText(result);
        } else {
            txtAVLResult->setText(QString(
                                      "❌ ID %1 tidak ditemukan dalam AVL Tree.\n"
                                      "ID tersedia: 101, 102, 103, 104, 201, 202, 203, 302, 303"
                                      ).arg(targetId));
        }
    });

    // Koneksi tombol lihat visualisasi struktur AVL
    connect(btnAVLVisualize, &QPushButton::clicked, this, [this](){
        QDialog *dlg = new QDialog(this);
        dlg->setWindowTitle("Visualisasi AVL Tree — Produk by ID");
        dlg->setMinimumSize(520, 420);
        QVBoxLayout *lay = new QVBoxLayout(dlg);

        QLabel *lblHead = new QLabel(
            "🌳 Setiap node menampilkan ID, nama, dan balance factor (bf).\n"
            "bf = height(kiri) - height(kanan). AVL menjaga |bf| ≤ 1 dengan rotasi otomatis.",
            dlg);
        lblHead->setStyleSheet("background:#eafaf1; padding:8px; border-radius:5px; color:#145a32;");
        lblHead->setWordWrap(true);
        lay->addWidget(lblHead);

        // Tabel semua produk (In-Order dari AVL = terurut by ID)
        QTableWidget *tblAVL = new QTableWidget(dlg);
        tblAVL->setColumnCount(5);
        tblAVL->setHorizontalHeaderLabels({"ID","Nama","Kategori","Harga","Asal Hub"});
        tblAVL->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tblAVL->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblAVL->setSelectionBehavior(QAbstractItemView::SelectRows);
        auto allNodes = avlTree.getAllSortedById();
        tblAVL->setRowCount((int)allNodes.size());
        for (int i = 0; i < (int)allNodes.size(); ++i) {
            tblAVL->setItem(i,0,new QTableWidgetItem(QString::number(allNodes[i]->id)));
            tblAVL->setItem(i,1,new QTableWidgetItem(QString::fromStdString(allNodes[i]->name)));
            tblAVL->setItem(i,2,new QTableWidgetItem(QString::fromStdString(allNodes[i]->category)));
            tblAVL->setItem(i,3,new QTableWidgetItem("Rp "+QString::number(allNodes[i]->price)));
            tblAVL->setItem(i,4,new QTableWidgetItem(QString::fromStdString(allNodes[i]->origin)));
        }
        tblAVL->setMaximumHeight(180);
        lay->addWidget(new QLabel("📋 Semua Produk (In-Order Traversal AVL = urut ID ascending):", dlg));
        lay->addWidget(tblAVL);

        // Visualisasi ASCII pohon
        QTextEdit *txtTree = new QTextEdit(dlg);
        txtTree->setReadOnly(true);
        txtTree->setFont(QFont("Courier", 10));
        txtTree->setText(QString::fromStdString(avlTree.getTreeVisualization()));
        lay->addWidget(new QLabel("🌿 Struktur Pohon AVL (bf = balance factor):", dlg));
        lay->addWidget(txtTree);

        QString validasi = avlTree.isValidAVL()
                               ? "✅ AVL Valid: semua balance factor ∈ {-1, 0, 1}"
                               : "❌ AVL Tidak Valid!";
        QLabel *lblValid = new QLabel(validasi + QString(" | Tree Height: %1").arg(avlTree.getTreeHeight()), dlg);
        lblValid->setStyleSheet("font-weight:bold; color:#16a085; padding:4px;");
        lay->addWidget(lblValid);

        QPushButton *btnOk = new QPushButton("Tutup", dlg);
        connect(btnOk, &QPushButton::clicked, dlg, &QDialog::accept);
        lay->addWidget(btnOk);
        dlg->exec();
    });

    // ===================== PAGE 5: TRACKING PELANGGAN =====================
    pageTrackPelanggan = new QWidget();
    QVBoxLayout *trackLayout = new QVBoxLayout(pageTrackPelanggan);
    lblTrackTitle = new QLabel("📡 Lacak Paket Saya", pageTrackPelanggan);
    lblTrackTitle->setStyleSheet("font-size:16px; font-weight:bold; color:#2c3e50;");
    trackLayout->addWidget(lblTrackTitle);

    QLabel *lblTrackInfo = new QLabel(
        "ℹ️  Status di bawah diperbarui oleh Admin. "
        "Klik Refresh untuk melihat data terbaru.",
        pageTrackPelanggan);
    lblTrackInfo->setStyleSheet("color:#7f8c8d; font-size:12px; padding:2px 0 6px 0;");
    lblTrackInfo->setWordWrap(true);
    trackLayout->addWidget(lblTrackInfo);

    listTrackOrders = new QListWidget(pageTrackPelanggan);
    listTrackOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listTrackOrders->setMaximumHeight(200);
    trackLayout->addWidget(listTrackOrders);

    txtTrackDetail = new QTextEdit(pageTrackPelanggan);
    txtTrackDetail->setReadOnly(true);
    trackLayout->addWidget(txtTrackDetail);

    btnRefreshTrack = new QPushButton("🔄 Refresh Status Paket", pageTrackPelanggan);
    btnRefreshTrack->setStyleSheet("background:#2980b9; color:white; font-weight:bold; min-height:30px;");
    trackLayout->addWidget(btnRefreshTrack);
    trackLayout->addStretch();
    stackedWidget->addWidget(pageTrackPelanggan);  // index 5

    // ===================== PAGE 6: MANAJEMEN PRODUK (ADMIN) =====================
    pageManajemenProduk = new QWidget();
    QVBoxLayout *mpLayout = new QVBoxLayout(pageManajemenProduk);

    lblManajemenTitle = new QLabel("✏️ Manajemen Produk — Add / Update / Delete (Admin)", pageManajemenProduk);
    lblManajemenTitle->setStyleSheet("font-size:15px; font-weight:bold; color:#2c3e50;");
    mpLayout->addWidget(lblManajemenTitle);

    QLabel *lblMpInfo = new QLabel(
        "ℹ️  Klik baris tabel untuk memuat data ke form. "
        "Isi semua field lalu klik tombol aksi yang diinginkan.",
        pageManajemenProduk);
    lblMpInfo->setStyleSheet("color:#7f8c8d; font-size:11px; padding:2px 0 6px 0;");
    lblMpInfo->setWordWrap(true);
    mpLayout->addWidget(lblMpInfo);

    // Tabel produk
    tabelProdukAdmin = new QTableWidget(pageManajemenProduk);
    tabelProdukAdmin->setColumnCount(5);
    tabelProdukAdmin->setHorizontalHeaderLabels({"ID","Nama","Kategori","Harga","Hub Asal"});
    tabelProdukAdmin->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tabelProdukAdmin->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabelProdukAdmin->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabelProdukAdmin->setAlternatingRowColors(true);
    tabelProdukAdmin->setMaximumHeight(220);
    mpLayout->addWidget(tabelProdukAdmin);

    // Form input produk
    QGroupBox *grpForm = new QGroupBox("📝 Form Produk", pageManajemenProduk);
    grpForm->setStyleSheet("QGroupBox { font-weight:bold; color:#2980b9; border:2px solid #2980b9; border-radius:6px; margin-top:8px; padding-top:6px; }");
    QFormLayout *formMp = new QFormLayout(grpForm);
    formMp->setSpacing(8);

    txtProdukId    = new QLineEdit(grpForm);
    txtProdukId->setPlaceholderText("Contoh: 401 (unik, angka)");
    txtProdukId->setMaximumWidth(200);

    txtProdukNama  = new QLineEdit(grpForm);
    txtProdukNama->setPlaceholderText("Nama produk");

    comboProdukKategori = new QComboBox(grpForm);
    comboProdukKategori->addItems({"Elektronik","Pakaian","Perabotan"});
    comboProdukKategori->setMaximumWidth(200);

    txtProdukHarga = new QLineEdit(grpForm);
    txtProdukHarga->setPlaceholderText("Harga dalam Rupiah, contoh: 150000");
    txtProdukHarga->setMaximumWidth(200);

    comboProdukHub = new QComboBox(grpForm);
    comboProdukHub->addItems({"Hub Jakarta","Sub-Hub Bekasi","Hub Surabaya","Hub Merauke"});
    comboProdukHub->setMaximumWidth(200);

    formMp->addRow("ID Produk :",  txtProdukId);
    formMp->addRow("Nama :",       txtProdukNama);
    formMp->addRow("Kategori :",   comboProdukKategori);
    formMp->addRow("Harga (Rp):", txtProdukHarga);
    formMp->addRow("Hub Asal :",   comboProdukHub);
    mpLayout->addWidget(grpForm);

    // Tombol aksi
    QHBoxLayout *mpBtnLayout = new QHBoxLayout();
    btnProdukTambah    = new QPushButton("➕ Tambah Produk",   pageManajemenProduk);
    btnProdukUpdate    = new QPushButton("✏️ Update Produk",   pageManajemenProduk);
    btnProdukHapus     = new QPushButton("🗑 Hapus Produk",    pageManajemenProduk);
    btnProdukBersihkan = new QPushButton("🧹 Bersihkan Form",  pageManajemenProduk);

    btnProdukTambah->setStyleSheet("background:#27ae60; color:white; font-weight:bold; min-height:32px; padding:0 12px;");
    btnProdukUpdate->setStyleSheet("background:#2980b9; color:white; font-weight:bold; min-height:32px; padding:0 12px;");
    btnProdukHapus->setStyleSheet("background:#e74c3c; color:white; font-weight:bold; min-height:32px; padding:0 12px;");
    btnProdukBersihkan->setStyleSheet("background:#95a5a6; color:white; font-weight:bold; min-height:32px; padding:0 12px;");

    mpBtnLayout->addWidget(btnProdukTambah);
    mpBtnLayout->addWidget(btnProdukUpdate);
    mpBtnLayout->addWidget(btnProdukHapus);
    mpBtnLayout->addWidget(btnProdukBersihkan);
    mpBtnLayout->addStretch();
    mpLayout->addLayout(mpBtnLayout);
    mpLayout->addStretch();
    stackedWidget->addWidget(pageManajemenProduk);  // index 6

    // ===================== PAGE 7: HASH TABLE QUICK LOOKUP (ADMIN) =====================
    pageHashLookup = new QWidget();
    QVBoxLayout *hashLayout = new QVBoxLayout(pageHashLookup);

    lblHashTitle = new QLabel("🔎 Hash Table — Quick Lookup O(1)", pageHashLookup);
    lblHashTitle->setStyleSheet("font-size:15px; font-weight:bold; color:#2c3e50;");
    hashLayout->addWidget(lblHashTitle);

    QLabel *lblHashInfo = new QLabel(
        "ℹ️  Hash Table menyediakan lookup O(1) rata-rata. "
        "Tiga tabel terpisah: Produk by ID, Voucher by Kode, dan Order by Resi.",
        pageHashLookup);
    lblHashInfo->setStyleSheet("color:#7f8c8d; font-size:11px; padding:2px 0 8px 0;");
    lblHashInfo->setWordWrap(true);
    hashLayout->addWidget(lblHashInfo);

    // --- 3 grup lookup sejajar ---
    QHBoxLayout *hashTripleLayout = new QHBoxLayout();

    // --- GRUP 1: Product Lookup ---
    QGroupBox *grpProd = new QGroupBox("1️⃣  Product Lookup by ID", pageHashLookup);
    grpProd->setStyleSheet("QGroupBox { font-weight:bold; color:#27ae60; border:2px solid #27ae60; border-radius:6px; margin-top:8px; padding-top:6px; }");
    QVBoxLayout *vProd = new QVBoxLayout(grpProd);

    QHBoxLayout *hProd = new QHBoxLayout();
    txtHashProdukId = new QLineEdit(grpProd);
    txtHashProdukId->setPlaceholderText("Masukkan ID produk (contoh: 101)");
    btnHashCariProduk = new QPushButton("🔍 Cari", grpProd);
    btnHashCariProduk->setStyleSheet("background:#27ae60; color:white; font-weight:bold; min-height:28px; padding:0 10px;");
    hProd->addWidget(txtHashProdukId);
    hProd->addWidget(btnHashCariProduk);
    vProd->addLayout(hProd);

    txtHashProdukResult = new QTextEdit(grpProd);
    txtHashProdukResult->setReadOnly(true);
    txtHashProdukResult->setMinimumHeight(100);
    txtHashProdukResult->setPlaceholderText("Hasil lookup produk akan muncul di sini...");
    txtHashProdukResult->setStyleSheet("font-family: monospace; font-size:11px;");
    vProd->addWidget(txtHashProdukResult);
    hashTripleLayout->addWidget(grpProd);

    // --- GRUP 2: Voucher Lookup ---
    QGroupBox *grpVouc = new QGroupBox("2️⃣  Voucher Code Lookup", pageHashLookup);
    grpVouc->setStyleSheet("QGroupBox { font-weight:bold; color:#2980b9; border:2px solid #2980b9; border-radius:6px; margin-top:8px; padding-top:6px; }");
    QVBoxLayout *vVouc = new QVBoxLayout(grpVouc);

    QHBoxLayout *hVouc = new QHBoxLayout();
    txtHashVoucherKode = new QLineEdit(grpVouc);
    txtHashVoucherKode->setPlaceholderText("Kode voucher (contoh: HEMAT20)");
    btnHashCariVoucher = new QPushButton("🔍 Cari", grpVouc);
    btnHashCariVoucher->setStyleSheet("background:#2980b9; color:white; font-weight:bold; min-height:28px; padding:0 10px;");
    hVouc->addWidget(txtHashVoucherKode);
    hVouc->addWidget(btnHashCariVoucher);
    vVouc->addLayout(hVouc);

    txtHashVoucherResult = new QTextEdit(grpVouc);
    txtHashVoucherResult->setReadOnly(true);
    txtHashVoucherResult->setMinimumHeight(100);
    txtHashVoucherResult->setPlaceholderText("Hasil lookup voucher akan muncul di sini...");
    txtHashVoucherResult->setStyleSheet("font-family: monospace; font-size:11px;");
    vVouc->addWidget(txtHashVoucherResult);
    hashTripleLayout->addWidget(grpVouc);

    // --- GRUP 3: Order Lookup ---
    QGroupBox *grpOrd = new QGroupBox("3️⃣  Order Lookup by Resi ID", pageHashLookup);
    grpOrd->setStyleSheet("QGroupBox { font-weight:bold; color:#8e44ad; border:2px solid #8e44ad; border-radius:6px; margin-top:8px; padding-top:6px; }");
    QVBoxLayout *vOrd = new QVBoxLayout(grpOrd);

    QHBoxLayout *hOrd = new QHBoxLayout();
    txtHashOrderId = new QLineEdit(grpOrd);
    txtHashOrderId->setPlaceholderText("Resi ID (contoh: PKT-12345)");
    btnHashCariOrder = new QPushButton("🔍 Cari", grpOrd);
    btnHashCariOrder->setStyleSheet("background:#8e44ad; color:white; font-weight:bold; min-height:28px; padding:0 10px;");
    hOrd->addWidget(txtHashOrderId);
    hOrd->addWidget(btnHashCariOrder);
    vOrd->addLayout(hOrd);

    txtHashOrderResult = new QTextEdit(grpOrd);
    txtHashOrderResult->setReadOnly(true);
    txtHashOrderResult->setMinimumHeight(100);
    txtHashOrderResult->setPlaceholderText("Hasil lookup order/resi akan muncul di sini...");
    txtHashOrderResult->setStyleSheet("font-family: monospace; font-size:11px;");
    vOrd->addWidget(txtHashOrderResult);
    hashTripleLayout->addWidget(grpOrd);

    hashLayout->addLayout(hashTripleLayout);

    // --- Stats Panel ---
    QGroupBox *grpStats = new QGroupBox("📊 Statistik Hash Tables (Load Factor & Collision Info)", pageHashLookup);
    grpStats->setStyleSheet("QGroupBox { font-weight:bold; color:#e67e22; border:2px solid #e67e22; border-radius:6px; margin-top:8px; padding-top:6px; }");
    QVBoxLayout *vStats = new QVBoxLayout(grpStats);

    txtHashStats = new QTextEdit(grpStats);
    txtHashStats->setReadOnly(true);
    txtHashStats->setMaximumHeight(110);
    txtHashStats->setStyleSheet("font-family: monospace; font-size:11px;");
    txtHashStats->setPlaceholderText("Klik salah satu tombol Cari untuk melihat statistik tabel...");
    QPushButton *btnRefreshStats = new QPushButton("🔄 Refresh Stats", grpStats);
    btnRefreshStats->setStyleSheet("background:#e67e22; color:white; font-weight:bold; min-height:26px; padding:0 10px;");
    vStats->addWidget(txtHashStats);
    vStats->addWidget(btnRefreshStats);
    hashLayout->addWidget(grpStats);
    hashLayout->addStretch();
    stackedWidget->addWidget(pageHashLookup);  // index 7

    // --- Koneksi signals PAGE 7 ---
    connect(btnHashCariProduk, &QPushButton::clicked, this, &MainWindow::klikQuickLookup);
    connect(btnHashCariVoucher, &QPushButton::clicked, this, &MainWindow::klikQuickLookup);
    connect(btnHashCariOrder,  &QPushButton::clicked, this, &MainWindow::klikQuickLookup);

    connect(btnRefreshStats, &QPushButton::clicked, this, [this](){
        QString stats;
        stats += QString::fromStdString(productHashTable.getStatsString()) + "\n\n";
        stats += QString::fromStdString(voucherHashTable.getStatsString()) + "\n\n";
        stats += QString::fromStdString(orderHashTable.getStatsString());
        txtHashStats->setText(stats);
    });

    // Klik baris tabel → isi form otomatis
    connect(tabelProdukAdmin, &QTableWidget::itemSelectionChanged, this, [this](){
        int row = tabelProdukAdmin->currentRow();
        if (row < 0 || row >= tabelProdukAdmin->rowCount()) return;
        txtProdukId->setText(tabelProdukAdmin->item(row,0)->text());
        txtProdukNama->setText(tabelProdukAdmin->item(row,1)->text());
        comboProdukKategori->setCurrentText(tabelProdukAdmin->item(row,2)->text());
        // strip "Rp " prefix jika ada
        QString hargaStr = tabelProdukAdmin->item(row,3)->text();
        hargaStr.remove("Rp ").remove(".");
        txtProdukHarga->setText(hargaStr);
        comboProdukHub->setCurrentText(tabelProdukAdmin->item(row,4)->text());
    });

    connect(btnManajemenProduk,  &QPushButton::clicked, this, &MainWindow::navigasiAdmin);
    connect(btnHashLookup,       &QPushButton::clicked, this, &MainWindow::navigasiAdmin);
    connect(btnProdukTambah,     &QPushButton::clicked, this, &MainWindow::klikTambahProdukAdmin);
    connect(btnProdukUpdate,     &QPushButton::clicked, this, &MainWindow::klikUpdateProdukAdmin);
    connect(btnProdukHapus,      &QPushButton::clicked, this, &MainWindow::klikHapusProdukAdmin);
    connect(btnProdukBersihkan,  &QPushButton::clicked, this, [this](){
        txtProdukId->clear(); txtProdukNama->clear();
        txtProdukHarga->clear();
        comboProdukKategori->setCurrentIndex(0);
        comboProdukHub->setCurrentIndex(0);
        tabelProdukAdmin->clearSelection();
    });

    shellLayout->addWidget(sidebarFrame);
    shellLayout->addWidget(stackedWidget);
    rootStack->addWidget(shellWidget);  // index 1

    // =========================================================
    // KONEKSI SIGNAL & SLOT
    // =========================================================
    connect(btnLoginMasuk, &QPushButton::clicked, this, &MainWindow::klikLogin);
    connect(txtLoginPass,  &QLineEdit::returnPressed, this, &MainWindow::klikLogin);
    connect(btnLogout,     &QPushButton::clicked, this, &MainWindow::klikLogout);
    connect(btnBack,       &QPushButton::clicked, this, &MainWindow::klikBack);

    connect(btnKatalog,        &QPushButton::clicked, this, &MainWindow::navigasiHalaman);
    connect(btnKeranjang,      &QPushButton::clicked, this, &MainWindow::navigasiHalaman);
    connect(btnAntrean,        &QPushButton::clicked, this, &MainWindow::navigasiAdmin);
    connect(btnRute,           &QPushButton::clicked, this, &MainWindow::navigasiAdmin);
    connect(btnAdminDashboard, &QPushButton::clicked, this, &MainWindow::navigasiAdmin);
    connect(btnTrackPelanggan, &QPushButton::clicked, this, &MainWindow::navigasiHalaman);
    connect(btnKatElektronik,  &QPushButton::clicked, this, &MainWindow::gantiKategoriKatalog);
    connect(btnKatPakaian,    &QPushButton::clicked, this, &MainWindow::gantiKategoriKatalog);
    connect(btnKatPerabotan,  &QPushButton::clicked, this, &MainWindow::gantiKategoriKatalog);
    connect(btnNextCatalog,   &QPushButton::clicked, this, &MainWindow::klikNextProduk);
    connect(btnPrevCatalog,   &QPushButton::clicked, this, &MainWindow::klikPrevProduk);
    connect(btnUrutHarga,     &QPushButton::clicked, this, &MainWindow::klikUrutHarga);
    connect(btnSearchId,      &QPushButton::clicked, this, &MainWindow::klikCariId);
    connect(btnAddToCart,     &QPushButton::clicked, this, &MainWindow::klikTambahKeranjang);
    connect(btnHapusCart,     &QPushButton::clicked, this, &MainWindow::klikHapusCart);
    connect(btnCheckout,      &QPushButton::clicked, this, &MainWindow::klikCheckout);

    connect(btnProsesOrder,   &QPushButton::clicked, this, &MainWindow::klikProsesOrderAdmin);
    connect(btnAdminUpdate,   &QPushButton::clicked, this, &MainWindow::klikUpdateStatusAdmin);
    connect(btnRefreshTrack,  &QPushButton::clicked, this, &MainWindow::klikRefreshTrackingPelanggan);

    connect(btnPakaiDiskon, &QPushButton::clicked, this, [this](){
        QString kode = txtKodeDiskon->text().trimmed().toUpper();
        if (kode.isEmpty()) { QMessageBox::warning(this,"Kode Kosong","Masukkan kode promo terlebih dahulu!"); return; }
        auto [valid, diskon] = cariKodeDiskon(kode);
        if (valid) {
            QMessageBox::information(this, "Kode Valid",
                                     QString("Kode '%1' berhasil diterapkan!\n%2").arg(diskon->kode).arg(diskon->deskripsi));
        } else {
            QMessageBox::warning(this, "Kode Tidak Valid",
                                 QString("Kode '%1' tidak ditemukan.\n\nKode tersedia:\n- CCART50\n- HEMAT20\n- WELCOME10\n- FLASH30\n- GRATIS").arg(kode));
        }
        refreshTampilanKeranjang();
    });
    connect(comboWilayahHub, &QComboBox::currentTextChanged, this, [this](const QString&){ refreshTampilanKeranjang(); });
    connect(comboPengiriman, &QComboBox::currentTextChanged, this, [this](const QString&){ refreshTampilanKeranjang(); });
    connect(btnAturLayanan,  &QPushButton::clicked,          this, &MainWindow::klikAturLayananPerPaket);

    connect(listAdminOrders, &QListWidget::currentRowChanged, this, [this](int row){
        if (row < 0 || row >= (int)databasePelacakan.size()) { txtAdminDetail->clear(); return; }
        const ActiveOrder& o = databasePelacakan[row];

        // ---- Isi detail teks ----
        QString detail = QString(
                             "RESI        : %1\n"
                             "Pelanggan   : %2\n"
                             "Asal        : %3\n"
                             "Tujuan      : %4\n"
                             "Rute        : %5\n"
                             "Lokasi Kini : %6\n"
                             "Sisa Waktu  : %7 Jam (%8 Hari)\n"
                             "Status      : %9\n"
                             "Tgl Update  : %10"
                             ).arg(QString::fromStdString(o.idPelanggan))
                             .arg(QString::fromStdString(o.namaPelanggan))
                             .arg(QString::fromStdString(o.kotaAsal))
                             .arg(QString::fromStdString(o.kotaTujuan))
                             .arg(getRuteVisual(QString::fromStdString(o.kotaAsal), QString::fromStdString(o.kotaTujuan)))
                             .arg(QString::fromStdString(o.lokasiSekarang))
                             .arg(o.sisaWaktuJam)
                             .arg(double(o.sisaWaktuJam)/24.0, 0, 'f', 1)
                             .arg(QString::fromStdString(o.statusLogistik))
                             .arg(QString::fromStdString(o.lastAdminUpdate));
        txtAdminDetail->setText(detail);

        // ---- [FIX] Isi comboAdminStatus HANYA dengan hub yang ada di rute paket ini ----
        // Ambil jalur hub dari asal ke tujuan (misal Jakarta→Surabaya→Merauke)
        auto jalurPaket = getJalurHub(
            QString::fromStdString(o.kotaAsal),
            QString::fromStdString(o.kotaTujuan)
            );
        QString lokasiKini = QString::fromStdString(o.lokasiSekarang);

        comboAdminStatus->blockSignals(true);
        comboAdminStatus->clear();

        bool sudahSampaiSkrg = (o.statusLogistik.find("Sampai") != std::string::npos);

        // Opsi "sedang dalam perjalanan" — tidak mengubah lokasi & tidak mengurangi sisa waktu
        if (!sudahSampaiSkrg) {
            comboAdminStatus->addItem("🚚 Sedang Dalam Perjalanan");
        }

        // Hanya tampilkan hub BERIKUTNYA dari posisi sekarang (tidak bisa mundur)
        bool lewatiLokasi = true;
        for (auto& hub : jalurPaket) {
            if (hub == lokasiKini) { lewatiLokasi = false; continue; }
            if (lewatiLokasi) continue; // lewati hub sebelum lokasi sekarang
            comboAdminStatus->addItem(hub);
        }
        // Tetap sediakan opsi manual "Paket Telah Sampai Tujuan" di akhir.
        // Catatan: jika admin langsung memilih hub tujuan akhir di atas, paket akan
        // otomatis dianggap "Sampai" — opsi manual ini hanya untuk jaga-jaga / override.
        if (!sudahSampaiSkrg) {
            comboAdminStatus->addItem("✅ Paket Telah Sampai Tujuan");
        }

        comboAdminStatus->blockSignals(false);

        // Nonaktifkan tombol update jika paket sudah sampai
        btnAdminUpdate->setEnabled(!sudahSampaiSkrg);
    });

    connect(listTrackOrders, &QListWidget::currentRowChanged, this, [this](int row){
        // Abaikan jika row tidak valid atau merupakan header/spacer (tidak bisa diklik)
        if (row < 0) { txtTrackDetail->clear(); return; }
        QListWidgetItem* clickedItem = listTrackOrders->item(row);
        if (!clickedItem || !(clickedItem->flags() & Qt::ItemIsEnabled)) {
            txtTrackDetail->clear(); return;
        }

        // Cari ActiveOrder yang sesuai berdasarkan teks item (cocokkan ID paket)
        QString itemText = clickedItem->text();
        const ActiveOrder* foundOrder = nullptr;
        for (auto& o : databasePelacakan) {
            if (o.namaPelanggan != usernameAktif) continue;
            if (itemText.contains(QString::fromStdString(o.idPelanggan))) {
                foundOrder = &o; break;
            }
        }
        if (!foundOrder) { txtTrackDetail->clear(); return; }
        const ActiveOrder& o = *foundOrder;
        if (!o.sudahDiUpdateAdmin) {
            txtTrackDetail->setText("⏳ Paket sedang diproses. Menunggu konfirmasi dari Admin...\n\nSilakan klik Refresh setelah beberapa saat.");
            return;
        }
        QString detail = QString(
                             "📦 RESI      : %1\n"
                             "📍 Dari      : %2\n"
                             "📍 Tujuan    : %3\n"
                             "🛣 Rute      : %4\n\n"
                             "📌 Lokasi Sekarang : %5\n"
                             "⏱ Sisa Waktu     : %6 Jam (%7 Hari)\n"
                             "📋 Status         : %8\n\n"
                             "🕐 Terakhir Update : %9"
                             ).arg(QString::fromStdString(o.idPelanggan))
                             .arg(QString::fromStdString(o.kotaAsal))
                             .arg(QString::fromStdString(o.kotaTujuan))
                             .arg(getRuteVisual(QString::fromStdString(o.kotaAsal), QString::fromStdString(o.kotaTujuan)))
                             .arg(QString::fromStdString(o.lokasiSekarang))
                             .arg(o.sisaWaktuJam)
                             .arg(double(o.sisaWaktuJam)/24.0, 0, 'f', 1)
                             .arg(QString::fromStdString(o.statusLogistik))
                             .arg(QString::fromStdString(o.lastAdminUpdate));
        txtTrackDetail->setText(detail);
    });

    // =========================================================
    // DATABASE PRODUK & GRAPH INITIALIZATION
    // Inisialisasi semua data struktur dengan produk sample saat startup
    // =========================================================

    // ─ No.2: KATALOG DLL — Doubly Linked List per kategori
    // Setiap kategori punya DLL terpisah untuk navigasi prev/next
    // Struktur: ID → Nama → Kategori → Harga → Hub Asal
    katalogElektronikDLL.insertProduct(101, "Keyboard Mechanical RGB", "Elektronik", 450000, "Hub Jakarta");
    katalogElektronikDLL.insertProduct(102, "Mouse Gaming Wireless",   "Elektronik", 250000, "Hub Surabaya");
    katalogElektronikDLL.insertProduct(103, "Headset Gaming Stereo",   "Elektronik", 350000, "Hub Merauke");

    katalogPakaianDLL.insertProduct(201, "Kemeja Flanel Premium",    "Pakaian", 180000, "Hub Jakarta");
    katalogPakaianDLL.insertProduct(202, "Kaos Polos Cotton Combed", "Pakaian",  75000, "Sub-Hub Bekasi");
    katalogPakaianDLL.insertProduct(203, "Jaket Bomber Waterproof",  "Pakaian", 220000, "Hub Surabaya");

    katalogPerabotanDLL.insertProduct(104, "Kursi Gaming Ergonomis", "Perabotan", 1200000, "Hub Surabaya");
    katalogPerabotanDLL.insertProduct(302, "Lampu Belajar LED Mini", "Perabotan",   45000, "Hub Jakarta");
    katalogPerabotanDLL.insertProduct(303, "Meja Lipat Minimalis",   "Perabotan",  150000, "Hub Merauke");

    // ─ No.7: BST — Binary Search Tree untuk Urut Harga
    // Insert produk berdasarkan harga sebagai key (comparator)
    // In-Order traversal → terurut ascending | Reverse In-Order → terurut descending
    // Digunakan di fitur "Urut Harga" untuk sorting real-time tanpa std::sort
    bstElektronik.insert(101, "Keyboard Mechanical RGB", "Elektronik", 450000, "Hub Jakarta");
    bstElektronik.insert(102, "Mouse Gaming Wireless",   "Elektronik", 250000, "Hub Surabaya");
    bstElektronik.insert(103, "Headset Gaming Stereo",   "Elektronik", 350000, "Hub Merauke");

    bstPakaian.insert(201, "Kemeja Flanel Premium",    "Pakaian", 180000, "Hub Jakarta");
    bstPakaian.insert(202, "Kaos Polos Cotton Combed", "Pakaian",  75000, "Sub-Hub Bekasi");
    bstPakaian.insert(203, "Jaket Bomber Waterproof",  "Pakaian", 220000, "Hub Surabaya");

    bstPerabotan.insert(104, "Kursi Gaming Ergonomis", "Perabotan", 1200000, "Hub Surabaya");
    bstPerabotan.insert(302, "Lampu Belajar LED Mini", "Perabotan",   45000, "Hub Jakarta");
    bstPerabotan.insert(303, "Meja Lipat Minimalis",   "Perabotan",  150000, "Hub Merauke");

    // ─ No.8: AVL Tree — Self-Balancing BST untuk O(log N) lookup
    // Indexing semua 9 produk by ID (key)
    // Otomatis balance setiap insert untuk menjaga height ≤ log(N)
    // Digunakan di admin dashboard untuk cari produk by ID dengan O(log N) efficiency
    avlTree.insert(101, "Keyboard Mechanical RGB", "Elektronik", 450000, "Hub Jakarta");
    avlTree.insert(102, "Mouse Gaming Wireless",   "Elektronik", 250000, "Hub Surabaya");
    avlTree.insert(103, "Headset Gaming Stereo",   "Elektronik", 350000, "Hub Merauke");
    avlTree.insert(201, "Kemeja Flanel Premium",   "Pakaian",    180000, "Hub Jakarta");
    avlTree.insert(202, "Kaos Polos Cotton Combed","Pakaian",     75000, "Sub-Hub Bekasi");
    avlTree.insert(203, "Jaket Bomber Waterproof", "Pakaian",    220000, "Hub Surabaya");
    avlTree.insert(104, "Kursi Gaming Ergonomis",  "Perabotan", 1200000, "Hub Surabaya");
    avlTree.insert(302, "Lampu Belajar LED Mini",  "Perabotan",   45000, "Hub Jakarta");
    avlTree.insert(303, "Meja Lipat Minimalis",    "Perabotan",  150000, "Hub Merauke");

    // ─ No.6: Binary Search Index — Array terurut untuk O(log N) pencarian
    // Store produk dalam array yang diurutkan berdasarkan ID dan Nama
    // buildIndex() mengurutkan array — diperlukan sebelum searchById()/searchByNamePrefix()
    // Digunakan di fitur Cari Produk (customer search bar)
    binarySearchIndex.addProduct(101, "Keyboard Mechanical RGB", "Elektronik", 450000, "Hub Jakarta");
    binarySearchIndex.addProduct(102, "Mouse Gaming Wireless",   "Elektronik", 250000, "Hub Surabaya");
    binarySearchIndex.addProduct(103, "Headset Gaming Stereo",   "Elektronik", 350000, "Hub Merauke");
    binarySearchIndex.addProduct(201, "Kemeja Flanel Premium",   "Pakaian",    180000, "Hub Jakarta");
    binarySearchIndex.addProduct(202, "Kaos Polos Cotton Combed","Pakaian",     75000, "Sub-Hub Bekasi");
    binarySearchIndex.addProduct(203, "Jaket Bomber Waterproof", "Pakaian",    220000, "Hub Surabaya");
    binarySearchIndex.addProduct(104, "Kursi Gaming Ergonomis",  "Perabotan", 1200000, "Hub Surabaya");
    binarySearchIndex.addProduct(302, "Lampu Belajar LED Mini",  "Perabotan",   45000, "Hub Jakarta");
    binarySearchIndex.addProduct(303, "Meja Lipat Minimalis",    "Perabotan",  150000, "Hub Merauke");
    binarySearchIndex.buildIndex(); // BUILD INDEX: sort array sebelum pencarian

    // ─ No.9: Hash Table — 3 tabel hash untuk O(1) lookup
    // 1. Product Hash Table: key=ID produk, lookup produk by ID dalam O(1)
    // 2. Voucher Hash Table: key=kode voucher, lookup diskon dalam O(1)
    // 3. Order Hash Table: key=ID paket, lookup status pesanan dalam O(1)

    // --- 1. Product Hash Table ────────────────────────────────────
    // Indexing 9 produk katalog untuk quick lookup saat admin manage produk
    productHashTable.insert(101, "Keyboard Mechanical RGB", "Elektronik", 450000, "Hub Jakarta");
    productHashTable.insert(102, "Mouse Gaming Wireless",   "Elektronik", 250000, "Hub Surabaya");
    productHashTable.insert(103, "Headset Gaming Stereo",   "Elektronik", 350000, "Hub Merauke");
    productHashTable.insert(201, "Kemeja Flanel Premium",   "Pakaian",    180000, "Hub Jakarta");
    productHashTable.insert(202, "Kaos Polos Cotton Combed","Pakaian",     75000, "Sub-Hub Bekasi");
    productHashTable.insert(203, "Jaket Bomber Waterproof", "Pakaian",    220000, "Hub Surabaya");
    productHashTable.insert(104, "Kursi Gaming Ergonomis",  "Perabotan", 1200000, "Hub Surabaya");
    productHashTable.insert(302, "Lampu Belajar LED Mini",  "Perabotan",   45000, "Hub Jakarta");
    productHashTable.insert(303, "Meja Lipat Minimalis",    "Perabotan",  150000, "Hub Merauke");

    // --- 2. Voucher Hash Table ────────────────────────────────────
    // 5 kode voucher + persentase diskon untuk validasi real-time saat input diskon
    // Diakses via cariKodeDiskon(kode) — return pair<bool, KodeDiskon*> untuk O(1) lookup
    voucherHashTable.insert("CCART50",   50,  "Diskon 50% semua produk");
    voucherHashTable.insert("HEMAT20",   20,  "Diskon 20% semua produk");
    voucherHashTable.insert("WELCOME10", 10,  "Diskon 10% untuk pelanggan baru");
    voucherHashTable.insert("FLASH30",   30,  "Flash sale diskon 30%");
    voucherHashTable.insert("GRATIS",    100, "Gratis 100% (produk + ongkir)");

    // --- 3. Order Hash Table ────────────────────────────────────
    // Kosong saat startup, diisi otomatis tiap checkout (klikCheckout)
    // Sinkronisasi dari databasePelacakan untuk quick lookup by ID paket

    // ─ No.10: Graph — Adjacency List untuk Jaringan Logistik
    // Nodes: Hub Jakarta, Sub-Hub Bekasi, Hub Surabaya, Hub Merauke
    // Edges: route dengan waktu tempuh (hours) — symmetric (bolak-balik sama)
    // Digunakan untuk BFS (shortest hop path) dan DFS (connectivity check)
    // Waktu realistis sesuai dengan kenyataan marketplace Indonesia
    petaLogistik.addRoute("Hub Jakarta",    "Sub-Hub Bekasi",  24);  // 1 hari (adjacent cities)
    petaLogistik.addRoute("Sub-Hub Bekasi", "Hub Jakarta",     24);  // 1 hari
    petaLogistik.addRoute("Hub Jakarta",    "Hub Surabaya",    72);  // 3 hari
    petaLogistik.addRoute("Hub Surabaya",   "Hub Jakarta",     72);  // 3 hari
    petaLogistik.addRoute("Hub Surabaya",   "Hub Merauke",    168);  // 7 hari (kargo Papua)
    petaLogistik.addRoute("Hub Merauke",    "Hub Surabaya",   168);  // 7 hari (kargo maritime/air)

    // Display network info di text area grafis (optional educational view)
    txtGraphNetwork->setText(QString::fromStdString(petaLogistik.getNetworkString()));

    // ─ STARTUP SEQUENCE ────────────────────────────────────────────
    // 1. Mulai di halaman login (rootStack index 0)
    // 2. Setelah login, pindah ke shell utama (rootStack index 1)
    // 3. Default ke halaman: Katalog (customer) atau Dashboard Admin (admin)
    rootStack->setCurrentIndex(0);  // tampilkan login page dulu
    this->setWindowTitle("CSHOP — Sistem Logistik");
    this->resize(1000, 680);  // ukuran window default
}

// ====================================================================
// DESTRUCTOR — Cleanup Resource
// ====================================================================
// Dipanggil otomatis saat application ditutup atau MainWindow didelete
// Pembersihan pointer ui yang di-allocate di designer file
MainWindow::~MainWindow() { delete ui; }

// ====================================================================
// HELPER: Format Rute Visual untuk Display
// ====================================================================
// Fungsi: Konversi vector rute (dari getJalurHub) menjadi string visual
// Input: asal (hub awal), tujuan (hub akhir)
// Output: String dengan format "Hub1 ➔ Hub2 ➔ Hub3 ➔ ... ➔ HubN"
// Contoh: "Hub Jakarta ➔ Hub Surabaya ➔ Hub Merauke"
// Digunakan untuk tampilan tracking pesanan & admin dashboard
QString MainWindow::getRuteVisual(const QString& asal, const QString& tujuan) {
    auto jalur = getJalurHub(asal, tujuan);  // ambil vector rute (BFS path)
    QStringList parts;
    for (auto& h : jalur) parts << h;        // konversi setiap hub ke QString
    return parts.join(" ➔ ");               // gabung dengan separator visual
}

// ====================================================================
// LOGIN / LOGOUT
// ====================================================================
void MainWindow::klikLogin() {
    QString user = txtLoginUser->text().trimmed();
    QString pass = txtLoginPass->text();

    // Tabel kredensial sederhana
    struct Akun { QString user; QString pass; UserRole role; };
    std::vector<Akun> db = {
                            {"pelanggan1", "pass123", UserRole::PELANGGAN},
                            {"pelanggan2", "pass456", UserRole::PELANGGAN},
                            {"admin",      "admin123", UserRole::ADMIN},
                            };

    UserRole foundRole = UserRole::NONE;
    for (auto& a : db) {
        if (a.user == user && a.pass == pass) { foundRole = a.role; break; }
    }

    if (foundRole == UserRole::NONE) {
        QMessageBox::warning(this, "Login Gagal", "Username atau password salah!");
        return;
    }

    roleAktif     = foundRole;
    usernameAktif = user.toStdString();

    // Set label selamat datang
    QString roleStr = (foundRole == UserRole::ADMIN) ? "Admin 🔧" : "Pelanggan 👤";
    lblWelcome->setText("Halo, " + user + "\n(" + roleStr + ")");

    // Atur visibilitas tombol sidebar sesuai role
    bool isAdmin = (foundRole == UserRole::ADMIN);
    btnKatalog->setVisible(!isAdmin);        // katalog hanya untuk pelanggan
    btnKeranjang->setVisible(!isAdmin);      // keranjang hanya untuk pelanggan
    btnAntrean->setVisible(isAdmin);
    btnRute->setVisible(isAdmin);
    btnAdminDashboard->setVisible(isAdmin);
    btnManajemenProduk->setVisible(isAdmin);
    btnHashLookup->setVisible(isAdmin);      // hash lookup hanya untuk admin
    btnTrackPelanggan->setVisible(!isAdmin);

    txtLoginUser->clear();
    txtLoginPass->clear();
    rootStack->setCurrentIndex(1);

    // Arahkan ke halaman default sesuai role
    if (isAdmin) {
        stackedWidget->setCurrentIndex(4); // langsung ke Dashboard Admin
        refreshDashboardAdmin();
    } else {
        stackedWidget->setCurrentIndex(0); // pelanggan ke Katalog
        refreshTampilanKatalog();
    }
}

void MainWindow::klikLogout() {
    roleAktif     = UserRole::NONE;
    usernameAktif = "";
    navStack.clear();                 // reset Stack riwayat navigasi
    pilihanInstanPerGudang.clear();   // reset pilihan layanan per paket
    btnBack->setEnabled(false);
    btnBack->setText("⬅ Back");
    rootStack->setCurrentIndex(0);
}

// ====================================================================
// NAVIGASI — dengan Stack Riwayat Halaman (No.4)
// ====================================================================

// Helper: push halaman saat ini ke stack sebelum pindah
static const char* namaHalaman(int idx) {
    switch(idx) {
    case 0: return "Katalog Produk";
    case 1: return "Keranjang Belanja";
    case 2: return "Antrean Pesanan";
    case 3: return "Jaringan Rute";
    case 4: return "Dashboard Admin";
    case 5: return "Lacak Paket";
    case 6: return "Kelola Produk";
    case 7: return "Quick Lookup (Hash)";
    default: return "Halaman";
    }
}

void MainWindow::navigasiHalaman() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // Blokir akses katalog & keranjang untuk admin
    if (roleAktif == UserRole::ADMIN &&
        (btn == btnKatalog || btn == btnKeranjang)) {
        QMessageBox::warning(this, "Akses Ditolak",
                             "Fitur Katalog dan Keranjang tidak tersedia untuk Admin.\n"
                             "Gunakan menu Kelola Produk untuk manajemen produk.");
        return;
    }

    // Push halaman saat ini ke Stack sebelum pindah
    int currentIdx = stackedWidget->currentIndex();
    navStack.push(currentIdx, namaHalaman(currentIdx));
    btnBack->setEnabled(true);
    btnBack->setText(QString("⬅ Back (%1)").arg(QString::fromStdString(navStack.peekLabel())));

    if (btn == btnKatalog)        { stackedWidget->setCurrentIndex(0); refreshTampilanKatalog(); }
    else if (btn == btnKeranjang) { stackedWidget->setCurrentIndex(1); refreshTampilanKeranjang(); }
    else if (btn == btnTrackPelanggan) { stackedWidget->setCurrentIndex(5); refreshTrackingPelanggan(); }
}

void MainWindow::navigasiAdmin() {
    if (roleAktif != UserRole::ADMIN) {
        QMessageBox::warning(this, "Akses Ditolak", "Halaman ini hanya untuk Admin.");
        return;
    }
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // Push halaman saat ini ke Stack sebelum pindah
    int currentIdx = stackedWidget->currentIndex();
    navStack.push(currentIdx, namaHalaman(currentIdx));
    btnBack->setEnabled(true);
    btnBack->setText(QString("⬅ Back (%1)").arg(QString::fromStdString(navStack.peekLabel())));

    if (btn == btnAntrean)              { stackedWidget->setCurrentIndex(2); refreshTampilanAntrean(); }
    else if (btn == btnRute)            { stackedWidget->setCurrentIndex(3); }
    else if (btn == btnAdminDashboard)  { stackedWidget->setCurrentIndex(4); refreshDashboardAdmin(); }
    else if (btn == btnManajemenProduk) { stackedWidget->setCurrentIndex(6); refreshTabelProdukAdmin(); }
    else if (btn == btnHashLookup)      { stackedWidget->setCurrentIndex(7); }
}

// Stack Pop: kembali ke halaman sebelumnya
void MainWindow::klikBack() {
    if (navStack.isEmpty()) return;
    int prevIdx = navStack.pop();

    // Admin tidak boleh kembali ke katalog (0) atau keranjang (1)
    if (roleAktif == UserRole::ADMIN && (prevIdx == 0 || prevIdx == 1)) {
        prevIdx = 4; // arahkan ke Dashboard Admin
    }

    stackedWidget->setCurrentIndex(prevIdx);

    // Refresh konten halaman tujuan
    switch(prevIdx) {
    case 0: refreshTampilanKatalog();   break;
    case 1: refreshTampilanKeranjang(); break;
    case 2: refreshTampilanAntrean();   break;
    case 4: refreshDashboardAdmin();    break;
    case 5: refreshTrackingPelanggan(); break;
    default: break;
    }

    // Update tombol Back
    if (navStack.isEmpty()) {
        btnBack->setEnabled(false);
        btnBack->setText("⬅ Back");
    } else {
        btnBack->setText(QString("⬅ Back (%1)").arg(QString::fromStdString(navStack.peekLabel())));
    }
}

// ====================================================================
// KATALOG
// ====================================================================
void MainWindow::gantiKategoriKatalog() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    btnKatElektronik->setStyleSheet("background:#ecf0f1; color:black; min-height:25px;");
    btnKatPakaian->setStyleSheet("background:#ecf0f1; color:black; min-height:25px;");
    btnKatPerabotan->setStyleSheet("background:#ecf0f1; color:black; min-height:25px;");
    if (btn == btnKatElektronik) { kategoriAktif="Elektronik"; btn->setStyleSheet("background:#34495e; color:white; font-weight:bold; min-height:25px;"); }
    else if (btn == btnKatPakaian)   { kategoriAktif="Pakaian";   btn->setStyleSheet("background:#34495e; color:white; font-weight:bold; min-height:25px;"); }
    else if (btn == btnKatPerabotan) { kategoriAktif="Perabotan"; btn->setStyleSheet("background:#34495e; color:white; font-weight:bold; min-height:25px;"); }
    refreshTampilanKatalog();
}

void MainWindow::refreshTampilanKatalog() {
    ProductNode* cur = nullptr;
    if (kategoriAktif=="Elektronik") cur = katalogElektronikDLL.getCurrentNode();
    else if (kategoriAktif=="Pakaian")   cur = katalogPakaianDLL.getCurrentNode();
    else if (kategoriAktif=="Perabotan") cur = katalogPerabotanDLL.getCurrentNode();
    if (cur) {
        lblProductIdVal->setText(QString::number(cur->id));
        lblProductNameVal->setText(QString::fromStdString(cur->name));
        lblProductCategoryVal->setText(QString::fromStdString(cur->category));
        lblProductPriceVal->setText("Rp " + QString::number(cur->price));
        lblPotonganDiskon->setText(QString::fromStdString(cur->origin));
    } else {
        lblProductIdVal->setText("-"); lblProductNameVal->setText("Katalog Kosong");
        lblProductCategoryVal->setText("-"); lblProductPriceVal->setText("Rp 0"); lblPotonganDiskon->setText("-");
    }
}

void MainWindow::klikNextProduk() {
    if (kategoriAktif=="Elektronik") katalogElektronikDLL.moveNext();
    else if (kategoriAktif=="Pakaian")   katalogPakaianDLL.moveNext();
    else if (kategoriAktif=="Perabotan") katalogPerabotanDLL.moveNext();
    refreshTampilanKatalog();
}

void MainWindow::klikPrevProduk() {
    if (kategoriAktif=="Elektronik") katalogElektronikDLL.movePrev();
    else if (kategoriAktif=="Pakaian")   katalogPakaianDLL.movePrev();
    else if (kategoriAktif=="Perabotan") katalogPerabotanDLL.movePrev();
    refreshTampilanKatalog();
}

void MainWindow::klikTambahKeranjang() {
    ProductNode* p = nullptr;
    if (kategoriAktif=="Elektronik") p = katalogElektronikDLL.getCurrentNode();
    else if (kategoriAktif=="Pakaian")   p = katalogPakaianDLL.getCurrentNode();
    else if (kategoriAktif=="Perabotan") p = katalogPerabotanDLL.getCurrentNode();
    if (p) {
        keranjangSLL.insertItem(p->id, p->name, p->price, p->origin);
        QMessageBox::information(this, "Berhasil", QString("'%1' ditambahkan ke keranjang.").arg(QString::fromStdString(p->name)));
        refreshTampilanKeranjang();
    }
}

// ====================================================================
// KERANJANG
// ====================================================================
void MainWindow::refreshTampilanKeranjang() {
    listCart->clear();
    std::vector<std::string> rawItems   = keranjangSLL.getAllItemsString();
    std::vector<std::string> semuaAsal  = keranjangSLL.getAllOrigins();
    QString kotaTujuan = comboWilayahHub->currentText().trimmed();
    long totalAwal = keranjangSLL.calculateTotal();

    // Kumpulkan daftar gudang unik beserta jam tempuh
    std::map<std::string,int> gudangJam;
    for (const auto& asal : semuaAsal) {
        if (gudangJam.find(asal) == gudangJam.end()) {
            int jam = hitungWaktuDuaArah(QString::fromStdString(asal).trimmed(), kotaTujuan);
            gudangJam[asal] = jam;
        }
    }

    // Inisialisasi default untuk gudang baru (reguler)
    // dan bersihkan gudang yang tidak ada lagi di keranjang
    std::map<std::string,bool> filtered;
    for (auto& [gudang, jam] : gudangJam) {
        if (pilihanInstanPerGudang.find(gudang) != pilihanInstanPerGudang.end())
            filtered[gudang] = pilihanInstanPerGudang[gudang];
        else
            filtered[gudang] = false; // default reguler
    }
    pilihanInstanPerGudang = filtered;

    bool adaDekat = false;
    for (auto& [gudang, jam] : gudangJam)
        if (jam <= 24) { adaDekat = true; break; }

    if (!adaDekat) lblOpsiKirimTitle->setText("📦 Semua Paket: Kargo Jauh");
    else           lblOpsiKirimTitle->setText("⚡ Layanan Pengiriman:");

    // Hitung total ongkir berdasarkan pilihan per gudang
    long ongkir = 0;
    QString ringkasan = "";
    for (auto& [gudang, jam] : gudangJam) {
        bool instan = (jam <= 24) && pilihanInstanPerGudang[gudang];
        if (jam > 24) {
            ongkir += 40000;
            ringkasan += QString("📦 %1 → KARGO JAUH +Rp40.000 | ").arg(QString::fromStdString(gudang));
        } else if (instan) {
            ongkir += 25000;
            ringkasan += QString("⚡ %1 → INSTAN +Rp25.000 | ").arg(QString::fromStdString(gudang));
        } else {
            ongkir += 10000;
            ringkasan += QString("📦 %1 → REGULER +Rp10.000 | ").arg(QString::fromStdString(gudang));
        }
    }
    if (ringkasan.endsWith(" | ")) ringkasan.chop(3);
    lblRingkasanLayanan->setText(rawItems.empty() ? "Belum ada produk di keranjang." : ringkasan);

    // Hitung diskon real-time
    QString kode = txtKodeDiskon->text().trimmed().toUpper();
    auto [valid, diskon] = cariKodeDiskon(kode);
    long potonganProduk = 0, potonganOngkir = 0;
    QString infoDiskon = "";
    if (!kode.isEmpty() && valid) {
        if (diskon->persen == 100) {
            potonganProduk = totalAwal; potonganOngkir = ongkir;
            infoDiskon = QString("  [Kode %1: gratis semua]").arg(diskon->kode);
        } else {
            potonganProduk = totalAwal * diskon->persen / 100;
            infoDiskon = QString("  [Kode %1: -Rp %2]").arg(diskon->kode).arg(potonganProduk);
        }
        txtKodeDiskon->setStyleSheet("padding:4px; border:1px solid #27ae60; border-radius:4px; color:#27ae60; font-weight:bold;");
    } else if (!kode.isEmpty()) {
        infoDiskon = "  [Kode tidak valid]";
        txtKodeDiskon->setStyleSheet("padding:4px; border:1px solid #e74c3c; border-radius:4px; color:#e74c3c;");
    } else {
        txtKodeDiskon->setStyleSheet("");
    }
    long totalAkhir = (totalAwal - potonganProduk) + (ongkir - potonganOngkir);
    if (totalAkhir < 0) totalAkhir = 0;

    lblTotalBelanja->setText(QString("Total Produk: Rp %1 | Ongkir: Rp %2\nTotal Akhir: Rp %3%4")
                                 .arg(totalAwal).arg(ongkir).arg(totalAkhir).arg(infoDiskon));

    for (size_t i = 0; i < rawItems.size(); ++i) {
        std::string asal = semuaAsal[i];
        int jam = gudangJam.count(asal) ? gudangJam[asal] : 99;
        bool instan = (jam <= 24) && pilihanInstanPerGudang.count(asal) && pilihanInstanPerGudang[asal];
        QString label = (jam>24) ? "[📦 KARGO JAUH +Rp40.000]" :
                            (instan ? "[⚡ INSTAN +Rp25.000]" : "[📦 REGULER +Rp10.000]");
        listCart->addItem(QString::fromStdString(rawItems[i]) + " | " + label);
    }
    lblHasilBFS->setText(rawItems.empty() ? "Keranjang kosong." : "ℹ️ Pesanan akan di-split per gudang asal saat checkout.");
}

void MainWindow::klikHapusCart() {
    int idx = listCart->currentRow();
    if (idx >= 0) {
        keranjangSLL.deleteNodeAt(idx);
        refreshTampilanKeranjang();
    } else {
        QMessageBox::warning(this, "Peringatan", "Pilih item terlebih dahulu!");
    }
}

void MainWindow::klikAturLayananPerPaket() {
    if (keranjangSLL.isEmpty()) {
        QMessageBox::warning(this, "Keranjang Kosong", "Tambahkan produk ke keranjang terlebih dahulu.");
        return;
    }

    QString kotaTujuan = comboWilayahHub->currentText().trimmed();
    auto semuaAsal = keranjangSLL.getAllOrigins();

    // Kumpulkan gudang unik dan cek apakah bisa instan
    std::map<std::string,int> gudangJam;
    for (const auto& asal : semuaAsal) {
        if (gudangJam.find(asal) == gudangJam.end()) {
            int jam = hitungWaktuDuaArah(QString::fromStdString(asal).trimmed(), kotaTujuan);
            gudangJam[asal] = jam;
        }
    }

    // Buat dialog pemilihan per sub-paket
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("⚙ Pilih Layanan Pengiriman Per Sub-Paket");
    dlg->setMinimumWidth(520);
    dlg->setStyleSheet(
        "QDialog { background:#1a2533; }"
        "QLabel { color:#ecf0f1; }"
        "QGroupBox { color:#16a085; font-weight:bold; border:2px solid #16a085; border-radius:6px; margin-top:8px; padding-top:6px; }"
        "QRadioButton { color:#ecf0f1; font-size:12px; padding:4px; }"
        "QRadioButton:hover { color:#1abc9c; }"
        "QPushButton { background:#2c3e50; color:#ecf0f1; border:1px solid #34495e; padding:8px 18px; border-radius:5px; font-weight:bold; }"
        "QPushButton:hover { background:#34495e; }"
        );
    QVBoxLayout *lay = new QVBoxLayout(dlg);

    QLabel *lblJudul = new QLabel(
        QString("Keranjang Anda memiliki <b>%1 sub-paket</b> dari gudang berbeda.<br>"
                "Pilih layanan pengiriman untuk masing-masing sub-paket:")
            .arg((int)gudangJam.size()), dlg);
    lblJudul->setWordWrap(true);
    lblJudul->setStyleSheet("color:#ecf0f1; background:#243447; padding:10px; border-radius:5px; margin-bottom:6px;");
    lay->addWidget(lblJudul);

    // Map gudang → radiobutton instan (agar bisa dibaca saat OK)
    std::map<std::string, QRadioButton*> radInstanMap;
    std::map<std::string, QRadioButton*> radRegulerMap;

    for (auto& [gudang, jam] : gudangJam) {
        QGroupBox *grp = new QGroupBox(
            QString("📦 Sub-Paket dari: %1  (estimasi %2 jam ke %3)")
                .arg(QString::fromStdString(gudang))
                .arg(jam)
                .arg(kotaTujuan),
            dlg);
        QVBoxLayout *grpLay = new QVBoxLayout(grp);

        if (jam > 24) {
            // Rute jauh: hanya kargo, tidak bisa instan
            QLabel *lblOnly = new QLabel("📦 Kargo Jauh — hanya tersedia reguler (+Rp 40.000)\nTidak mendukung pengiriman instan.", grp);
            lblOnly->setStyleSheet("color:#95a5a6; font-style:italic; padding:4px;");
            grpLay->addWidget(lblOnly);
        } else {
            bool currentInstan = pilihanInstanPerGudang.count(gudang) ? pilihanInstanPerGudang[gudang] : false;

            QRadioButton *radR = new QRadioButton("📦 Reguler Standard  (+Rp 10.000)", grp);
            QRadioButton *radI = new QRadioButton("⚡ Instan Kilat  (+Rp 25.000)", grp);
            radR->setChecked(!currentInstan);
            radI->setChecked(currentInstan);
            grpLay->addWidget(radR);
            grpLay->addWidget(radI);

            radInstanMap[gudang]  = radI;
            radRegulerMap[gudang] = radR;
        }
        lay->addWidget(grp);
    }

    // Tombol cepat: Semua Instan / Semua Reguler
    bool adaYangBisaInstan = false;
    for (auto& [g, j] : gudangJam) if (j <= 24) { adaYangBisaInstan = true; break; }

    if (adaYangBisaInstan) {
        QHBoxLayout *quickLay = new QHBoxLayout();
        QPushButton *btnSemuaInstan   = new QPushButton("⚡ Semua Instan",   dlg);
        QPushButton *btnSemuaReguler  = new QPushButton("📦 Semua Reguler",  dlg);
        btnSemuaInstan->setStyleSheet("background:#16a085; color:white; font-weight:bold; padding:6px 14px; border-radius:5px;");
        btnSemuaReguler->setStyleSheet("background:#2c3e50; color:#ecf0f1; padding:6px 14px; border-radius:5px;");
        quickLay->addWidget(btnSemuaInstan);
        quickLay->addWidget(btnSemuaReguler);
        quickLay->addStretch();
        lay->addLayout(quickLay);

        connect(btnSemuaInstan, &QPushButton::clicked, dlg, [&radInstanMap](){
            for (auto& [g, rb] : radInstanMap) if (rb) rb->setChecked(true);
        });
        connect(btnSemuaReguler, &QPushButton::clicked, dlg, [&radRegulerMap](){
            for (auto& [g, rb] : radRegulerMap) if (rb) rb->setChecked(true);
        });
    }

    QHBoxLayout *btnLay = new QHBoxLayout();
    QPushButton *btnOK     = new QPushButton("✅ Simpan Pilihan", dlg);
    QPushButton *btnBatal  = new QPushButton("Batal", dlg);
    btnOK->setStyleSheet("background:#2ecc71; color:white; font-weight:bold; padding:8px 20px; border-radius:5px;");
    btnLay->addStretch();
    btnLay->addWidget(btnBatal);
    btnLay->addWidget(btnOK);
    lay->addLayout(btnLay);

    connect(btnBatal, &QPushButton::clicked, dlg, &QDialog::reject);
    connect(btnOK,    &QPushButton::clicked, dlg, [&, dlg](){
        for (auto& [gudang, jam] : gudangJam) {
            if (jam <= 24 && radInstanMap.count(gudang))
                pilihanInstanPerGudang[gudang] = radInstanMap[gudang]->isChecked();
            else
                pilihanInstanPerGudang[gudang] = false;
        }
        dlg->accept();
    });

    if (dlg->exec() == QDialog::Accepted)
        refreshTampilanKeranjang();
}

void MainWindow::klikCheckout() {
    if (keranjangSLL.isEmpty()) { QMessageBox::warning(this, "Gagal", "Keranjang kosong!"); return; }

    QString kotaTujuan = comboWilayahHub->currentText().trimmed();

    std::map<std::string, std::vector<size_t>> kelompok;
    auto semuaAsal  = keranjangSLL.getAllOrigins();
    auto semuaNama  = keranjangSLL.getAllNames();
    auto semuaHarga = keranjangSLL.getAllPrices();
    for (size_t i = 0; i < semuaAsal.size(); ++i) kelompok[semuaAsal[i]].push_back(i);

    for (auto const& [asalStd, idxList] : kelompok) {
        QString asal = QString::fromStdString(asalStd).trimmed();
        int jam = hitungWaktuDuaArah(asal, kotaTujuan);

        // Ambil pilihan instan dari pilihanInstanPerGudang (sudah diset via dialog)
        bool instan = (jam <= 24) && pilihanInstanPerGudang.count(asalStd) && pilihanInstanPerGudang[asalStd];
        long ongkir = (jam>24) ? 40000 : (instan ? 25000 : 10000);
        std::string labelKurir = (jam>24) ? "[📦 KARGO JAUH +Rp40.000]" : (instan ? "[⚡ INSTAN +Rp25.000]" : "[📦 REGULER +Rp10.000]");

        std::string nota = "===== SUB-PAKET " + labelKurir + " =====\n"
                           + "🏭 Asal Gudang : " + asalStd + "\n"
                           + "📍 Tujuan      : " + kotaTujuan.toStdString() + "\n"
                           + "-------------------------------------------\n";
        long totalProduk = 0;
        for (size_t i : idxList) {
            nota += "- " + semuaNama[i] + " (Rp " + std::to_string(semuaHarga[i]) + ")\n";
            totalProduk += semuaHarga[i];
        }
        long potongan = 0, potonganOngkirLokal = 0;
        QString kode2 = txtKodeDiskon->text().trimmed().toUpper();
        auto [valid2, diskon2] = cariKodeDiskon(kode2);
        if (valid2) {
            if (diskon2->persen == 100) {
                potongan = totalProduk; potonganOngkirLokal = ongkir;
                nota += "Diskon " + diskon2->kode.toStdString() + ": GRATIS semua\n";
            } else {
                potongan = totalProduk * diskon2->persen / 100;
                nota += "Diskon " + diskon2->kode.toStdString() + " (" + std::to_string(diskon2->persen) + "%): -Rp " + std::to_string(potongan) + "\n";
            }
        }
        long totalAkhir = (totalProduk - potongan) + (ongkir - potonganOngkirLokal);
        if (totalAkhir < 0) totalAkhir = 0;
        nota += "-------------------------------------------\n"
                + std::string("Ongkir: Rp ") + std::to_string(ongkir - potonganOngkirLokal) + "\n"
                + "Total Bayar: Rp " + std::to_string(totalAkhir) + "\n"
                + "Estimasi Waktu: " + std::to_string(jam) + " Jam\n";

        std::string idPaket = "PKT-" + std::to_string(rand()%90000+10000);
        antreanQueue.enqueueOrderPriority(idPaket, totalAkhir, nota, instan, kotaTujuan.toStdString(), asalStd, usernameAktif);

        // Langsung masuk databasePelacakan agar pelanggan bisa lacak tanpa perlu tunggu admin proses
        ActiveOrder o;
        o.idPelanggan        = idPaket;
        o.namaPelanggan      = usernameAktif;
        o.detailNota         = nota;
        o.kotaAsal           = asalStd;
        o.kotaTujuan         = kotaTujuan.toStdString();
        o.isInstant          = instan;
        o.statusLogistik     = "Menunggu Konfirmasi Admin";
        o.sisaWaktuJam       = jam;
        o.lokasiSekarang     = asalStd;
        o.lastAdminUpdate    = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss").toStdString();
        o.sudahDiUpdateAdmin = true;
        databasePelacakan.push_back(o);

        // Sinkron ke Order Hash Table untuk quick lookup O(1)
        orderHashTable.upsert(idPaket, usernameAktif, asalStd,
                              kotaTujuan.toStdString(),
                              "Menunggu Konfirmasi Admin", asalStd,
                              jam, instan,
                              o.lastAdminUpdate);
    }

    keranjangSLL.clearList();
    pilihanInstanPerGudang.clear();
    txtKodeDiskon->clear();
    refreshTampilanKeranjang();

    QMessageBox::information(this, "Checkout Berhasil",
                             QString("%1 sub-paket berhasil dipesan!\nCek status di menu Lacak Paket Saya.").arg((int)kelompok.size()));
}

// ====================================================================
// ANTREAN (ADMIN)
// ====================================================================
void MainWindow::refreshTampilanAntrean() {
    listQueue->clear();
    for (auto& s : antreanQueue.getQueueSummary())
        listQueue->addItem(QString::fromStdString(s));
    std::string detail = antreanQueue.peekFrontDetail();
    txtDetailAntrean->setText(detail.empty()
                                  ? "=== Antrean Kosong ==="
                                  : QString::fromStdString(detail));
}

void MainWindow::klikProsesOrderAdmin() {
    if (roleAktif != UserRole::ADMIN) { QMessageBox::warning(this,"Akses Ditolak","Hanya admin."); return; }
    if (antreanQueue.isEmpty()) { QMessageBox::warning(this,"Kosong","Antrean paket kosong."); return; }

    std::string idPaket = antreanQueue.peekFrontId();   // ambil ID paket dari antrian
    std::string tujuan  = antreanQueue.peekFrontDestination();
    std::string asal    = antreanQueue.peekFrontOrigin();

    QString kotaAsal   = QString::fromStdString(asal).trimmed();
    QString kotaTujuan = QString::fromStdString(tujuan).trimmed();
    int jamTotal       = hitungWaktuDuaArah(kotaAsal, kotaTujuan);

    // Cari entry yang sudah ada di databasePelacakan (dibuat saat checkout)
    // dan update statusnya — JANGAN buat entry baru agar tidak duplikat
    bool found = false;
    for (auto& o : databasePelacakan) {
        if (o.idPelanggan == idPaket) {
            o.statusLogistik     = "Siap Dikirim dari " + asal;
            o.lokasiSekarang     = asal;
            o.sisaWaktuJam       = jamTotal;
            o.lastAdminUpdate    = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss").toStdString();
            o.sudahDiUpdateAdmin = true;
            found = true;
            break;
        }
    }

    // Fallback: jika karena suatu alasan entry belum ada, baru dibuat
    if (!found) {
        std::string detail    = antreanQueue.peekFrontDetail();
        std::string namaPelanggan = antreanQueue.peekFrontOwner();
        ActiveOrder o;
        o.idPelanggan        = idPaket;
        o.namaPelanggan      = namaPelanggan;
        o.detailNota         = detail;
        o.kotaAsal           = asal;
        o.kotaTujuan         = tujuan;
        o.isInstant          = (detail.find("INSTAN") != std::string::npos);
        o.statusLogistik     = "Siap Dikirim dari " + asal;
        o.sisaWaktuJam       = jamTotal;
        o.lokasiSekarang     = asal;
        o.lastAdminUpdate    = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss").toStdString();
        o.sudahDiUpdateAdmin = true;
        databasePelacakan.push_back(o);
    }

    antreanQueue.dequeue();
    refreshTampilanAntrean();
    refreshDashboardAdmin();

    QMessageBox::information(this, "Paket Diproses",
                             QString("Paket %1 diproses. Estimasi waktu: %2 jam.\n"
                                     "Perbarui lokasi paket di Dashboard Admin.")
                                 .arg(QString::fromStdString(idPaket)).arg(jamTotal));
}

// ====================================================================
// DASHBOARD ADMIN — update status + kurangi waktu per hub
// ====================================================================
void MainWindow::refreshDashboardAdmin() {
    listAdminOrders->clear();
    comboAdminStatus->clear();
    btnAdminUpdate->setEnabled(false);
    txtAdminDetail->clear();

    for (auto& o : databasePelacakan) {
        bool sudahSampai = (o.statusLogistik.find("Sampai") != std::string::npos);
        QString icon = sudahSampai ? "✅" : "🚚";
        QString item = QString("%1 [%2] %3 | %4 → %5 | Sisa: %6 Jam (%7 Hari) | Lokasi: %8")
                           .arg(icon)
                           .arg(QString::fromStdString(o.statusLogistik))
                           .arg(QString::fromStdString(o.idPelanggan))
                           .arg(QString::fromStdString(o.kotaAsal))
                           .arg(QString::fromStdString(o.kotaTujuan))
                           .arg(o.sisaWaktuJam)
                           .arg(double(o.sisaWaktuJam)/24.0, 0, 'f', 1)
                           .arg(QString::fromStdString(o.lokasiSekarang));
        listAdminOrders->addItem(item);
    }
}

void MainWindow::klikUpdateStatusAdmin() {
    if (roleAktif != UserRole::ADMIN) { QMessageBox::warning(this,"Akses Ditolak","Hanya admin."); return; }
    int row = listAdminOrders->currentRow();
    if (row < 0 || row >= (int)databasePelacakan.size()) {
        QMessageBox::warning(this, "Pilih Paket", "Pilih paket dari daftar terlebih dahulu!");
        return;
    }
    if (comboAdminStatus->count() == 0) {
        QMessageBox::information(this, "Tidak Ada Opsi", "Tidak ada hub berikutnya yang tersedia untuk paket ini.");
        return;
    }

    ActiveOrder& o     = databasePelacakan[row];
    QString lokasiLama = QString::fromStdString(o.lokasiSekarang);
    QString lokasiBaru = comboAdminStatus->currentText();

    bool dalamPerjalanan = (lokasiBaru == "🚚 Sedang Dalam Perjalanan");

    // Otomatis dianggap "Sampai" jika admin langsung memilih hub tujuan akhir,
    // tidak perlu lagi pilih opsi "✅ Paket Telah Sampai Tujuan" secara terpisah.
    // Opsi manual tetap disediakan di combo untuk jaga-jaga / override.
    bool sampai = !dalamPerjalanan &&
                  (lokasiBaru.contains("Sampai Tujuan") ||
                   lokasiBaru == QString::fromStdString(o.kotaTujuan));

    // ---- Hitung pengurangan waktu ----
    int pengurangan = 0;
    if (dalamPerjalanan) {
        // Status "sedang dalam perjalanan": lokasi & sisa waktu TIDAK berubah
        pengurangan = 0;
    } else if (sampai) {
        // Habiskan semua sisa waktu
        pengurangan = o.sisaWaktuJam;
    } else {
        // Kurangi waktu tempuh antar hub bertetangga di jalur ini
        pengurangan = waktuAntarHub(lokasiLama, lokasiBaru);
        if (pengurangan == 0 && lokasiLama != lokasiBaru) {
            auto jalur = getJalurHub(lokasiLama, lokasiBaru);
            for (int i = 0; i+1 < (int)jalur.size(); ++i)
                pengurangan += waktuAntarHub(jalur[i], jalur[i+1]);
        }
    }

    o.sisaWaktuJam -= pengurangan;
    if (o.sisaWaktuJam < 0) o.sisaWaktuJam = 0;

    // ---- Update status & lokasi ----
    if (dalamPerjalanan) {
        o.statusLogistik = "Sedang Dalam Perjalanan dari " + o.lokasiSekarang;
        // lokasiSekarang & sisaWaktuJam tidak berubah
    } else if (sampai) {
        o.lokasiSekarang = o.kotaTujuan;
        o.statusLogistik = "Paket Telah Sampai Tujuan ✅";
        o.sisaWaktuJam   = 0;
    } else {
        o.lokasiSekarang = lokasiBaru.toStdString();
        o.statusLogistik = "Tiba di " + lokasiBaru.toStdString();
    }

    o.lastAdminUpdate    = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss").toStdString();
    o.sudahDiUpdateAdmin = true;

    // Sinkron ke Order Hash Table
    orderHashTable.upsert(o.idPelanggan, o.namaPelanggan, o.kotaAsal, o.kotaTujuan,
                          o.statusLogistik, o.lokasiSekarang,
                          o.sisaWaktuJam, o.isInstant, o.lastAdminUpdate);

    refreshDashboardAdmin();
    listAdminOrders->setCurrentRow(row); // trigger ulang combo refresh

    QString infoWaktu;
    if (dalamPerjalanan) {
        infoWaktu = "Status diperbarui menjadi \"Sedang Dalam Perjalanan\" — sisa waktu tidak berkurang.";
    } else if (pengurangan > 0) {
        infoWaktu = QString("Waktu dikurangi : %1 Jam (%2 Hari)\nSisa Waktu      : %3 Jam (%4 Hari)")
        .arg(pengurangan).arg(double(pengurangan)/24.0, 0,'f',1)
            .arg(o.sisaWaktuJam).arg(double(o.sisaWaktuJam)/24.0, 0,'f',1);
    } else {
        infoWaktu = "Waktu tidak berubah (lokasi sama).";
    }

    QString lokasiTampil = dalamPerjalanan ? QString::fromStdString(o.lokasiSekarang)
                           : sampai          ? QString::fromStdString(o.kotaTujuan)
                                           : lokasiBaru;

    QMessageBox::information(this, "✅ Status Diperbarui",
                             QString("Paket   : %1\nLokasi  : %2\n\n%3")
                                 .arg(QString::fromStdString(o.idPelanggan))
                                 .arg(lokasiTampil)
                                 .arg(infoWaktu));
}

// ====================================================================
// TRACKING PELANGGAN
// ====================================================================
void MainWindow::refreshTrackingPelanggan() {
    listTrackOrders->clear();
    txtTrackDetail->clear();

    // Pisahkan paket aktif dan selesai
    std::vector<ActiveOrder*> aktif, selesai;
    for (auto& o : databasePelacakan) {
        if (o.namaPelanggan != usernameAktif) continue;
        if (o.statusLogistik.find("Sampai") != std::string::npos)
            selesai.push_back(&o);
        else
            aktif.push_back(&o);
    }

    bool ada = !aktif.empty() || !selesai.empty();

    // ── Bagian PAKET AKTIF ──
    if (!aktif.empty()) {
        QListWidgetItem* headerAktif = new QListWidgetItem("─── 🚚 PAKET AKTIF ───────────────────");
        headerAktif->setFlags(Qt::NoItemFlags);          // tidak bisa diklik
        headerAktif->setForeground(QColor("#f39c12"));
        headerAktif->setBackground(QColor("#2c3e50"));
        QFont fh = headerAktif->font(); fh.setBold(true); headerAktif->setFont(fh);
        listTrackOrders->addItem(headerAktif);

        for (auto* o : aktif) {
            QString icon;
            if (o->statusLogistik.find("Menunggu") != std::string::npos) icon = "[DIPROSES]";
            else                                                           icon = "[DIKIRIM] ";
            QListWidgetItem* item = new QListWidgetItem(
                QString("%1 %2 | %3 → %4 | Sisa: %5 Jam")
                    .arg(icon)
                    .arg(QString::fromStdString(o->idPelanggan))
                    .arg(QString::fromStdString(o->kotaAsal))
                    .arg(QString::fromStdString(o->kotaTujuan))
                    .arg(o->sisaWaktuJam));
            item->setForeground(QColor("#2ecc71"));
            listTrackOrders->addItem(item);
        }
    }

    // ── Bagian PAKET SELESAI ──
    if (!selesai.empty()) {
        // Spasi pemisah antar seksi (hanya jika ada paket aktif juga)
        if (!aktif.empty()) {
            QListWidgetItem* spacer = new QListWidgetItem("");
            spacer->setFlags(Qt::NoItemFlags);
            listTrackOrders->addItem(spacer);
        }

        QListWidgetItem* headerSelesai = new QListWidgetItem("─── ✅ SUDAH SELESAI ──────────────────");
        headerSelesai->setFlags(Qt::NoItemFlags);
        headerSelesai->setForeground(QColor("#95a5a6"));
        headerSelesai->setBackground(QColor("#2c3e50"));
        QFont fh2 = headerSelesai->font(); fh2.setBold(true); headerSelesai->setFont(fh2);
        listTrackOrders->addItem(headerSelesai);

        for (auto* o : selesai) {
            QListWidgetItem* item = new QListWidgetItem(
                QString("[SELESAI] %1 | %2 → %3 | Sampai ✅")
                    .arg(QString::fromStdString(o->idPelanggan))
                    .arg(QString::fromStdString(o->kotaAsal))
                    .arg(QString::fromStdString(o->kotaTujuan)));
            item->setForeground(QColor("#7f8c8d"));
            listTrackOrders->addItem(item);
        }
    }

    if (!ada) {
        txtTrackDetail->setText("Belum ada paket aktif untuk akun ini.");
    } else {
        // Auto-pilih item pertama yang bisa diklik (paket aktif jika ada, selesai jika tidak)
        for (int i = 0; i < listTrackOrders->count(); ++i) {
            if (listTrackOrders->item(i)->flags() & Qt::ItemIsEnabled) {
                listTrackOrders->setCurrentRow(i);
                break;
            }
        }
    }
}

// ====================================================================
// ROTASI BANNER PROMO (Circular Linked List)
// ====================================================================
void MainWindow::rotasiPromoBanner() {
    promoBanner.moveNext();
    lblPromoBanner->setText(QString::fromStdString(promoBanner.getTeks()));
}

void MainWindow::klikRefreshTrackingPelanggan() {
    refreshTrackingPelanggan();
    if (listTrackOrders->count() > 0)
        QMessageBox::information(this, "Refresh", QString("Data diperbarui. %1 paket ditemukan.").arg(listTrackOrders->count()));
    else
        QMessageBox::information(this, "Refresh", "Belum ada paket aktif.");
}

// ====================================================================
// URUT HARGA — BST In-Order Traversal (No.7)
// ====================================================================
void MainWindow::klikUrutHarga() {
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("Urut Harga — BST In-Order Traversal"); dlg->setMinimumSize(600, 460);
    dlg->setStyleSheet("QDialog { background:#ffffff; } QLabel { color:#2c3e50; } QRadioButton { color:#2c3e50; font-weight:bold; }");
    QVBoxLayout *lay = new QVBoxLayout(dlg);

    QLabel *lblInfo = new QLabel(
        "📊 Data diurutkan menggunakan <b>BST In-Order Traversal</b>.<br>"
        "Insert produk ke BST berdasarkan harga → In-Order menghasilkan urutan ascending otomatis.",
        dlg);
    lblInfo->setWordWrap(true);
    lblInfo->setStyleSheet("background:#eaf4fb; color:#2c3e50; padding:8px; border-radius:5px; margin-bottom:4px;");
    lay->addWidget(lblInfo);

    QHBoxLayout *radLay = new QHBoxLayout();
    QRadioButton *radAsc  = new QRadioButton("Termurah → Termahal (In-Order)", dlg);
    QRadioButton *radDesc = new QRadioButton("Termahal → Termurah (Reverse In-Order)", dlg);
    radAsc->setStyleSheet("color:#2c3e50; font-weight:bold;");
    radDesc->setStyleSheet("color:#2c3e50; font-weight:bold;");
    radAsc->setChecked(true);
    radLay->addWidget(radAsc); radLay->addWidget(radDesc);
    lay->addLayout(radLay);

    QTableWidget *tbl = new QTableWidget(dlg);
    tbl->setColumnCount(4);
    tbl->setHorizontalHeaderLabels({"ID","Nama Produk","Harga","Aksi"});
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    lay->addWidget(tbl);

    // Tombol lihat struktur BST
    QPushButton *btnLihatBST = new QPushButton("🌳 Lihat Struktur BST", dlg);
    btnLihatBST->setStyleSheet("background:#16a085; color:white; font-weight:bold;");
    connect(btnLihatBST, &QPushButton::clicked, dlg, [this](){
        QDialog *bstDlg = new QDialog(this);
        bstDlg->setWindowTitle("Visualisasi Struktur BST — " + QString::fromStdString(kategoriAktif));
        bstDlg->setMinimumSize(480, 340);
        QVBoxLayout *bstLay = new QVBoxLayout(bstDlg);
        QTextEdit *txtBST = new QTextEdit(bstDlg);
        txtBST->setReadOnly(true);
        txtBST->setFont(QFont("Courier", 10));
        if (kategoriAktif=="Elektronik")   txtBST->setText(QString::fromStdString(bstElektronik.getTreeVisualization()));
        else if (kategoriAktif=="Pakaian") txtBST->setText(QString::fromStdString(bstPakaian.getTreeVisualization()));
        else                               txtBST->setText(QString::fromStdString(bstPerabotan.getTreeVisualization()));
        bstLay->addWidget(txtBST);
        QPushButton *ok = new QPushButton("Tutup", bstDlg);
        connect(ok, &QPushButton::clicked, bstDlg, &QDialog::accept);
        bstLay->addWidget(ok);
        bstDlg->exec();
    });
    lay->addWidget(btnLihatBST);

    // Isi tabel dengan hasil BST traversal
    auto isiTabel = [this, tbl, radAsc, dlg]() {
        tbl->setRowCount(0);
        bool asc = radAsc->isChecked();

        // Gunakan BST (No.7) untuk traversal — bukan std::sort
        std::vector<BSTNode*> data;
        if (kategoriAktif=="Elektronik")
            data = asc ? bstElektronik.getSortedAscending() : bstElektronik.getSortedDescending();
        else if (kategoriAktif=="Pakaian")
            data = asc ? bstPakaian.getSortedAscending()    : bstPakaian.getSortedDescending();
        else
            data = asc ? bstPerabotan.getSortedAscending()  : bstPerabotan.getSortedDescending();

        tbl->setRowCount((int)data.size());
        for (int i = 0; i < (int)data.size(); ++i) {
            tbl->setItem(i,0,new QTableWidgetItem(QString::number(data[i]->id)));
            tbl->setItem(i,1,new QTableWidgetItem(QString::fromStdString(data[i]->name)));
            tbl->setItem(i,2,new QTableWidgetItem("Rp "+QString::number(data[i]->price)));
            QPushButton *btn = new QPushButton("➕ Keranjang", tbl);
            btn->setStyleSheet("background:#2ecc71; color:white; max-height:20px; font-size:11px;");
            BSTNode* p = data[i];
            connect(btn, &QPushButton::clicked, dlg, [this,p](){
                keranjangSLL.insertItem(p->id,p->name,p->price,p->origin);
                QMessageBox::information(nullptr,"Berhasil",QString("'%1' ditambahkan.").arg(QString::fromStdString(p->name)));
                refreshTampilanKeranjang();
            });
            tbl->setCellWidget(i,3,btn);
        }
    };
    isiTabel();
    connect(radAsc,  &QRadioButton::toggled, dlg, isiTabel);
    connect(radDesc, &QRadioButton::toggled, dlg, isiTabel);
    QPushButton *btnTutup = new QPushButton("Tutup", dlg);
    connect(btnTutup, &QPushButton::clicked, dlg, &QDialog::accept);
    lay->addWidget(btnTutup);
    dlg->exec();  // tampilkan dialog modal
    delete dlg;   // cleanup setelah dialog ditutup
}

// ====================================================================
// KELOLA PRODUK (ADMIN) — CRUD Operations
// ====================================================================
// Halaman admin untuk Add/Edit/Delete produk dari katalog
// Produk di-store di: CatalogDLL, BST (per kategori), AVL Tree, Binary Search, Hash Table
// Sinkronisasi otomatis ke semua struktur data saat admin ubah

// ====================================================================
// CARI PRODUK — Binary Search (No.6) + fallback substring
// ====================================================================
void MainWindow::klikCariId() {
    bool ok;
    QString kw = QInputDialog::getText(this, "Pencarian Produk",
                                       "Masukkan Nama atau ID Produk\n(Binary Search by ID / Prefix Search by Nama):",
                                       QLineEdit::Normal, "", &ok);
    if (!ok || kw.trimmed().isEmpty()) return;

    std::string kws = kw.trimmed().toStdString();
    std::vector<ProductEntry> hasilBS;
    std::string logBST;

    // Coba parse sebagai ID (Binary Search by ID)
    bool isNumeric = true;
    for (char c : kws) if (!std::isdigit(c)) { isNumeric = false; break; }

    if (isNumeric) {
        int targetId = std::stoi(kws);
        logBST = binarySearchIndex.getSearchLog(targetId); // tampilkan log proses
        const ProductEntry* found = binarySearchIndex.searchById(targetId);
        if (found) hasilBS.push_back(*found);
    } else {
        // Binary Search by prefix nama
        hasilBS = binarySearchIndex.searchByNamePrefix(kws);
        logBST  = "=== BINARY SEARCH LOG (by Nama Prefix: \"" + kws + "\") ===\n";
        logBST += "Array diurutkan A-Z, cari lower-bound prefix → kumpulkan semua match.\n";
        logBST += "Hasil ditemukan: " + std::to_string(hasilBS.size()) + " produk.";
    }

    if (hasilBS.empty()) {
        // Fallback: linear substring search di semua katalog
        std::vector<ProductNode*> fallback;
        for (auto* r : katalogElektronikDLL.searchProductBySubstring(kws)) fallback.push_back(r);
        for (auto* r : katalogPakaianDLL.searchProductBySubstring(kws))    fallback.push_back(r);
        for (auto* r : katalogPerabotanDLL.searchProductBySubstring(kws))  fallback.push_back(r);

        if (fallback.empty()) {
            QMessageBox::critical(this,"Tidak Ditemukan",
                                  QString("Tidak ada produk dengan kata kunci '%1'.").arg(kw));
            return;
        }
        // Konversi ke ProductEntry untuk tampilan seragam
        for (auto* p : fallback)
            hasilBS.push_back({p->id, p->name, p->category, p->price, p->origin});
        logBST += "\n⚠ Binary Search tidak menemukan prefix persis → fallback ke linear search.";
    }

    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("Hasil Pencarian Binary Search");
    dlg->setMinimumSize(580, 380);
    dlg->setStyleSheet("QDialog { background:#ffffff; } QLabel { color:#2c3e50; }");
    QVBoxLayout *lay = new QVBoxLayout(dlg);

    // Panel log proses Binary Search (edukasi)
    QTextEdit *txtLog = new QTextEdit(dlg);
    txtLog->setReadOnly(true);
    txtLog->setMaximumHeight(100);
    txtLog->setFont(QFont("Courier", 9));
    txtLog->setStyleSheet("background:#f4f6f7; color:#2c3e50;");
    txtLog->setText(QString::fromStdString(logBST));
    lay->addWidget(new QLabel("📋 Log Proses Binary Search:", dlg));
    lay->addWidget(txtLog);

    QTableWidget *tbl = new QTableWidget(dlg);
    tbl->setColumnCount(4);
    tbl->setHorizontalHeaderLabels({"Nama","Kategori","Harga","Aksi"});
    tbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    lay->addWidget(tbl);
    tbl->setRowCount((int)hasilBS.size());
    for (int i = 0; i < (int)hasilBS.size(); ++i) {
        tbl->setItem(i,0,new QTableWidgetItem(QString::fromStdString(hasilBS[i].name)));
        tbl->setItem(i,1,new QTableWidgetItem(QString::fromStdString(hasilBS[i].category)));
        tbl->setItem(i,2,new QTableWidgetItem("Rp "+QString::number(hasilBS[i].price)));
        QPushButton *btn = new QPushButton("➕ Tambah", tbl);
        btn->setStyleSheet("background:#3498db; color:white; font-weight:bold;");
        ProductEntry e = hasilBS[i];
        connect(btn, &QPushButton::clicked, dlg, [this,e](){
            keranjangSLL.insertItem(e.id, e.name, e.price, e.origin);
            QMessageBox::information(nullptr,"Berhasil",
                                     QString("'%1' masuk keranjang!").arg(QString::fromStdString(e.name)));
            refreshTampilanKeranjang();
        });
        tbl->setCellWidget(i,3,btn);
    }
    QPushButton *btnOk = new QPushButton("Selesai",dlg);
    connect(btnOk, &QPushButton::clicked, dlg, &QDialog::accept);
    lay->addWidget(btnOk);
    dlg->exec();
}
// ====================================================================
// MANAJEMEN PRODUK — Helper: sync semua struktur data
// ====================================================================

// Helper internal: tambahkan produk ke semua struktur data sekaligus
static void syncInsertAllDS(const std::string& name, const std::string& cat,
                            int id, long price, const std::string& origin,
                            CatalogDLL& elDLL, CatalogDLL& paDLL, CatalogDLL& prDLL,
                            PriceBST& elBST, PriceBST& paBST, PriceBST& prBST,
                            AVLTree& avl, BinarySearch& bs,
                            ProductHashTable& ht)
{
    if      (cat == "Elektronik") { elDLL.insertProduct(id, name, cat, price, origin); elBST.insert(id, name, cat, price, origin); }
    else if (cat == "Pakaian")    { paDLL.insertProduct(id, name, cat, price, origin); paBST.insert(id, name, cat, price, origin); }
    else                          { prDLL.insertProduct(id, name, cat, price, origin); prBST.insert(id, name, cat, price, origin); }
    avl.insert(id, name, cat, price, origin);
    bs.addProduct(id, name, cat, price, origin);
    bs.buildIndex();
    ht.insert(id, name, cat, price, origin); // Hash Table O(1) insert
}

// ====================================================================
// MANAJEMEN PRODUK — Refresh tabel
// ====================================================================
void MainWindow::refreshTabelProdukAdmin() {
    // Kumpulkan dari AVL Tree (sudah terurut by ID)
    auto semua = avlTree.getAllSortedById();
    tabelProdukAdmin->setRowCount((int)semua.size());
    for (int i = 0; i < (int)semua.size(); ++i) {
        tabelProdukAdmin->setItem(i, 0, new QTableWidgetItem(QString::number(semua[i]->id)));
        tabelProdukAdmin->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(semua[i]->name)));
        tabelProdukAdmin->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(semua[i]->category)));
        tabelProdukAdmin->setItem(i, 3, new QTableWidgetItem("Rp " + QString::number(semua[i]->price)));
        tabelProdukAdmin->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(semua[i]->origin)));
    }
    tabelProdukAdmin->clearSelection();
}

// ====================================================================
// MANAJEMEN PRODUK — Tambah
// ====================================================================
void MainWindow::klikTambahProdukAdmin() {
    if (roleAktif != UserRole::ADMIN) return;

    // Validasi input
    QString idStr    = txtProdukId->text().trimmed();
    QString nama     = txtProdukNama->text().trimmed();
    QString kat      = comboProdukKategori->currentText();
    QString hargaStr = txtProdukHarga->text().trimmed();
    QString hub      = comboProdukHub->currentText();

    if (idStr.isEmpty() || nama.isEmpty() || hargaStr.isEmpty()) {
        QMessageBox::warning(this, "Form Tidak Lengkap", "ID, Nama, dan Harga wajib diisi!");
        return;
    }
    bool okId, okHarga;
    int  id    = idStr.toInt(&okId);
    long harga = hargaStr.toLong(&okHarga);
    if (!okId || id <= 0) {
        QMessageBox::warning(this, "ID Tidak Valid", "ID harus berupa angka bulat positif."); return;
    }
    if (!okHarga || harga <= 0) {
        QMessageBox::warning(this, "Harga Tidak Valid", "Harga harus berupa angka positif."); return;
    }

    // Cek ID sudah ada di AVL Tree
    if (avlTree.search(id) != nullptr) {
        QMessageBox::warning(this, "ID Duplikat",
                             QString("Produk dengan ID %1 sudah ada.\nGunakan Update untuk mengubah data, atau pakai ID lain.").arg(id));
        return;
    }

    std::string sNama = nama.toStdString();
    std::string sKat  = kat.toStdString();
    std::string sHub  = hub.toStdString();

    syncInsertAllDS(sNama, sKat, id, harga, sHub,
                    katalogElektronikDLL, katalogPakaianDLL, katalogPerabotanDLL,
                    bstElektronik, bstPakaian, bstPerabotan,
                    avlTree, binarySearchIndex, productHashTable);

    refreshTabelProdukAdmin();
    refreshTampilanKatalog();

    // Bersihkan form
    txtProdukId->clear(); txtProdukNama->clear(); txtProdukHarga->clear();
    comboProdukKategori->setCurrentIndex(0); comboProdukHub->setCurrentIndex(0);

    QMessageBox::information(this, "Berhasil",
                             QString("✅ Produk '%1' (ID: %2) berhasil ditambahkan ke katalog %3.")
                                 .arg(nama).arg(id).arg(kat));
}

// ====================================================================
// MANAJEMEN PRODUK — Update
// ====================================================================
void MainWindow::klikUpdateProdukAdmin() {
    if (roleAktif != UserRole::ADMIN) return;

    QString idStr    = txtProdukId->text().trimmed();
    QString nama     = txtProdukNama->text().trimmed();
    QString kat      = comboProdukKategori->currentText();
    QString hargaStr = txtProdukHarga->text().trimmed();
    QString hub      = comboProdukHub->currentText();

    if (idStr.isEmpty() || nama.isEmpty() || hargaStr.isEmpty()) {
        QMessageBox::warning(this, "Form Tidak Lengkap", "ID, Nama, dan Harga wajib diisi!"); return;
    }
    bool okId, okHarga;
    int  id    = idStr.toInt(&okId);
    long harga = hargaStr.toLong(&okHarga);
    if (!okId || id <= 0) { QMessageBox::warning(this, "ID Tidak Valid", "ID harus angka bulat positif."); return; }
    if (!okHarga || harga <= 0) { QMessageBox::warning(this, "Harga Tidak Valid", "Harga harus angka positif."); return; }

    // Harus sudah ada
    AVLNode* existing = avlTree.search(id);
    if (!existing) {
        QMessageBox::warning(this, "Tidak Ditemukan",
                             QString("Produk ID %1 tidak ditemukan.\nGunakan Tambah untuk produk baru.").arg(id)); return;
    }

    std::string sNama    = nama.toStdString();
    std::string sKat     = kat.toStdString();
    std::string sHub     = hub.toStdString();
    std::string sKatLama = existing->category;

    // Konfirmasi
    auto jawab = QMessageBox::question(this, "Konfirmasi Update",
                                       QString("Update produk ID %1:\n\nNama  : %2\nKategori: %3\nHarga : Rp %4\nHub   : %5\n\nLanjutkan?")
                                           .arg(id).arg(nama).arg(kat).arg(harga).arg(hub));
    if (jawab != QMessageBox::Yes) return;

    // AVL Tree: update in-place (insert dengan ID sama akan overwrite)
    avlTree.insert(id, sNama, sKat, harga, sHub);

    // CatalogDLL: hapus dari kategori lama (re-insert ke yg baru jika kategori berubah)
    // Untuk kesederhanaan, rebuild seluruh katalog dari AVL Tree
    katalogElektronikDLL.clearCatalog();
    katalogPakaianDLL.clearCatalog();
    katalogPerabotanDLL.clearCatalog();
    bstElektronik.clear();
    bstPakaian.clear();
    bstPerabotan.clear();
    binarySearchIndex.clear();
    productHashTable.clear(); // rebuild hash table juga

    for (auto* node : avlTree.getAllSortedById()) {
        std::string n = node->name, c = node->category, o = node->origin;
        long p = node->price; int nid = node->id;
        if      (c == "Elektronik") { katalogElektronikDLL.insertProduct(nid,n,c,p,o); bstElektronik.insert(nid,n,c,p,o); }
        else if (c == "Pakaian")    { katalogPakaianDLL.insertProduct(nid,n,c,p,o);    bstPakaian.insert(nid,n,c,p,o); }
        else                        { katalogPerabotanDLL.insertProduct(nid,n,c,p,o);  bstPerabotan.insert(nid,n,c,p,o); }
        binarySearchIndex.addProduct(nid, n, c, p, o);
        productHashTable.insert(nid, n, c, p, o);
    }
    binarySearchIndex.buildIndex();

    refreshTabelProdukAdmin();
    refreshTampilanKatalog();

    QMessageBox::information(this, "Berhasil",
                             QString("✅ Produk ID %1 berhasil diperbarui.").arg(id));
}

// ====================================================================
// MANAJEMEN PRODUK — Hapus
// ====================================================================
void MainWindow::klikHapusProdukAdmin() {
    if (roleAktif != UserRole::ADMIN) return;

    QString idStr = txtProdukId->text().trimmed();
    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Pilih Produk", "Pilih produk dari tabel atau isi ID terlebih dahulu!"); return;
    }
    bool okId;
    int id = idStr.toInt(&okId);
    if (!okId || id <= 0) { QMessageBox::warning(this, "ID Tidak Valid", "ID harus angka bulat positif."); return; }

    AVLNode* target = avlTree.search(id);
    if (!target) {
        QMessageBox::warning(this, "Tidak Ditemukan", QString("Produk ID %1 tidak ada.").arg(id)); return;
    }

    // Konfirmasi dengan detail produk
    auto jawab = QMessageBox::question(this, "Konfirmasi Hapus",
                                       QString("⚠️ Yakin hapus produk berikut?\n\nID    : %1\nNama  : %2\nKategori: %3\nHarga : Rp %4\n\nAksi ini tidak bisa dibatalkan!")
                                           .arg(target->id)
                                           .arg(QString::fromStdString(target->name))
                                           .arg(QString::fromStdString(target->category))
                                           .arg(target->price),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (jawab != QMessageBox::Yes) return;

    // Hapus dari AVL Tree
    avlTree.remove(id);

    // Rebuild semua katalog dari sisa AVL Tree
    katalogElektronikDLL.clearCatalog();
    katalogPakaianDLL.clearCatalog();
    katalogPerabotanDLL.clearCatalog();
    bstElektronik.clear();
    bstPakaian.clear();
    bstPerabotan.clear();
    binarySearchIndex.clear();
    productHashTable.clear(); // rebuild hash table juga

    for (auto* node : avlTree.getAllSortedById()) {
        std::string n = node->name, c = node->category, o = node->origin;
        long p = node->price; int nid = node->id;
        if      (c == "Elektronik") { katalogElektronikDLL.insertProduct(nid,n,c,p,o); bstElektronik.insert(nid,n,c,p,o); }
        else if (c == "Pakaian")    { katalogPakaianDLL.insertProduct(nid,n,c,p,o);    bstPakaian.insert(nid,n,c,p,o); }
        else                        { katalogPerabotanDLL.insertProduct(nid,n,c,p,o);  bstPerabotan.insert(nid,n,c,p,o); }
        binarySearchIndex.addProduct(nid, n, c, p, o);
        productHashTable.insert(nid, n, c, p, o);
    }
    binarySearchIndex.buildIndex();

    refreshTabelProdukAdmin();
    refreshTampilanKatalog();

    // Bersihkan form
    txtProdukId->clear(); txtProdukNama->clear(); txtProdukHarga->clear();
    comboProdukKategori->setCurrentIndex(0); comboProdukHub->setCurrentIndex(0);

    QMessageBox::information(this, "Berhasil", QString("🗑 Produk ID %1 berhasil dihapus.").arg(id));
}
// ====================================================================
// HASH TABLE — Quick Lookup (Product / Voucher / Order)
// ====================================================================
void MainWindow::klikQuickLookup() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // Refresh stats setiap kali lookup
    QString stats;
    stats += QString::fromStdString(productHashTable.getStatsString()) + "\n\n";
    stats += QString::fromStdString(voucherHashTable.getStatsString()) + "\n\n";
    stats += QString::fromStdString(orderHashTable.getStatsString());
    txtHashStats->setText(stats);

    // ---- 1. Product Lookup by ID ----
    if (btn == btnHashCariProduk) {
        QString input = txtHashProdukId->text().trimmed();
        if (input.isEmpty()) {
            txtHashProdukResult->setText("⚠️  Masukkan ID produk terlebih dahulu.\nContoh: 101, 202, 303");
            return;
        }
        bool ok;
        int id = input.toInt(&ok);
        if (!ok || id <= 0) {
            txtHashProdukResult->setText("⚠️  ID harus berupa angka bulat positif.\nContoh: 101");
            return;
        }

        const ProductHashEntry* result = productHashTable.lookup(id);
        if (result) {
            txtHashProdukResult->setText(
                QString("✅  DITEMUKAN — O(1) Hash Lookup\n"
                        "─────────────────────────────\n"
                        "ID       : %1\n"
                        "Nama     : %2\n"
                        "Kategori : %3\n"
                        "Harga    : Rp %4\n"
                        "Asal Hub : %5\n"
                        "─────────────────────────────\n"
                        "Langsung ke bucket hash(key=%1)\n"
                        "tanpa traversal — O(1) rata-rata.")
                    .arg(result->id)
                    .arg(QString::fromStdString(result->name))
                    .arg(QString::fromStdString(result->category))
                    .arg(result->price)
                    .arg(QString::fromStdString(result->origin)));
        } else {
            txtHashProdukResult->setText(
                QString("❌  ID %1 tidak ditemukan di hash table.\n\n"
                        "ID tersedia: 101, 102, 103, 104,\n"
                        "             201, 202, 203, 302, 303").arg(id));
        }
    }

    // ---- 2. Voucher Lookup by Kode ----
    else if (btn == btnHashCariVoucher) {
        QString input = txtHashVoucherKode->text().trimmed().toUpper();
        if (input.isEmpty()) {
            txtHashVoucherResult->setText("⚠️  Masukkan kode voucher.\nContoh: HEMAT20, FLASH30");
            return;
        }

        const VoucherEntry* result = voucherHashTable.lookup(input.toStdString());
        if (result) {
            txtHashVoucherResult->setText(
                QString("✅  VOUCHER VALID — O(1) Hash Lookup\n"
                        "─────────────────────────────────\n"
                        "Kode      : %1\n"
                        "Diskon    : %2%%\n"
                        "Deskripsi : %3\n"
                        "Status    : ✅ Aktif\n"
                        "─────────────────────────────────\n"
                        "Hash djb2(%1) → bucket langsung\n"
                        "tanpa linear scan — O(1).")
                    .arg(QString::fromStdString(result->kode))
                    .arg(result->persen)
                    .arg(QString::fromStdString(result->deskripsi)));
        } else {
            txtHashVoucherResult->setText(
                QString("❌  Kode '%1' tidak ditemukan atau tidak aktif.\n\n"
                        "Kode tersedia:\n"
                        "  CCART50 · HEMAT20 · WELCOME10\n"
                        "  FLASH30 · GRATIS").arg(input));
        }
    }

    // ---- 3. Order Lookup by Resi ID ----
    else if (btn == btnHashCariOrder) {
        QString input = txtHashOrderId->text().trimmed().toUpper();
        if (input.isEmpty()) {
            txtHashOrderResult->setText("⚠️  Masukkan Resi ID.\nContoh: PKT-12345\n\nLakukan checkout terlebih dahulu untuk membuat order.");
            return;
        }

        const OrderHashEntry* result = orderHashTable.lookup(input.toStdString());
        if (result) {
            QString tipe = result->isInstant ? "⚡ INSTAN" : "📦 REGULER";
            txtHashOrderResult->setText(
                QString("✅  ORDER DITEMUKAN — O(1) Hash Lookup\n"
                        "──────────────────────────────────\n"
                        "Resi ID   : %1\n"
                        "Pelanggan : %2\n"
                        "Asal      : %3\n"
                        "Tujuan    : %4\n"
                        "Tipe      : %5\n"
                        "Lokasi    : %6\n"
                        "Sisa Waktu: %7 Jam\n"
                        "Status    : %8\n"
                        "Update    : %9\n"
                        "──────────────────────────────────\n"
                        "Hash djb2(resi) → bucket langsung\n"
                        "O(1) avg — tanpa loop databasePelacakan.")
                    .arg(QString::fromStdString(result->orderId))
                    .arg(QString::fromStdString(result->namaPelanggan))
                    .arg(QString::fromStdString(result->kotaAsal))
                    .arg(QString::fromStdString(result->kotaTujuan))
                    .arg(tipe)
                    .arg(QString::fromStdString(result->lokasiSekarang))
                    .arg(result->sisaWaktuJam)
                    .arg(QString::fromStdString(result->statusLogistik))
                    .arg(QString::fromStdString(result->lastUpdate)));
        } else {
            txtHashOrderResult->setText(
                QString("❌  Resi '%1' tidak ditemukan.\n\n"
                        "Pastikan format: PKT-XXXXX\n"
                        "Lakukan checkout untuk membuat order baru,\n"
                        "lalu salin Resi ID dari halaman Lacak Paket.")
                    .arg(input));
        }
    }
}