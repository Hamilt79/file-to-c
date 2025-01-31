target := f2c

all:
	mkdir -p build
	$(CXX) main.c $(CXXFLAGS) -o ./build/$(target)

install: all
	mv ./build/$(target) /usr/bin

install-local: all
	mv ./build/$(target) ~/.local/bin

uninstall:
	rm /usr/bin/$(target)

uninstall-local:
	rm ~/.local/bin/$(target)
