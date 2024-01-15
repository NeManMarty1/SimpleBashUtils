#!/bin/bash

# Создаем файл с тестовыми данными
cat > test.txt << EOF
This is a test file
It contains some lines
Some of them match a pattern
Others do not
EOF
# Логируем создание файла
logger -t "Name.sh[$LINENO]" "Created test file"

# Объявляем переменные для хранения результатов
original_grep_output="" # вывод оригинального grep
s21_grep_output="" # вывод твоей программы
correct_tests=0 # счетчик правильных тестов
total_tests=0 # счетчик всех тестов

# Проверяем наличие папки для результатов
if [ ! -d results ]; then
  # Создаем папку, если ее нет
  mkdir results
  # Логируем создание папки
  logger -t "Name.sh[$LINENO]" "Created results folder"
fi

# Функция для сравнения выводов двух программ
compare_outputs () {
  # Увеличиваем счетчик всех тестов на 1
  ((total_tests++))
  # Если выводы совпадают, то увеличиваем счетчик правильных тестов на 1 и выводим сообщение
  if [ "$original_grep_output" == "$s21_grep_output" ]; then
    ((correct_tests++))
    echo "Test passed: $@"
    # Логируем успешный тест
    logger -t "Name.sh[$LINENO]" "Test passed: $@"
  # Иначе выводим сообщение об ошибке и показываем различия
  else
    echo "Test failed: $@"
    echo "Original grep output:"
    echo "$original_grep_output"
    echo "s21_grep output:"
    echo "$s21_grep_output"
    # Логируем неудачный тест
    logger -t "Name.sh[$LINENO]" "Test failed: $@"
  fi
}

# Функция для запуска теста с заданными параметрами
run_test () {
  # Запускаем оригинальный grep с переданными параметрами и сохраняем вывод
  original_grep_output="$(grep $@ test.txt)"
  # Запускаем твою программу с переданными параметрами и сохраняем вывод
  s21_grep_output="$(./s21_grep $@ test.txt)"
  # Сравниваем выводы
  compare_outputs $@
  # Сохраняем выводы в папку results
  echo "$original_grep_output" > results/original_grep_$@.txt
  echo "$s21_grep_output" > results/s21_grep_$@.txt
  # Логируем сохранение выводов
  logger -t "Name.sh[$LINENO]" "Saved outputs for $@"
}

# Запускаем тесты с разными флагами и шаблонами
run_test -e test # поиск строки, соответствующей шаблону test
run_test -i TEST # поиск строки, соответствующей шаблону TEST без учета регистра
run_test -v test # поиск строки, не соответствующей шаблону test
run_test -c test # подсчет количества строк, соответствующих шаблону test
run_test -l test # вывод имени файла, содержащего строку, соответствующую шаблону test
run_test -n test # вывод номера строки и строки, соответствующей шаблону test
run_test -f test.txt # вывод номера строки и строки, соответствующей шаблону test
run_test -o test # вывод номера строки и строки, соответствующей шаблону test
run_test -s test # вывод номера строки и строки, соответствующей шаблону test
run_test -h test # вывод номера строки и строки, соответствующей шаблону test
run_test -iv test # вывод номера строки и строки, соответствующей шаблону test

# Выводим итоговый результат
echo "Total tests: $total_tests"
echo "Correct tests: $correct_tests"
# Логируем итоговый результат
logger -t "Name.sh[$LINENO]" "Total tests: $total_tests"
logger -t "Name.sh[$LINENO]" "Correct tests: $correct_tests"
