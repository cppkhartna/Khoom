Хартикова Анастасия, 231 группа.

Программа содержит две сцены, которые можно переключать клавишами 0 и 1.
Вместе с программой идут текстуры и набор объектов в текстовом формате.
Программа компилировалась на gcc 4.4.5 в ОС Debian GNU/Linux. Её
работа также проверялась на Archlinux. Требует библиотеку glew.

Сцена 0.

В кубе, символизирующем окружающее пространство, парит тор со
сферическим отражением.

Клавиши:
'f/F': имитация меха (слои)
'o/O': "поджигает" мех
'+/=': по каждому нажатию  - постепенное превращение тора в сферу (нетривиальный морфинг с изменением связности)
'-': обратный процесс
's/S': сразу тор -> сфера
'u/U': сразу сфера -> тор
Красивые эффекты: 
* меховой тор в меховую сферу
* меховая светящаяся сфера
'c/C': кубические текстуры в режиме окружающей среды 
Красивые эффекты: 
* Этот момент в сцене похож на мыльный пузырь. Медленное обратное
превращение в тор дополняет эту иллюзию. 
'b/B': отключает сферу 
* При превращении в тор лучше видно окружающую среду на нём.
't/T': bump mapping

Сцена 1.

Комната с зеркальным полом.

'f/F': слоистый вершинный туман по краям стен.
'l/L': свет.
'b/B': в комнате появляется полупрозрачный объект, требующий
упорядоченного вывода граней
'n/N': природа:
* первое окно выходит в горы, сгенерированные из карты высот map3.bmp
функцией landscape (в коде закомментирована, так как при загрузке
отнимает время) - рендеринг объёмных данных
'h/H' - горы в режиме wire/solid. Solid хорошо смотрится при отключенном
свете, wire - при включенном.
* второе окно вышло к лесу: ёлки из billboards
* третье окно выходит к океану: B-spline или NURBS поверхности, составляющие существенную часть сцены


Клавиши управления (для обеих сцен):
<Esc>  - выход
<Up> - вперёд
<Down>  - назад
<Left> - посмотреть влево
<Right> - посмотреть вправо
<PgUp> - посмотреть вверх
<PgDown> - посмотреть вниз
