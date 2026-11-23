/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "growbox_fw", "index.html", [
    [ "Сборка устройства Growbox PIPAS", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html", [
      [ "1. Перечень оборудования", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md2", [
        [ "1) Модуль реле с оптической развязкой, 5 В, 2 канала, AC 250 В / 30 А", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md3", null ],
        [ "2) Силовой ключ N-канал (Trema-модуль)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md4", null ],
        [ "3) Мембранный насос R385", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md5", null ],
        [ "4) Понижающий DC-DC преобразователь LM2596S", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md6", null ],
        [ "5) Arduino Uno R3 (Dccduino, USB CH340)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md7", null ],
        [ "6) Часы реального времени RTC RX-8025T (Trema-модуль)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md8", null ],
        [ "7) Модуль EEPROM AT24C256", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md9", null ],
        [ "8) Bluetooth-модуль HC-05", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md10", null ],
        [ "9) Датчик температуры и влажности DHT22 (модуль Waveshare)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md11", null ],
        [ "10) Блок питания 12 В / 2 А", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md12", null ],
        [ "11) Мембранная кнопка (5 шт, серая)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md13", null ]
      ] ],
      [ "2. Дополнительные компоненты (не входят в перечень выше)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md15", null ],
      [ "3. Схема распределения питания", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md17", [
        [ "Подпайка DC-DC к плате Arduino (питание через USB-B)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md18", null ]
      ] ],
      [ "4. Схема подключения сигналов", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md20", [
        [ "Подключение HC-05 (согласование уровней 5 В → 3,3 В)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md21", null ],
        [ "Подключение насоса (защита от ЭДС самоиндукции)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md22", null ],
        [ "Подключение релейных модулей", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md23", null ],
        [ "Подключение шины I²C (RTC и EEPROM)", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md24", null ],
        [ "Подключение датчиков", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md25", null ]
      ] ],
      [ "5. Итоговая карта выводов", "md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md27", null ]
    ] ],
    [ "Протокол взаимодействия с устройством через BLUETOOTH", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html", [
      [ "Общее описание протокола", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md29", null ],
      [ "Доступные команды", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md30", null ],
      [ "Положительное подтверждение приема данных (ACK)", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md31", null ],
      [ "Отрицательное подтверждение приема данных (NACK)", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md32", [
        [ "Получение общей информации", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md33", [
          [ "Формат маски устройств", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md34", null ]
        ] ],
        [ "Получить имя устройства и адрес устройства", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md35", null ],
        [ "Установить новое имя", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md36", null ],
        [ "Установить новый пароль", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md37", null ],
        [ "Получить текущее время", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md38", null ],
        [ "Установить время", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md39", null ],
        [ "Получить состояние устройства", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md40", [
          [ "Данные об устройстве, в зависимости от типа устройства", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md41", null ]
        ] ],
        [ "Изменить состояние устройства", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md42", null ],
        [ "Получить режим работы устройства", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md43", null ],
        [ "Установить режим работы устройства", "md_docs_2_b_t___p_r_o_t_o_c_o_l___r_u.html#autotoc_md44", null ]
      ] ]
    ] ],
    [ "Growbox PIPAS — описание работы проекта", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html", [
      [ "1. Назначение", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md47", null ],
      [ "2. Общая информация", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md49", null ],
      [ "3. Аппаратная часть", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md51", [
        [ "3.1. Датчики", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md52", null ],
        [ "3.2. Исполнительные механизмы", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md53", null ],
        [ "3.3. Bluetooth-модуль (HC-05)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md54", null ],
        [ "3.4. Прочее", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md55", null ]
      ] ],
      [ "4. Структура репозитория", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md57", null ],
      [ "5. Стартовая инициализация", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md59", null ],
      [ "6. Главный рабочий цикл", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md61", null ],
      [ "7. Алгоритм работы", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md63", [
        [ "7.1. Общая схема", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md64", null ],
        [ "7.2. Алгоритм инициализации (конструктор Controller)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md65", null ],
        [ "7.3. Алгоритм главного цикла (Controller::Process)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md66", null ],
        [ "7.4. Алгоритм опроса датчиков (readSensors)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md67", null ],
        [ "7.5. Алгоритм управления лампой", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md68", null ],
        [ "7.6. Алгоритм управления вентилятором", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md69", null ],
        [ "7.7. Алгоритм управления насосом", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md70", null ],
        [ "7.8. Алгоритм управления увлажнителем", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md71", null ],
        [ "7.9. Алгоритм чтения и записи конфигурации (Storage)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md72", null ],
        [ "7.10. Алгоритм приёма пакетов Bluetooth (конечный автомат)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md73", null ]
      ] ],
      [ "8. Маска устройств", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md75", null ],
      [ "9. Логика автоматического управления", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md77", [
        [ "9.1. Лампа", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md78", null ],
        [ "9.2. Вентилятор", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md79", null ],
        [ "9.3. Насос (полив)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md80", null ],
        [ "9.4. Увлажнитель воздуха", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md81", null ]
      ] ],
      [ "10. Конфигурация", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md83", [
        [ "10.1. Хранение в EEPROM (wear leveling)", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md84", null ]
      ] ],
      [ "11. Управление по Bluetooth", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md86", [
        [ "11.1. Режимы работы модуля", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md87", null ],
        [ "11.2. Энергосбережение", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md88", null ],
        [ "11.3. Обмен пакетами", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md89", null ],
        [ "11.4. Команды протокола", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md90", null ]
      ] ],
      [ "12. Режимы работы устройства", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md92", null ],
      [ "13. Замечания и ограничения текущей реализации", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md94", null ],
      [ "14. Сборка и прошивка", "md_docs_2_o_v_e_r_v_i_e_w___r_u.html#autotoc_md96", null ]
    ] ],
    [ "Growbox PIPAS", "md__r_e_a_d_m_e.html", [
      [ "Возможности", "md__r_e_a_d_m_e.html#autotoc_md98", null ],
      [ "Аппаратная часть", "md__r_e_a_d_m_e.html#autotoc_md99", null ],
      [ "Структура репозитория", "md__r_e_a_d_m_e.html#autotoc_md100", null ],
      [ "Документация", "md__r_e_a_d_m_e.html#autotoc_md101", null ],
      [ "Сборка и прошивка", "md__r_e_a_d_m_e.html#autotoc_md102", null ],
      [ "Лицензия", "md__r_e_a_d_m_e.html#autotoc_md103", null ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"air_8cpp.html",
"md_docs_2_a_s_s_e_m_b_l_i_n_g___r_u.html#autotoc_md27"
];

const SYNCONMSG = 'click to disable panel synchronization';
const SYNCOFFMSG = 'click to enable panel synchronization';
const LISTOFALLMEMBERS = 'List of all members';