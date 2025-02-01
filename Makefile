target := f2c

all:
	mkdir -p build
	$(CXX) main.c $(CXXFLAGS) -o ./build/$(target)

install: all
	mkdir -p ~/.local/bin
	mv ./build/$(target) /usr/bin

install-local: all
	mkdir -p ~/.local/bin
	mv ./build/$(target) ~/.local/bin

uninstall:
	rm /usr/bin/$(target)

uninstall-local:
	rm ~/.local/bin/$(target)
