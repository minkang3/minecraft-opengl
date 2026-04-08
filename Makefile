all:
	em++ -g0 -o ./index.html src/*.cpp -I./include \
		-s USE_WEBGL2=1 -s FULL_ES3=1 \
		-s ALLOW_MEMORY_GROWTH=1 \
		--preload-file assets --preload-file shaders \
		-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2
