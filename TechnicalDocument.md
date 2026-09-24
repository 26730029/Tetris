# Tetris Terminal — Tài liệu dự án

> **Tên đề tài:** Xây dựng trò chơi Tetris trên terminal bằng C++  
> **Ngôn ngữ:** C++ (C++11)  
> **Môi trường thực thi:** Windows, Linux hoặc macOS có terminal tương thích  
> **Thông tin báo cáo:** Có thể bổ sung tên môn học, giảng viên hướng dẫn, lớp và thành viên nhóm tại đây.

## Mục lục

1. Giới thiệu game
2. Yêu cầu và cách chạy
3. Hướng dẫn chơi
4. Tài liệu kỹ thuật
5. Kiểm thử và đánh giá
6. Giới hạn và hướng phát triển

## 1. Giới thiệu game

Đây là phiên bản **Tetris chạy trên cửa sổ dòng lệnh (terminal/console)**, viết bằng C++. Người chơi điều khiển các khối gồm bốn ô (*tetromino*) đang rơi để lấp đầy các hàng ngang. Hàng đầy sẽ bị xóa, các hàng bên trên rơi xuống, và người chơi nhận điểm. Trò chơi kết thúc khi khối mới không thể xuất hiện ở đầu bàn cờ.

Tetris là trò chơi cổ điển phù hợp để vận dụng các kiến thức về mảng hai chiều, vòng lặp thời gian thực, xử lý bàn phím, kế thừa, đa hình và thao tác tệp. Phiên bản này ưu tiên sự gọn nhẹ: toàn bộ trải nghiệm diễn ra trong terminal, không cần thư viện đồ họa hay thư viện game bên ngoài.

Game hiện có các chức năng:

- Đủ 7 loại tetromino: `I`, `O`, `T`, `S`, `Z`, `J`, `L`.
- Di chuyển ngang, rơi nhanh và xoay khối.
- Xóa đồng thời từ 1 đến 4 hàng, tính điểm theo kiểu Tetris.
- Tăng tốc độ rơi theo tổng số hàng đã xóa.
- Lưu điểm cao nhất vào tệp cục bộ `highscore.txt`.
- Đọc phím không chặn trên Windows và các hệ Unix-like (Linux/macOS).

### Mục tiêu thực hiện

- Mô phỏng đầy đủ cơ chế cốt lõi: sinh khối, rơi, dịch chuyển, xoay, va chạm, khóa khối và xóa hàng.
- Tổ chức bảy loại khối theo hướng đối tượng để minh họa kế thừa và đa hình trong C++.
- Nhận phím liên tục mà không làm khối ngừng rơi.
- Cung cấp điểm số, tăng độ khó theo tiến trình và lưu điểm cao giữa các lần chạy.

### Luật chơi được hiện thực

Mỗi tetromino gồm đúng bốn ô. Khối chỉ được di chuyển hoặc xoay khi mọi ô của nó còn nằm trong vùng chơi và không đè lên ô đã cố định. Khi không thể rơi tiếp, khối được khóa vào bàn cờ. Ngay sau đó, các hàng ngang đầy được xóa. Game kết thúc nếu không còn vị trí hợp lệ để sinh khối kế tiếp.

## 2. Yêu cầu và cách chạy

### Yêu cầu

- Trình biên dịch C++ hỗ trợ C++11 trở lên, ví dụ: GCC/G++, MinGW-w64 hoặc Clang.
- Terminal hỗ trợ xóa màn hình bằng `cls` (Windows) hoặc `clear` (Linux/macOS).

### Biên dịch

Tại thư mục dự án, chạy:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o tetris
```

Trên Linux/macOS, nếu môi trường yêu cầu liên kết thư viện luồng, dùng:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o tetris -pthread
```

### Khởi chạy

```bash
# Windows PowerShell
.\tetris.exe

# Linux/macOS
./tetris
```

Tệp `highscore.txt` được tạo trong **thư mục đang chạy chương trình** khi người chơi kết thúc bằng Game Over và đạt điểm cao mới. Không cần tạo tệp này trước.

### Lưu ý khi chạy

