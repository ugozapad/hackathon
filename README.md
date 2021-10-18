# Failure game
![Screnshot](https://ugozapad.github.io/images/small.png)

## Ссылка на проект
https://itch.io/jam/gaijin-jam/rate/1161714

### История
Очень простая технодемка, из которой мы пытались сделать что-то стоящее. 
Движок игры был создан за три недели, в итоге архитектура скатилась в что-то ужасное и некоторые фичи просто сломаны.

#### Технологии:
* OpenGL 3.3 рендер
* Deferred Shading (две фазы, одна на геометрию, вторая свет как пост-процессинг)
* Физика основана на Bullet Physics
* Простая звуковая система на FMOD
* Entity-Component игровая логика
* Файловая система и загрузка контента в отдельном потоке

##### Библиотеки
* spdlog
* GLFW
* GLM
* GLAD
* AssImp
* Im3D
* ImGui
* Bullet
* Ozz Animation
* FMOD
