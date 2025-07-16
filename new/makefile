# Makefile para compilar main.cpp y formater.cpp

# Nombre del ejecutable
TARGET = autolog

# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Librerías adicionales (asegúrate que CPR esté instalado)
LIBS = -lcpr

# Archivos fuente y objetos
SOURCES = main.cpp formater.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Regla para limpiar archivos temporales
clean:
	$(RM) $(TARGET) $(OBJECTS)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Regla automática para compilar archivos .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