- Chạy trong terminal đủ rộng để thấy phần điểm số bên phải bàn cờ.
- Không chuyển hướng đầu vào từ tệp, vì game cần nhận phím trực tiếp và liên tục từ terminal.
- Trên Windows, chương trình dùng `_kbhit()` và `_getch()` từ `conio.h`. Trên Linux/macOS, chương trình dùng `termios`, `fcntl()` và `read()` để đọc phím không chặn.
- Khi chạy trên Unix-like, terminal được đưa về trạng thái ban đầu lúc thoát nhờ `atexit()` và xử lý tín hiệu `SIGINT`/`SIGTERM`.

## 3. Hướng dẫn chơi

Mỗi ô gạch được vẽ bằng `[]`; viền bàn chơi được vẽ bằng `##`. Điểm hiện tại và điểm cao nhất hiển thị bên phải bàn cờ.

| Phím | Chức năng |
| --- | --- |
| `A` / `a` | Di chuyển khối sang trái |
| `D` / `d` | Di chuyển khối sang phải |
| `W` / `w` | Xoay khối 90° theo chiều kim đồng hồ |
| `X` / `x` | Làm khối rơi xuống nhanh thêm một hàng |
| `Q` / `q` | Thoát game ngay |
| `Ctrl+C` | Dừng chương trình; terminal được khôi phục cấu hình nhập liệu |

Lưu ý: mã nguồn hiện xử lý phím chữ thường. Với bố cục bàn phím thông thường, hãy dùng phím chữ và không bật Caps Lock. Lệnh xoay hoặc di chuyển sẽ bị bỏ qua nếu thao tác đó làm khối chạm tường, đáy hoặc khối đã cố định.

### Mẹo chơi

- Giữ bề mặt các khối tương đối phẳng để hạn chế các hốc trống khó lấp đầy.
- Có thể để một khe dọc cao bốn ô cho khối `I`; xóa bốn hàng trong một lượt sẽ đạt số điểm cao nhất.
- Phím `x` chỉ làm khối rơi thêm một hàng cho mỗi lần nhận phím, không phải thao tác rơi thẳng xuống đáy.
- Khối `O` không đổi hình khi xoay; các khối còn lại xoay theo chiều kim đồng hồ.

### Cách tính điểm và tốc độ

| Số hàng xóa trong một lần khóa khối | Điểm cộng |
| --- | ---: |
| 1 hàng (Single) | 100 |
| 2 hàng (Double) | 300 |
| 3 hàng (Triple) | 500 |
| 4 hàng (Tetris) | 800 |

Khối bắt đầu rơi một hàng mỗi 250 ms. Sau mỗi 5 hàng bị xóa (tính cộng dồn), thời gian này giảm 50 ms; giới hạn thấp nhất là 100 ms mỗi hàng.

## 4. Tài liệu kỹ thuật

### 4.1. Cấu trúc mã nguồn

| Tệp | Vai trò |
| --- | --- |
| `main.cpp` | Vòng lặp game, nhập phím, vẽ màn hình, va chạm, khóa khối, xóa hàng, điểm và điểm cao. |
| `Blocks.h` | Lớp cơ sở `Blocks`: lưu ma trận hình dạng 4×4, xoay, đọc và ghi ô. |
| `IBlock.h` … `ZBlock.h` | Bảy lớp kế thừa `Blocks`, mỗi lớp khởi tạo một hình tetromino riêng. |

Không có thư viện game/đồ họa ngoài; chương trình chỉ dùng thư viện chuẩn C++ và API terminal theo hệ điều hành.

Các header sử dụng *include guard* (`#ifndef`, `#define`, `#endif`) để tránh bị nạp lặp lại khi biên dịch. Từng lớp khối nằm trong một tệp riêng, vì vậy việc điều chỉnh hình ban đầu của một tetromino không ảnh hưởng trực tiếp đến vòng lặp game.

### 4.2. Mô hình dữ liệu

