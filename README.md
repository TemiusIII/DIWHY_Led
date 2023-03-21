# DIWHY_Led

## Описание | Descriptiion
Этот проект был создан для того, чтобы помочь людям иметь LED подсветку по низкой цене, а также иметь возможность создавать свои эффекты.

Для полноценного проекта вам понадобится приобрести:
1. Адрессную ленту (https://aliexpress.ru/item/1005001689899636.html)
2. NODE MCU (https://aliexpress.ru/item/1005001621719732.html)
3. Если лента более 5 метров, то блок питания на 5В
4. Для постоянного сервера можно использовать Raspberry Pi, но это необязательно

## Использование | Usage

1. git clone https://github.com/TemiusIII/DIWHY_Led
2. Запустите main.py на вашем компьютере и скопируйте адресс сервера. ВАЖНО! Нужно поставить статический IP, чтобы не прошивать постоянно NODE_MCU

![image](https://user-images.githubusercontent.com/65336315/226576806-f1856700-3512-4a23-a1c1-8ddf08216c58.png)

3. Вставьте адресс сервера в код esp32_led_code.ino

![image](https://user-images.githubusercontent.com/65336315/226577281-68ed29c8-48e7-4e21-906e-68a893d7c51e.png)

4. Вставьте логин и пароль от вашей WIFI сети в код esp32_led_code.ino

![image](https://user-images.githubusercontent.com/65336315/226577526-042b7062-ab83-42bd-8cbd-8091a2772992.png)

5. Соедините LED Ленту с GND,  5v и D5 пинами, а также добавьте дополнительное питание для подсветки при необходимости
![image](https://user-images.githubusercontent.com/65336315/226578253-1072ee45-4ff4-4819-a962-e89919a813d9.png)
![image](https://user-images.githubusercontent.com/65336315/226578270-a2597655-d4d8-459f-82ff-88df6b339a97.png)


6. Прошейте NODE_MCU
7. Используйте сайт для контроля подсветки
8. Наслаждайтесь!

# Примеры использования | Usage examples


https://user-images.githubusercontent.com/65336315/226580401-c9d001ff-3166-4fbc-b818-490d05ca9783.MOV



https://user-images.githubusercontent.com/65336315/226580407-4dec61cf-ea98-42c7-b8c6-8fc1e8d12569.MOV

