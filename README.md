# ***Where's Gnuol !?***

## **Giới Thiệu**

+ Họ và Tên: Trần Đức Lương

+ Mã sinh viên: 22021148

+ Bài tập lớn: **Game Where's Gnuol** / INT2215 N1 - Lập trình nâng cao.

## **Hướng dẫn cài đặt Game**

- *Trước tiên hãy đảm bảo máy tính của bạn đã có thư viện SDL2 ( gồm cả SDL_image, SDL_mixer và SDL_ttf ).*

### **Cài đặt thư viện SDL**

- Hướng dẫn cài SDL: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php
- Hướng dẫn cài SDL_Image: https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php
- Hướng dẫn cài SDL_Mixer: https://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php
- Hướng dẫn cài SDL_TTF: https://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php

### **Cài đặt và chạy Game**

- Sau khi đã cài đặt được thư viện. Tiếp theo, nếu bạn có khả năng làm việc với Makefile hãy thay đổi đường dẫn include và lib sao cho phù hợp với thư viện SDL2 của bạn. Nếu không hãy để thư viện SDL2 ở ổ *C::* với đường dẫn như sau: 
    + include: *C:/SDL2/include* 
    + lib: *C:/SDL2/lib*

- Tải game về dưới dạng file zip hoặc dùng lệnh sau:
```
    git clone https://github.com/dluong1210/GameSDL_WheresGnuol.git
```

- Cuối cùng, mở project ( giải nén file nếu cần ) và click vào file *Where'sGnuol.exe* để chạy Game ( quá trình này có thế mất 10-30 giây ).

- Mọi góp ý hay lỗi trong quá trình cài đặt và chạy game, hãy liên hệ với tôi qua Email *luong8076@gmail.com*.

## **Mô tả Game**

### *Câu chuyện*

- 

### *Luật chơi*

- Người chơi sẽ có HP = 3. Bạn phải cố gắng di chuyển xa và tiêu diệt được nhiều quái để đạt được điểm cao.

- Có 4 loại quái trong game ( 3 con di chuyển trên mặt đất và 1 con bay ) cùng với các thuộc tính:
    + ![alt tag](https://github.com/dluong1210/GameSDL_WheresGnuol/blob/master/src/Texture/Enemy/enemy1.png) : HP = 3, Speed = 4, tiêu diệt giúp nhận thêm HP.
    + ![alt tag](https://github.com/dluong1210/GameSDL_WheresGnuol/blob/master/src/Texture/Enemy/enemy2.png) : HP = 5, Speed = 4, tiêu diệt giúp nhận 10 điểm.
    + ![alt tag](https://github.com/dluong1210/GameSDL_WheresGnuol/blob/master/src/Texture/Enemy/enemy3.png) : HP = 2, Speed = 5, tiêu diệt giúp nhận 5 điểm.
    + ![alt tag](https://github.com/dluong1210/GameSDL_WheresGnuol/blob/master/src/Texture/Enemy/eagle4.png) : HP = 3, Speed = 4, tiêu diệt giúp nhận 15 điểm.

- Khi bị phát hiện quái sẽ đuổi theo và tấn công bạn. Khi hết HP bạn sẽ thua nên hãy cẩn thận lượng HP của mình !!!

### *Hướng dẫn chơi*

- Đối với Menu: 
    + Sử dụng chuột để tương tác với Menu của game.
    + Trong quá trình chơi, ấn ESC để tạm dừng game.

- Đối với nhân vật:
    + Sử dụng nút A để di chuyển sang trái.
    + Sử dụng nút D để di chuyển sang phải.
    + Sử dụng nút W hoặc Space để nhảy lên.
    + Sử dụng nút J để tấn công.

### *Các tính năng của Game*

- Tắt/bật nhạc nền ( music ) và hiệu ứng âm thanh ( sound ).

- Tắt/bật chế độ toàn màn hình.

- Tính điểm và lưu lại điểm cao nhất kể cả khi đã tắt game.

- Khi thua có thể chọn thử lại luôn.

## **Các kỹ thuật sử dụng trong Game**

- Kĩ thuật tách file.

- Xây dựng và xử lý va chạm với Tile Map.

- Xây dựng các class, object. Sử dụng lớp kế thừa, quản lý và liên kết các object.

- Xử lý đồ họa, tương tác âm thanh và chuyển động của các object( player, enemy... ).

- Sử dụng phương pháp tính lerp ( nội suy tuyến tính ) đối với camera;

- Kỹ thuật nối các Map với nhau. Do đó xây dựng được Map dài vô tận: *Trong 1 thời điểm chỉ có 3 Map trong hàng chờ, di chuyển tới đâu render tới đó.*

- Đối với các đối tượng quái trên mặt đất *Enemy*:
    + Tự động di chuyển và đuổi theo khi người chơi đến gần.
    + Luôn tính toán xem có thể di chuyển được không. Từ đó tránh tình trạng tự lao xuống vực hay cắm mặt vào tường.

- Đối với đối tượng quái bay *Eagle*:
    + Sử dụng thuật toán ngoài chương trình: *Tìm đường đi ngắn nhất A-Star ( Xử lý theo BFS )* để đuổi theo người chơi.

- Clean code, xử lý tối ưu không xuất hiện tình trạng tràn Ram.