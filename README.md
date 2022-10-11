# Proyecto Estadistica


## Dependencias
* [cmake](https://cmake.org/)
* [SFML](https://github.com/SFML/SFML)
* [fmt](https://github.com/fmtlib/fmt)
* [DearImGui](https://github.com/ocornut/imgui)
* [ImPlot](https://github.com/epezent/implot)
* [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml)

## Uso
Para poder usar este codigo instala las dependencias de arriba y descarga este repositorio, navega a este usando el simbolo del sistema o la shell de tu preferencia y escribe los siguientes comandos:

```
mkdir build
cd build
cmake ..
make
```

NOTAS:
* Si tienes problemas instalando ImGui-SFML, recomendaria que instales SFML con librerias estaticas(el default es compartidas), esto es, tu cmake debe de verse algo asi cuando instales SFML.
```
cmake .. -DBUILD_SHARED_LIBS=OFF
```
* Especifica el directorio donde descargaste ImGui cuando vayas a compilar ImGui-SFML, tal y como lo explica el [repositorio](https://github.com/eliasdaler/imgui-sfml).
* Pon todos los archivos .cpp, .h de [Implot](https://github.com/epezent/implot), en el mismo directorio de DearImgui y ponlos. 
