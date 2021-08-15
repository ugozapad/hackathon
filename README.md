# hackathon

## БАГИ И ТОДО
* Камера вместо блокирования поворта на больше 89 градусов начинает просто накапливать поворот мыши.
* Физика не работает нормально, к тому же нужно будет видимо перевернуть матрицу bullet в OpenGL
* (СДЕЛАНО, ОТДЕЛЬНЫЙ ПРОХОД ДЛЯ СВЕТА) Рендерить источники света как сферы
* Скайбокс имеет какую-то ребристость (видимо с фильтрацией проблемы, мб выключить или включить...)
* (ПОЧТИ ГОТОВО) Файловая система (точнее, нужно для начала найти файл в архиве, а потом найти его на жд) 

### Engine features
* Deferred Shading
* Data-driven materials
* Node-based world and component-based game logic
* Integrated Bullet rigid-body physics

#### README FOR RELEASE

<Название игры>
Powered by <Название движка>

<Описание>

Управление:
WSAD - перемещение
Escape - выход в меню паузы (пока выход из игры)

Использованные библиотеки:
spdlog
GLFW
GLM
GLAD
AssImp
Im3D
ImGui
Bullet
Ozz Animation
FMOD

Фичи движка:

Deferred Shading (два прохода)
Физика на основа Bullet Physics
Entity-Component игровая логика


Требования:
видеокарта с поддержкой OpenGL 3.3

Рантайм студии 2019