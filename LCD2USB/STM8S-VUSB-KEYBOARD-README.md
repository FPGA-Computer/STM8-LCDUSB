# STM8S-VUSB-KEYBOARD

The project is based on the source code of Denis Zheleznyakov. Links: http://ziblog.ru/ https://github.com/ZiB/STM8S-USB

The project includes:
- USB stack (with limitations)
- universal USB HID keyboard (3 pages of scan codes)
- customization of keys via HID requests and storage of settings in EEPROM
- 4x4 button matrix
- anti-bounce filter for keys
- the ability to work from an external crystal at 16 MHz, and from an internal RC oscillator with tuning

Disadvantages:
- The algorithm does not know how to cut out insignificant bits on reception, therefore, if the USB host assigns the 0xXF address to the device on the bus, the device is not able to correctly receive packets, and responds to the host in the spirit of "mine yours does not understand", after which the host resets the device and assigns it another address. It goes unnoticed for the user.
- Port C pins can only be used as GPIO inputs.

Development environment - STVD 4.3.11

Compiler - Cosmic cross compiler v.4.4.7

Scheme: https://github.com/BBS215/STM8S-VUSB-KEYBOARD/blob/master/SCHEME/Scheme.pdf

When the firmware starts up, the following OPTION BYTE settings are recorded automatically:
1) AFR0: PORT C7 Alternate Function = TIM1_CH2
2) HSITRIM: 4 bit on-the-fly trimming

To use HSI, you need to set #define USB_CLOCK_HSI to 1. After flashing, the controller will try to connect to the USB host at different HSI_TRIM_VAL values, every 8 seconds. If the connection is successful, the working value of HSI_TRIM_VAL will be saved in EEPROM. This value will later be used when restarting the controller. To re-configure HSI_TRIM_VAL, write zeros in the first 2 bytes of the EEPROM and restart the controller.

Key customization software:
- https://github.com/BBS215/SetupKeys - console utility
- https://github.com/BBS215/SetupKeysGUI - graphical utility

-----------------------------------------------------------------
# STM8S-VUSB-KEYBOARD

Проект основан на исходниках Дениса Железнякова. Ссылки: http://ziblog.ru/  https://github.com/ZiB/STM8S-USB

В проекте реализованы:
- USB стэк (с ограничениями)
- универсальная USB HID клавиатура (3 страницы скан-кодов)
- настройка клавиш через HID запросы и хранение настроек в EEPROM
- матрица кнопок 4x4
- антидребезговый фильтр на клавиши
- возможность работы от внешнего кварца на 16 МГц, и от внутреннего RC генератора с подстройкой

Недостатки:
- Алгоритм не умеет вырезать незначащие биты на приёме, поэтому, если USB хост назначает девайсу на шине адрес 0xXF, девайс не способен корректно принимать пакеты, и отвечает хосту в духе "моя твоя не понимать", после чего хост сбрасывает девайс, и назначает ему другой адрес. Для юзера это проходит незаметно.
- Пины порта С можно использовать только как GPIO входы.

Среда разработки - STVD 4.3.11

Компилятор - Cosmic cross compiler v.4.4.7

Схема: https://github.com/BBS215/STM8S-VUSB-KEYBOARD/blob/master/SCHEME/Scheme.pdf

При запуске прошивки, следующие настройки OPTION BYTE записываются автоматически:
1) AFR0: PORT C7 Alternate Function = TIM1_CH2
2) HSITRIM: 4 bit on-the-fly trimming

Для использования HSI, нужно установить #define USB_CLOCK_HSI в 1. После прошивки, контроллер будет пробовать подключиться к USB хосту при разных значениях HSI_TRIM_VAL, с периодичностью 8 секунд. При удачном подключении, рабочее значение HSI_TRIM_VAL будет сохраненно в EEPROM. Это значение в дальнейшем будет использоваться при перезапуске контроллера. Для повторной настройки HSI_TRIM_VAL, запишите нули в первые 2 байта EEPROM и перезапустите контроллер.

ПО для настройки клавиш: 
- https://github.com/BBS215/SetupKeys - консольная утилита
- https://github.com/BBS215/SetupKeysGUI - графическая утилита
-----------------------------------------------------------------
