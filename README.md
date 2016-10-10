# MultiplayerPinball
проект весна 2015 г.

<h3>Инструкция</h3>
1. Установите Ubuntu. 
<br>
(c Debian возникли проблемы при запуске скрипта install-deps-linux.sh, пишет что проблемы с OpenGL)
<br>
2. Скачайте cocos2d-x-3.6 
<br>
(именно эта версия должна быть, более новые версия искажают физическое пространство для данной игры)
<br>
3. Далее инструкция по https://habrahabr.ru/post/270133/
<br>
3.1 зайдите в папку с распакованным кокосом
<br>
3.2 ./build/install-deps-linux.sh 
<br>
3.3 ./tools/travis-scripts/install_glfw.sh 
<br>
3.4 ./setup.py
<br>
3.5 mkdir build/linux-build
<br>
3.6 cd build/linux-build
<br>
3.7 cmake ../..
<br>
3.8 make 
<br>
(на данном этапе будут выскакивать ошибки, связанные с isnan, в файлах, в которых происходят эти ошибки перед isnan напишите "std::" без кавычек)
<br>
3.9 ./bin/cpp-tests/cpp-tests
<br>

<br>

<br>

<br>
