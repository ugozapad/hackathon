# Failure game
![Screnshot](https://ugozapad.github.io/images/small.png)

## Ссылка на проект
https://itch.io/jam/gaijin-jam/rate/1161714

### История
Простая технодемка, из которой мы пытались сделать что-то стоящее. Но не успели из-за отсутствия навыков и достаточного времени для доработки и или исправления багов. Сам движок писался с расчетом на большее, но из-за маленького срока скатился в некоторый хардкод. 
Движок игры был создан за три недели.

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
