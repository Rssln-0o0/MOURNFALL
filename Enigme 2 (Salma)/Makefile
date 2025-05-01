
EXECUTABLE = jeu_enigmes

# الكومبايلر اللي باش نستعملوه (عادةً GCC)
CC = gcc

# أعلام الكومبايلر (خيارات إضافية)
CFLAGS = -Wall -O2

# أعلام الربط (link flags) للمكتبات اللي نستعملوا فيها
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf

# الملفات المصدرية اللي عندنا (.c files)
SOURCES = main.c enigme.c chronometre.c

# الملفات الهدف (.o files) اللي باش يتكونوا من الملفات المصدرية
OBJECTS = $(SOURCES:.c=.o)

# قاعدة بناء البرنامج القابل للتنفيذ
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

# قاعدة بناء الملفات الهدف (.o files)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# قاعدة التنظيف (باش نمسحوا الملفات اللي تكونت)
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
