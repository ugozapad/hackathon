# hackathon

## БАГИ И ТОДО
* Камера вместо блокирования поворта на больше 89 градусов начинает просто накапливать поворот мыши.
* Физика не работает нормально, к тому же нужно будет видимо перевернуть матрицу bullet в OpenGL
* Нету дебаг рендера
* Нету графа сцены
* Рендерить источники света как сферы
* Скайбокс имеет какую-то ребристость (видимо с фильтрацией проблемы, мб выключить или включить...)

### Engine features
* Deferred Shading
* Data-driven materials
* Node-based world and component-based game logic
* Integrated Bullet rigid-body physics

#### README FOR RELEASE

<Название игры>
Powered by <Название движка>

<Описание>

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