- Bàn cờ là mảng toàn cục `char board[20][15]`.
- Hàng `0`, hàng `19`, cột `0` và cột `14` là tường `#`; vùng chơi thực tế vì vậy có **18 hàng × 13 cột**.
- Mỗi khối có ma trận riêng `shape[4][4]`. Ô trống là ký tự cách (`' '`); các ô còn lại mang ký tự tên khối, chẳng hạn `T` hoặc `L`.
- `currentBlock` là con trỏ `Blocks*` đến khối đang rơi; `createBlock()` đóng vai trò factory để cấp phát đúng lớp dẫn xuất. Khối cũ được `delete` trước khi sinh khối mới và khi thoát chương trình.
- Vị trí góc trên-trái của ma trận khối trên bàn cờ là `(x, y)`. Khối mới xuất hiện tại `x = 5`, `y = 1`.

Sơ đồ dữ liệu chính:

```text
board[20][15]
 ├── Tường trên/dưới và hai tường bên: '#'
 └── Vùng chơi: ô trống ' ' hoặc các ô của khối đã khóa

currentBlock (Blocks*)
 └── IBlock / OBlock / TBlock / SBlock / ZBlock / JBlock / LBlock
      └── shape[4][4], được ghép vào board tại tọa độ (x, y)
```

Ký tự `I`, `O`, `T`, `S`, `Z`, `J`, `L` dùng để nhận diện ô trong dữ liệu. Khi vẽ, mọi ô không trống hiện đều được hiển thị là `[]`, nên phiên bản này chưa phân biệt màu giữa các loại khối.

### 4.3. Kế thừa và đa hình

`Blocks` cung cấp `virtual void rotate()` để xoay ma trận 4×4 theo chiều kim đồng hồ. Các khối `I`, `T`, `S`, `Z`, `J`, `L` dùng hành vi mặc định. `OBlock` ghi đè hàm này bằng hàm rỗng vì khối vuông không đổi hình khi xoay.

Destructor của `Blocks` là `virtual`, do đó xóa một đối tượng dẫn xuất qua `Blocks*` là an toàn. Khi người chơi nhấn `w`, `rotateCurrentBlock()` sao chép hình cũ, thực hiện lời gọi đa hình `currentBlock->rotate()`, rồi phục hồi hình cũ nếu vị trí xoay không hợp lệ.

Hàm factory `createBlock(int type)` nhận số từ 0 đến 6 và trả về đối tượng thuộc lớp dẫn xuất tương ứng qua con trỏ `Blocks*`. Việc mỗi lần sinh đều tạo object mới giúp phép xoay khối hiện tại không làm thay đổi một khuôn mẫu dùng chung, nhờ đó tránh lỗi khối sinh sau có hình dạng bị sai.

### 4.4. Các hàm xử lý chính

| Hàm | Nhiệm vụ |
| --- | --- |
| `initBoard()` | Khởi tạo tường `#` và tất cả ô trống. |
| `spawnBlock()` | Hủy khối cũ, chọn ngẫu nhiên loại mới, tạo object và đặt vị trí xuất hiện. |
| `canMove(dx, dy)` | Kiểm tra tính hợp lệ khi dịch khối thêm `(dx, dy)`. |
| `rotateCurrentBlock()` | Xoay, kiểm tra va chạm và hoàn tác nếu cần. |
| `block2Board()` | Chép các ô của khối đang rơi vào `board` để hiển thị/khóa. |
| `boardDelBlock()` | Xóa hình tạm thời của khối đang rơi trước khi cập nhật. |
| `removeLine()` | Xóa hàng đầy, dồn hàng phía trên xuống và trả số hàng bị xóa. |
| `draw()` | Xóa console rồi in bàn cờ, điểm hiện tại và điểm cao. |
| `loadHighScore()` / `saveHighScore()` | Đọc/ghi điểm cao từ tệp `highscore.txt`. |

`canMove()` là điểm kiểm tra an toàn trung tâm. Hàm duyệt đủ ma trận 4×4; với mỗi ô không trống `(i, j)`, nó kiểm tra ô tương ứng `(x + j + dx, y + i + dy)` trên bàn cờ. Hành động bị từ chối khi tọa độ nằm ngoài vùng cho phép hoặc ô đích đã không còn trống.

### 4.5. Luồng xử lý một khung hình

