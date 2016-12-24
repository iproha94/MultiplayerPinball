# Технопарк Mail.ru Group: Pinball multiplayer

<h3>Участники проекта</h3>
https://github.com/DmitryTsyganov физика
<br>
https://github.com/Evgeny-Ivanov меню
<br>
https://github.com/iproha94 клиент-сервер


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
если возникет ошибка "error while loading shared libraries: libfmod.so.6", то смотрите тут https://vidagnu.blogspot.ru/2016/02/cpp-tests-error-while-loading-shared.html
<br>
3.10 Возвращаемя в основную папку кокоса. Создание нового проекта: 
<br>
cocos new MyGame -p com.your_company.mygame -l cpp -d NEW_PROJECTS_DIR
<br>
3.11 Запуск
<br>
cocos run -s NEW_PROJECTS_DIR/MyGame -p linux
<br>
4. В NEW_PROJECTS_DIR/MyGame удалите папку Classes, Resources и файл CMakeLists.txt
<br>
5. Склонируйте данный репозиторий
<br>
5. Переместите папки Classes, Resources и файл CMakeLists.txt в папку NEW_PROJECTS_DIR/MyGame
<br>
6. Из папки Multiplayer переместите файлы, в которых нет слова "server" в папку Classes 

<h3>Полезные ссылки:</h3>
http://www.cocos2d-x.org/wiki/Linux_Installation_and_Setup
<br>
http://www.cocos2d-x.org/wiki/How_to_Start_A_New_Cocos2D-X_Game

<h3>Команды компиляции сервера</h3>
g++ -c -std=c++11 mainServerPinball.cpp serverPinball.cpp 
<br>
g++ -pthread -std=c++11 mainServerPinball.o serverPinball.o -o serexe

<h3>Команды компиляции клиентской части</h3>
(для тестирования клиента и сервера без игры)
<br>
g++ -c -std=c++11 mainClientPinball.cpp clientGameTwoPlayers.cpp 
<br>
g++ -pthread -std=c++11 mainClientPinball.o clientGameTwoPlayers.o -o cliexe

