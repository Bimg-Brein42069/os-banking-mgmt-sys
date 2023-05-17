BUILD_DIR   = ./build
BIN_DIR     = ./bin
INCLUDE_DIR = ./include
SRC_DIR = ./src

CC          = gcc -I $(INCLUDE_DIR)
LD          = gcc -I $(INCLUDE_DIR)

SEED_PROGRAM = seed
CLIENT_PROGRAM = client
SERVER_PROGRAM = server


SOURCES = $(wildcard $(SRC_DIR)/*.c)

CFLAG = -pthread

run: $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM)

seed: $(BIN_DIR)/$(SEED_PROGRAM)
	./$(BIN_DIR)/$(SEED_PROGRAM)

client: $(BIN_DIR)/$(CLIENT_PROGRAM)
	./$(BIN_DIR)/$(CLIENT_PROGRAM)

server: $(BIN_DIR)/$(SERVER_PROGRAM)
	./$(BIN_DIR)/$(SERVER_PROGRAM)

$(BIN_DIR)/$(SEED_PROGRAM): $(BUILD_DIR)/IMT2020129_seeds.o 
	$(LD) $(BUILD_DIR)/IMT2020129_seeds.o -o $(BIN_DIR)/$(SEED_PROGRAM)

$(BIN_DIR)/$(CLIENT_PROGRAM): $(BUILD_DIR)/IMT2020129_client.o 
	$(LD) $(BUILD_DIR)/IMT2020129_client.o -o $(BIN_DIR)/$(CLIENT_PROGRAM)

$(BIN_DIR)/$(SERVER_PROGRAM): $(BUILD_DIR)/IMT2020129_server.o $(BUILD_DIR)/IMT2020129_database.o
	$(LD) $(CFLAG) $(BUILD_DIR)/IMT2020129_server.o $(BUILD_DIR)/IMT2020129_database.o -o $(BIN_DIR)/$(SERVER_PROGRAM)


$(BUILD_DIR)/IMT2020129_server.o: $(SRC_DIR)/IMT2020129_server.c $(INCLUDE_DIR)/IMT2020129_data_structures.h $(INCLUDE_DIR)/IMT2020129_constants.h $(INCLUDE_DIR)/IMT2020129_database.h $(INCLUDE_DIR)/IMT2020129_server.h
	$(CC) -c $(SRC_DIR)/IMT2020129_server.c -o $(BUILD_DIR)/IMT2020129_server.o

$(BUILD_DIR)/IMT2020129_client.o: $(SRC_DIR)/IMT2020129_client.c $(INCLUDE_DIR)/IMT2020129_data_structures.h $(INCLUDE_DIR)/IMT2020129_constants.h  $(INCLUDE_DIR)/IMT2020129_client.h
	$(CC) -c $(SRC_DIR)/IMT2020129_client.c -o $(BUILD_DIR)/IMT2020129_client.o

$(BUILD_DIR)/IMT2020129_seeds.o: $(SRC_DIR)/IMT2020129_seeds.c $(INCLUDE_DIR)/IMT2020129_data_structures.h $(INCLUDE_DIR)/IMT2020129_constants.h $(INCLUDE_DIR)/IMT2020129_database.h
	$(CC) -c $(SRC_DIR)/IMT2020129_seeds.c -o $(BUILD_DIR)/IMT2020129_seeds.o

$(BUILD_DIR)/IMT2020129_database.o: $(INCLUDE_DIR)/IMT2020129_database.h $(INCLUDE_DIR)/IMT2020129_data_structures.h $(INCLUDE_DIR)/IMT2020129_constants.h
	$(CC) -c $(SRC_DIR)/IMT2020129_database.c -o $(BUILD_DIR)/IMT2020129_database.o


build: $(BIN_DIR)/$(PROG_NAME)

clean:
	rm -f $(BIN_DIR)/$(SEED_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM) $(BUILD_DIR)/*.o database/*