Chương trình chạy với chu kỳ khoảng 16 ms (xấp xỉ 60 FPS):

1. Xóa phần hiển thị tạm thời của khối đang rơi khỏi `board` bằng `boardDelBlock()`.
2. Đọc toàn bộ các phím đang chờ và áp dụng các thao tác hợp lệ.
3. Cộng bộ đếm thời gian. Khi đạt `sleepTime`, thử cho khối rơi một hàng.
4. Nếu không thể rơi, ghi khối vào bàn cờ, xóa các hàng đầy, cập nhật điểm/tốc độ, rồi sinh khối mới.
5. Ghi lại khối đang rơi, gọi `draw()` để vẽ toàn bộ bàn cờ và chờ 16 ms.

Hàm `canMove(dx, dy)` duyệt đủ ma trận 4×4, kiểm tra mọi ô không trống của khối với biên bàn cờ và nội dung `board`. Đây là điều kiện chung cho di chuyển, rơi và kiểm tra sau xoay.

Giả mã của vòng lặp game:

```text
khởi tạo terminal, điểm cao, bàn cờ và khối đầu tiên
trong khi chưa thoát:
    xóa khối đang rơi khỏi board tạm thời
    đọc toàn bộ phím đang chờ; áp dụng thao tác hợp lệ
    nếu bộ đếm gravity đạt sleepTime:
        nếu khối còn rơi được: tăng y
        nếu không: khóa khối, xóa hàng, cập nhật điểm/tốc độ, sinh khối mới
        nếu khối mới không hợp lệ: Game Over
    ghi lại khối đang rơi và vẽ board
    chờ 16 ms
```

### 4.6. Xóa hàng, Game Over và lưu điểm

`removeLine()` quét từ dưới lên. Khi gặp một hàng đầy, nó dồn các hàng phía trên xuống một bậc, làm trống hàng đầu vùng chơi, tăng biến đếm và kiểm tra lại cùng chỉ số hàng. Cách này cho phép xử lý các hàng đầy liền kề trong một lượt khóa khối.

Sau khi sinh khối mới, chương trình gọi `canMove(0, 0)`. Nếu khối đã chồng lên khối cố định, cờ `gameOver` được bật, điểm được hiển thị và điểm cao chỉ được ghi đè nếu `score > highScore`. Thoát bằng `q` không đi vào nhánh Game Over, nên không cập nhật điểm cao ở phiên bản hiện tại.

### 4.7. Xử lý nhập liệu và quản lý tài nguyên

Chương trình không dùng `std::cin` trong game loop vì lời gọi chặn sẽ làm khối ngừng rơi trong lúc chờ người chơi. Thay vào đó, `kbhit()` chỉ kiểm tra sự tồn tại của phím trong hàng đợi. Trên Windows, đây là lớp bao cho `_kbhit()`; trên Unix-like, mã nguồn dùng terminal non-canonical, tắt echo và gắn cờ `O_NONBLOCK`.

Khối được cấp phát động do loại cụ thể chỉ được biết sau bước chọn ngẫu nhiên. `spawnBlock()` luôn gọi `delete currentBlock` trước khi tạo khối kế tiếp, và cuối `main()` cũng giải phóng khối còn lại. `currentBlock` được khởi tạo `nullptr`, vì thế lệnh `delete` đầu tiên an toàn. Destructor ảo của `Blocks` đảm bảo hủy đúng đối tượng dẫn xuất.

### 4.8. Khả năng mở rộng

- Thêm phím hard drop, giữ khối (hold) hoặc hiển thị khối kế tiếp.
- Thay `rand()` bằng bộ sinh số ngẫu nhiên hiện đại và cơ chế 7-bag để phân phối khối cân bằng hơn.
- Tách mã thành các lớp `Board`, `Game` và `Input` để giảm biến toàn cục, thuận tiện kiểm thử.
- Dùng thư viện giao diện như SFML/SDL hoặc ANSI escape codes để có màu sắc, âm thanh và giao diện ổn định hơn.
- Lưu điểm cao ngay khi người chơi thoát chủ động nếu đó là hành vi mong muốn.

