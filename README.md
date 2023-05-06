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

- Tải game về dưới dạng file zip hoặc dùng lệnh sau 
```
    git clone https://github.com/dluong1210/GameSDL_WheresGnuol.git
```

- Cuối cùng, mở project ( giải nén file nếu cần ) và click vào file *Where'sGnuol.exe* để chạy Game ( quá trình này có thế mất 10-30 giây ).

- Mọi góp ý hay lỗi trong quá trình cài đặt và chạy game, hãy liên hệ với tôi qua Email *luong8076@gmail.com*.

## **Mô tả Game**

### *Câu chuyện*

- 

### *Luật chơi*

- ![alt tag](https://github.com/dluong1210/GameSDL_WheresGnuol/blob/master/src/Texture/Enemy/enemyBlue.png)

### *Hướng dẫn chơi*

- Đối với Menu: 
    + Sử dụng chuột để tương tác với Menu của game.
    + Trong quá trình chơi, ấn ESC để tạm dừng game.

- Đối với nhân vật:
    + Sử dụng nút A để di chuyển sang trái.
    + Sử dụng nút D để di chuyển sang phải.
    + Sử dụng nút W hoặc Space để nhảy lên.
    + Sử dụng nút J để tấn công.