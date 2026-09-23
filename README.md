# Tetris Terminal (C++)

> Trò chơi Tetris chạy trên terminal, được xây dựng bằng C++ theo mô hình lập trình hướng đối tượng.

## Giới thiệu

Dự án hiện thực phiên bản Tetris chơi trực tiếp trong cửa sổ dòng lệnh. Người chơi điều khiển các khối tetromino đang rơi để lấp kín hàng ngang. Khi một hàng được lấp đầy, hàng đó bị xóa và người chơi nhận điểm. Trò chơi kết thúc khi không thể sinh khối mới ở đầu bàn cờ.

Chương trình không sử dụng thư viện game hay đồ họa bên ngoài. Toàn bộ thao tác vẽ, nhập phím, kiểm tra va chạm và lưu điểm cao được xử lý bằng C++ cùng API terminal của hệ điều hành.

## Thành viên nhóm

| Họ và tên | MSSV |
| --- | --- |
| Dương Vĩnh Khang | 26730029 |
| Phạm Minh Thư | 26730066 |
| Nguyễn Minh Hiếu | 26730019 |
| Lê Kiều Diễm | 26730010 |
| Vũ Duy Khánh | 26730034 |


## Chức năng

- Hỗ trợ đầy đủ 7 loại tetromino: `I`, `O`, `T`, `S`, `Z`, `J`, `L`.
- Di chuyển trái/phải, xoay khối và rơi nhanh.
- Kiểm tra va chạm với tường, đáy bàn cờ và các khối đã cố định.
- Xóa từ 1 đến 4 hàng trong một lượt.
- Tính điểm theo mức Single, Double, Triple và Tetris.
- Tăng tốc độ rơi sau mỗi 5 hàng bị xóa.
- Lưu điểm cao nhất trong tệp `highscore.txt`.
- Hỗ trợ nhập phím không chặn trên Windows và các hệ Unix-like.

## Yêu cầu môi trường

- Trình biên dịch hỗ trợ C++11 trở lên: GCC/G++, MinGW-w64 hoặc Clang.
- Terminal/console hỗ trợ lệnh `cls` trên Windows hoặc `clear` trên Linux/macOS.

## Biên dịch và chạy

Mở terminal tại thư mục dự án, sau đó biên dịch:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o tetris
```

Trên Linux/macOS, nếu trình biên dịch yêu cầu liên kết thư viện luồng:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o tetris -pthread
```

Chạy chương trình:

```bash
# Windows PowerShell
.\tetris.exe

# Linux/macOS
./tetris
```

Tệp `highscore.txt` không cần tạo sẵn. Chương trình sẽ đọc điểm cao nếu tệp đã có và ghi tệp khi người chơi đạt điểm cao mới sau Game Over.

## Điều khiển

| Phím | Chức năng |
| --- | --- |
| `a` | Di chuyển khối sang trái |
| `d` | Di chuyển khối sang phải |
| `w` | Xoay khối 90° theo chiều kim đồng hồ |
| `x` | Làm khối rơi nhanh thêm một hàng |
| `q` | Thoát game |
| `Ctrl+C` | Dừng chương trình an toàn |

Lưu ý: chương trình xử lý phím chữ thường. Thao tác di chuyển hoặc xoay không hợp lệ sẽ bị bỏ qua.

## Cách tính điểm và độ khó

| Số hàng bị xóa trong một lượt | Điểm nhận được |
| --- | ---: |
| 1 hàng — Single | 100 |
| 2 hàng — Double | 300 |
| 3 hàng — Triple | 500 |
| 4 hàng — Tetris | 800 |

Khối ban đầu rơi một hàng mỗi 250 ms. Cứ mỗi 5 hàng bị xóa (tính cộng dồn), thời gian rơi giảm 50 ms, tối đa nhanh đến 100 ms mỗi hàng.

## Cấu trúc dự án

```text
Tetris/
├── main.cpp       # Vòng lặp game, bàn cờ, nhập phím, điểm số
├── Blocks.h       # Lớp cơ sở của mọi khối tetromino
├── IBlock.h       # Khối I
├── OBlock.h       # Khối O
├── TBlock.h       # Khối T
├── SBlock.h       # Khối S
├── ZBlock.h       # Khối Z
├── JBlock.h       # Khối J
├── LBlock.h       # Khối L
├── TechnicalDocument.md    # Tài liệu kỹ thuật và hướng dẫn chi tiết
└── README.md      # Giới thiệu nhanh dự án
```

`Blocks` là lớp cơ sở chứa ma trận hình dạng 4×4 và hàm xoay ảo `rotate()`. Bảy lớp khối kế thừa từ lớp này. `OBlock` ghi đè thao tác xoay vì hình vuông không thay đổi sau khi xoay. Khối hiện tại được quản lý qua con trỏ `Blocks*`, thể hiện việc sử dụng đa hình trong C++.

## Tài liệu đầy đủ

Xem [TechnicalDocument.md](TechnicalDocument.md) để đọc tài liệu chi tiết về kiến trúc, cấu trúc dữ liệu, thuật toán va chạm/xóa hàng, xử lý nhập liệu đa nền tảng, kiểm thử, giới hạn và định hướng phát triển.

## Giấy phép

Dự án được thực hiện phục vụ mục đích học tập.