## 5. Kiểm thử và đánh giá

Do game tương tác trực tiếp với terminal, việc kiểm thử gồm kiểm tra biên dịch và chạy các tình huống thủ công. Bảng sau có thể dùng làm checklist khi nghiệm thu chương trình.

| Mã | Thao tác / điều kiện | Kết quả mong đợi |
| --- | --- | --- |
| TC01 | Biên dịch `main.cpp` với chuẩn C++11 | Không có lỗi biên dịch; tạo được chương trình thực thi. |
| TC02 | Khởi chạy khi chưa có `highscore.txt` | Game vẫn chạy; điểm cao mặc định là 0. |
| TC03 | Nhấn `a` khi bên trái còn trống | Khối dịch sang trái một ô. |
| TC04 | Nhấn `a` sát tường trái hoặc `d` sát tường phải | Khối không vượt qua tường. |
| TC05 | Nhấn `w` ở vị trí trống | Khối xoay 90° theo chiều kim đồng hồ. |
| TC06 | Nhấn `w` khi xoay gây chạm tường/khối khác | Hình cũ được phục hồi, không có ô chồng lấp. |
| TC07 | Để khối chạm đáy hoặc khối đã khóa | Khối được cố định; một khối mới xuất hiện. |
| TC08 | Lấp đầy một hàng | Hàng biến mất, hàng trên dồn xuống, điểm tăng 100. |
| TC09 | Lấp đầy bốn hàng trong một lần khóa | Bốn hàng bị xóa và điểm tăng 800. |
| TC10 | Lấp khu vực sinh khối | Game hiển thị `GAME OVER` và điểm cuối. |
| TC11 | Game Over với điểm cao hơn giá trị cũ | `highscore.txt` được ghi điểm cao mới. |
| TC12 | Nhấn `q` hoặc `Ctrl+C` | Chương trình dừng; terminal Unix-like được hoàn nguyên cấu hình nhập. |

### Kiểm tra kỹ thuật đã thực hiện

Mã nguồn được kiểm tra cú pháp bằng lệnh:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic -fsyntax-only main.cpp
```

Lệnh trên không tạo tệp thực thi và phù hợp để phát hiện lỗi cú pháp hoặc cảnh báo cơ bản của trình biên dịch. Những hành vi phụ thuộc terminal, chẳng hạn nhận phím và xóa màn hình, cần được xác nhận thêm bằng cách chạy trực tiếp trên hệ điều hành đích.

## 6. Giới hạn và hướng phát triển

Phiên bản hiện tại đã hoàn thành cơ chế chơi cốt lõi, tuy nhiên vẫn còn các giới hạn sau:

- Màn hình được xóa và vẽ lại hoàn toàn qua `system("cls")` hoặc `system("clear")` ở mỗi khung hình. Cách làm này đơn giản nhưng có thể nhấp nháy và không tối ưu.
- Chưa có màu sắc, âm thanh, menu bắt đầu, tạm dừng, hiển thị khối kế tiếp hoặc chức năng giữ khối.
- Khối được chọn độc lập bằng `rand() % 7`, nên phân phối có thể mất cân bằng so với thuật toán 7-bag của Tetris hiện đại.
- Chưa có cơ chế wall kick: nếu xoay không hợp lệ, khối chỉ hoàn tác thay vì tự dịch ngang.
- Điểm cao chỉ được lưu khi Game Over, không lưu nếu người chơi thoát bằng `q`.
- Chưa có bộ kiểm thử tự động tách biệt với vòng lặp console.

Hướng phát triển phù hợp là tách mã thành các lớp `Board`, `Game`, `Input` và `Renderer`; dùng ANSI escape code hoặc thư viện SDL/SFML để vẽ mượt; bổ sung màu, preview, hold, pause, hard drop và hệ thống level; thay `rand()` bằng cơ chế 7-bag; đồng thời viết kiểm thử đơn vị cho va chạm và xóa hàng. Các cải tiến này sẽ giúp trò chơi hoàn thiện hơn về trải nghiệm, đồng thời làm mã nguồn dễ bảo trì và mở rộng.
