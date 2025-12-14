CC = gcc
CFLAGS = -O3 -Wall -Wextra

# ==========================================
# 路徑設定 (指向隔壁的 PQClean)
# ==========================================
PQ_DIR = ..
PQ_COMMON = $(PQ_DIR)/common
PQ_KEM = $(PQ_DIR)/crypto_kem/ml-kem-768/clean
PQ_SIGN = $(PQ_DIR)/crypto_sign/ml-dsa-44/clean
# 告訴編譯器要去哪裡找 .h 檔
# 關鍵：這裡必須包含 common 才能找到 randombytes.h
INCLUDES = -I./ -I$(PQ_COMMON) -I$(PQ_KEM) -I$(PQ_SIGN)
# ==========================================
# 需要編譯的物件檔
# ==========================================

# 1. 基礎工具 (雜湊、亂數)
COMMON_OBJS = $(PQ_COMMON)/fips202.o \
              $(PQ_COMMON)/randombytes.o

# 2. ML-KEM-768 演算法核心 (注意：這裡要列出該資料夾下所有 .c 對應的 .o)
KEM_OBJS = $(PQ_KEM)/kem.o \
           $(PQ_KEM)/indcpa.o \
           $(PQ_KEM)/polyvec.o \
           $(PQ_KEM)/poly.o \
           $(PQ_KEM)/ntt.o \
           $(PQ_KEM)/cbd.o \
           $(PQ_KEM)/reduce.o \
           $(PQ_KEM)/verify.o \
           $(PQ_KEM)/symmetric-shake.o

SIGN_OBJS = $(PQ_SIGN)/sign.o \
            $(PQ_SIGN)/packing.o \
            $(PQ_SIGN)/polyvec.o \
            $(PQ_SIGN)/poly.o \
            $(PQ_SIGN)/ntt.o \
            $(PQ_SIGN)/reduce.o \
            $(PQ_SIGN)/rounding.o\
			$(PQ_SIGN)/symmetric-shake.o
# 將所有依賴打包成變數
ALL_DEPS = $(COMMON_OBJS) $(KEM_OBJS) $(SIGN_OBJS)
# ==========================================
# 編譯目標
# ==========================================

.PHONY: all clean

all: server_sign client_sign eve_sign

# 連結執行檔
server_sign: server_sign.o $(ALL_DEPS)
	$(CC) $(CFLAGS) -o server_sign server_sign.o $(ALL_DEPS)

client_sign: client_sign.o $(ALL_DEPS)
	$(CC) $(CFLAGS) -o client_sign client_sign.o $(ALL_DEPS)
eve_sign: eve_sign.o $(ALL_DEPS)
	$(CC) $(CFLAGS) -o eve_sign eve_sign.o $(ALL_DEPS)

# ==========================================
# 通用編譯規則
# ==========================================

# 針對當前目錄的 .c
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 針對 PQClean common 目錄的 .c
$(PQ_COMMON)/%.o: $(PQ_COMMON)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 針對 PQClean KEM 目錄的 .c
$(PQ_KEM)/%.o: $(PQ_KEM)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PQ_SIGN)/%.o: $(PQ_SIGN)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:clean_key
	rm -f *.o server_sign client_sign eve_sign
	rm -f $(PQ_COMMON)/*.o
	rm -f $(PQ_KEM)/*.o

clean_exe:
	rm -f *.o *.key *.dat server_sign client_sign eve_sign
clean_key:
	rm -f *.key *.dat