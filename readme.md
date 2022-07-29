# keterangan utama
ini adalah repo untuk esp 01 yang digunakan untuk menerima dan mengirm data antara sensor pada arduino uno dan database pada firebase.
## alat dan bahan yang digunakan
untuk kodingan pada esp 01 kali ini saya menggunakan liblarry firebase dan liblarry wifi yang sudah include pada board esp.
## cara kerja
cara kerja kodingan untuk esp01

=> firebase mengirimkan berbagai data kepada esp01 secara realtime dan direfresh setiap ada perubahan pada database

=> esp 01 menerina berbagai data dari database dan mengirimkannya kepada arduino uno menggunakan komnikasi serial berupa baris dengan jenis data string.

## keterangan tambahan
=> jika ada yang tidak dimengetri, bisa laporkan di kolom laporan bug dan error pada repo github ini.

=> atau bisa juga mengirimkan pesan ke admin di nomor wa 08988927565

=> saya menggunakan platform io untuk melakukan pemrograman esp01 ini. platform io dapat diinstall dari ekstensi vs code.