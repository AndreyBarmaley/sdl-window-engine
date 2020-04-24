# makefile
# libSDL C++ wrapper engine

all:
	rm -f libswe.a libswe.so
	rm -f dist/libswe.a dist/libswe.so
	+make -C dist
	ln -fs dist/libswe.a .

libswe.a:
	+make -C dist libswe.a
	ln -fs dist/libswe.a .

libswe.so:
	+make -C dist libswe.so
	ln -fs dist/libswe.so .

clean:
	make clean -C dist
	rm -f libswe.a libswe.so
