# Технопарк Mail.ru Group: Pinball multiplayer

##Участники проекта
https://github.com/DmitryTsyganov физика  
https://github.com/Evgeny-Ivanov меню  
https://github.com/iproha94 клиент-сервер  

**Язык:** C++  
**Использвемые технологии на сервере:**  
1. Posix threads  
2. Posix sockets  


### Инструкция
1. Установите Ubuntu.  
(c Debian возникли проблемы при запуске скрипта *install-deps-linux.sh*, пишет что проблемы с OpenGL)  
2. Скачайте cocos2d-x-3.6   
(именно эта версия должна быть, более новые версия искажают физическое пространство для данной игры)  
3. Далее инструкция по [ссылке](https://habrahabr.ru/post/270133/)  
3.1 зайдите в папку с распакованным кокосом  
3.2 *./build/install-deps-linux.sh*  
3.3 *./tools/travis-scripts/install_glfw.sh*  
3.4 *./setup.py*  
3.5 *mkdir build/linux-build*  
3.6 *cd build/linux-build*  
3.7 *cmake ../..*  
3.8 *make*  
(на данном этапе будут выскакивать ошибки, связанные с isnan, в файлах, в которых происходят эти ошибки перед *isnan* напишите *std::*)  
3.9 *./bin/cpp-tests/cpp-tests*  
если возникет ошибка *error while loading shared libraries: libfmod.so.6*, то смотрите тут [ссылка](https://vidagnu.blogspot.ru/2016/02/cpp-tests-error-while-loading-shared.html)  
3.10 Возвращаемся в основную папку кокоса. Создание нового проекта:  
*cocos new MyGame -p com.your_company.mygame -l cpp -d NEW_PROJECTS_DIR*  
3.11 Запуск  
*cocos run -s NEW_PROJECTS_DIR/MyGame -p linux*  
4. В *NEW_PROJECTS_DIR/MyGame* удалите папку *Classes*, *Resources* и файл *CMakeLists.txt*  
5. Склонируйте данный репозиторий  
5. Переместите папки *Classes*, Resources и файл *CMakeLists.txt* в папку *NEW_PROJECTS_DIR/MyGame*  
6. Из папки *Multiplayer* переместите файлы, в которых нет слова "server" в папку *Classes* 

### Полезные ссылки:  
[ссылка 1](http://www.cocos2d-x.org/wiki/Linux_Installation_and_Setup)  
[ссылка 2](http://www.cocos2d-x.org/wiki/How_to_Start_A_New_Cocos2D-X_Game)  

**Команды компиляции сервера:**  
*g++ -c -std=c++11 mainServerPinball.cpp serverPinball.cpp*  
*g++ -pthread -std=c++11 mainServerPinball.o serverPinball.o -o serexe*  

**Команды компиляции клиентской части:**  
(для тестирования клиента и сервера без игры)  
*g++ -c -std=c++11 mainClientPinball.cpp clientGameTwoPlayers.cpp*   
*g++ -pthread -std=c++11 mainClientPinball.o clientGameTwoPlayers.o -o cliexe*  

