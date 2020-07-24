all:
	@printf "\e[32;1mCUrl confirure run.\e[0m\n"
	@git submodule update --init
	@autoreconf -i
	@automake
	@autoconf
	@./configure --without-zlib  --without-librtmp  --without-brotli  --without-nghttp2 --with-ssl=/usr/local/opt/openssl/
	@printf "\e[34;1mCUrl confirure finished.\e[0m\n"
	@printf "\e[32;1mCUrl build run.\e[0m\n"
	@make
	@printf "\e[34;1mCUrl build finished.\e[0m\n"
