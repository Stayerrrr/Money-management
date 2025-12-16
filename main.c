#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================ Variabel global ================
char username[25];
double inputPassword;

#define MAX_NOREKENING 999999
#define MIN_NOREKENING 100000
#define MIN_TRANSFER 20000

FILE *saldo;
const char *namaFile_saldo = "saldo.txt";
FILE *history_transfer;
const char *namaFile_history_transfer = "history_transfer.txt";
FILE *salDeposit;
const char *namaFile_deposit = "deposit.txt";

#define MAX_LINE_LENGTH 200
char line[MAX_LINE_LENGTH];

double saldo_tabungan;
double depoSaldo;

double kirimTf;
int noRekeningTf;
char tanggalTf[11];

// ================= FUNGSI BANTU =================
void updateSaldo(double saldoBaru) {
    FILE *temp = fopen("temp_saldo.txt", "w");
    if (temp == NULL) {
        printf("Gagal update saldo!\n");
        return;
    }
    fprintf(temp, "Saldo: Rp. %.2lf\n", saldoBaru);
    fclose(temp);
    remove(namaFile_saldo);
    rename("temp_saldo.txt", namaFile_saldo);
}

// ================= DEKLARASI =================
void menu();
void lihatTabungan();
void transfer();
void peminjaman();
void bayar_tagihan();

// ================= LIHAT TABUNGAN & DEPOSIT =================
void tabungan() {
    saldo = fopen(namaFile_saldo, "r");
    if (!saldo) return;
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    printf("\nSaldo Anda: Rp. %.2lf\n", saldo_tabungan);
    printf("Tekan Enter...");
    getchar(); getchar();
}

void deposit() {
    saldo = fopen(namaFile_saldo, "r");
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    salDeposit = fopen(namaFile_deposit, "r");
    if (salDeposit) {
        fgets(line, sizeof(line), salDeposit);
        sscanf(line, "Saldo Deposit: Rp. %lf", &depoSaldo);
        fclose(salDeposit);
    } else depoSaldo = 0;

    printf("\nSaldo Tabungan: Rp. %.2lf\n", saldo_tabungan);
    printf("Saldo Deposit : Rp. %.2lf\n", depoSaldo);

    printf("Tambah deposit? (y/n): ");
    char c; scanf(" %c", &c);
    if (c == 'n' || c == 'N') return;

    double tambah;
    printf("Jumlah deposit: ");
    scanf("%lf", &tambah);

    if (tambah <= 0 || tambah > saldo_tabungan) {
        printf("Jumlah tidak valid!\n");
        return;
    }

    saldo_tabungan -= tambah;
    depoSaldo += tambah;
    updateSaldo(saldo_tabungan);

    FILE *t = fopen(namaFile_deposit, "w");
    fprintf(t, "Saldo Deposit: Rp. %.2lf\n", depoSaldo);
    fclose(t);

    printf("Deposit berhasil!\n");
}

void lihatTabungan() {
    int p;
    do {
        printf("\n1. Tabungan\n2. Deposit\n3. Kembali\nPilih: ");
        scanf("%d", &p);
        if (p == 1) tabungan();
        else if (p == 2) deposit();
    } while (p != 3);
}

// ================= TRANSFER =================
void transfer() {
    saldo = fopen(namaFile_saldo, "r");
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    printf("Jumlah transfer (Minimal 20k): ");
    scanf("%lf", &kirimTf);
    if (kirimTf < MIN_TRANSFER || kirimTf > saldo_tabungan) {
        printf("Jumlah tidak valid! (Minimal 20k)\n");
        return;
    }

    printf("Tanggal (DD/MM/YYYY): ");
    scanf("%10s", tanggalTf);

    printf("No Rekening: ");
    scanf("%d", &noRekeningTf);
    if (noRekeningTf < MIN_NOREKENING || noRekeningTf > MAX_NOREKENING) {
        printf("No rekening salah!\n");
        return;
    }

    history_transfer = fopen(namaFile_history_transfer, "a");
    fprintf(history_transfer, "%s | %.2lf | %d\n",
            tanggalTf, kirimTf, noRekeningTf);
    fclose(history_transfer);

    saldo_tabungan -= kirimTf;
    updateSaldo(saldo_tabungan);

    printf("Transfer berhasil!\n");
}

// ================= PEMINJAMAN (SALDO BERTAMBAH) =================
void peminjaman() {
    int pilihan;
    double jumlah;

    saldo = fopen(namaFile_saldo, "r");
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    printf("\n1. KPR (3jt - 128jt)\n2. Multiguna (5jt - 500jt)\n3. Kembali\nPilih: ");
    scanf("%d", &pilihan);

    if (pilihan == 1 || pilihan == 2) {
        printf("Masukkan jumlah pinjaman: ");
        scanf("%lf", &jumlah);

        if ((pilihan == 1 && (jumlah < 3000000 || jumlah > 128000000)) ||
            (pilihan == 2 && (jumlah < 5000000 || jumlah > 500000000))) {
            printf("Jumlah tidak sesuai ketentuan!\n");
            return;
        }

        saldo_tabungan += jumlah;
        updateSaldo(saldo_tabungan);

        printf("Pinjaman disetujui!\n");
        printf("Saldo sekarang: Rp. %.2lf\n", saldo_tabungan);
    }
}

// ================= BAYAR TAGIHAN (SALDO BERKURANG) =================
void bayar_tagihan() {
    int p;
    double n;

    saldo = fopen(namaFile_saldo, "r");
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    while (1) {
        printf("\n1. PLN\n2. Internet\n3. Air\n4. E-Wallet\n");
        printf("5. Asuransi\n6. Pajak\n7. Pendidikan\n8. Kembali\nPilih: ");
        scanf("%d", &p);
        if (p == 8) break;

        printf("Nominal: ");
        scanf("%lf", &n);

        if (n <= 0 || n > saldo_tabungan) {
            printf("Saldo tidak cukup!\n");
            continue;
        }

        saldo_tabungan -= n;
        updateSaldo(saldo_tabungan);

        printf("Pembayaran berhasil!\n");
        printf("Sisa saldo: Rp. %.2lf\n", saldo_tabungan);
    }
}

// ================= MENU =================
void menu() {
    int p;
    do {
        printf("\n1. Lihat Tabungan\n2. Transfer\n3. Peminjaman\n4. Bayar Tagihan\n5. Keluar\nPilih: ");
        scanf("%d", &p);

        if (p == 1) lihatTabungan();
        else if (p == 2) transfer();
        else if (p == 3) peminjaman();
        else if (p == 4) bayar_tagihan();

    } while (p != 5);
}

// ================= MAIN =================
int main() {
    char correctUsername[] = "Tioganteng123";
    double correctPassword = 606060;

    printf("Selamat Datang di Aplikasi Akubisa-mobile\n");

    while (1) {
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Password: ");
        scanf("%lf", &inputPassword);
        getchar();

        if (!strcmp(username, correctUsername) &&
            inputPassword == correctPassword) break;

        printf("Login salah!\n");
    }

    saldo = fopen(namaFile_saldo, "a+");
    if (!fgets(line, sizeof(line), saldo)) {
        printf("Masukkan saldo awal: ");
        scanf("%lf", &saldo_tabungan);
        fprintf(saldo, "Saldo: Rp. %.2lf\n", saldo_tabungan);
    }
    fclose(saldo);

    menu();
    return 0;
}