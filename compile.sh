emcc \
-sEXPORT_ES6 \
-sENVIRONMENT=web,worker \
-sALLOW_MEMORY_GROWTH=1 \
-sMAXIMUM_MEMORY=4GB \
-pthread \
-sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency \
-lembind \
-Oz \
-std=c++17 \
--emit-tsd lib.d.ts \
./src/lib.cpp \
-o ./dist/lib.js