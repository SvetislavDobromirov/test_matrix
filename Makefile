CC = gcc
CFLAGS:= -Wall -Werror -Wextra -g
GCOV_FLAGS := -fprofile-arcs -ftest-coverage
LDFLAGS := -fprofile-arcs --coverage
LIN_FLAGS := -lpthread -pthread -lrt -lm -lsubunit
LCHECK= -lcheck
ifeq ($(shell uname), Linux)
CHECK_FLAGS += $(LIN_FLAGS)
endif

SOURCES:= s21_matrix.c
OBJECTS := $(addprefix obj/,$(SOURCES:.c=.o))
SOURCES_DEBUG:= debug.c
OBJECTS_DEBUG:= $(addprefix obj/,$(SOURCES_DEBUG:.c=.o))
OBJECTS_GCOV := $(addprefix obj_gcov/,$(SOURCES:.c=.o))
HEADER:= s21_matrix.h debug.h
OBJ_DIR     := obj
OBJ_GCOV_DIR:= obj_gcov
CHECKMK_FILE:= matrix.check
TEST_EXE:=test_test.out

all: clean style test

create_dir:
	@mkdir -p $(OBJ_DIR) $(OBJ_GCOV_DIR)

$(OBJ_DIR)/%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $(subst /,_,$@) $(CHECK_FLAGS)
	@mv *.o $(OBJ_DIR)/

s21_matrix.a: create_dir $(OBJECTS)
	ar rc $@ $(OBJ_DIR)/*.o

debug.a: create_dir $(OBJECTS_DEBUG)
	ar rc $@ $(OBJ_DIR)/*.o

matrix_tests.c:
	@checkmk clean_mode=0 $(CHECKMK_FILE) > matrix_tests.c

check:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test_test.out
#valgrind --leak-check=full ./s21_matrix





main: s21_matrix.a debug.a
	$(CC) $(CFLAGC) main.c $^ -o main.out
	./main.out
# 
test: matrix_tests.c $(HEADER) s21_matrix.a debug.a
	$(CC) $(CFLAGS) $^ -o $(TEST_EXE) $(LCHECK) $(CHECK_FLAGS)
	@./$(TEST_EXE)

$(OBJ_GCOV_DIR)/%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $(subst /,_,$@) $(CHECK_FLAGS)
	mv *.o *gcno $(OBJ_GCOV_DIR)/

libgcov.a: create_dir $(OBJECTS_GCOV)
	ar rcs libgcov.a $(OBJ_GCOV_DIR)/*.o

gcov_report: matrix_tests.c $(HEADER) libgcov.a
	$(CC) $(CFLAGS) $^ -o $(TEST_EXE) $(LCHECK) $(LDFLAGS) $(GCOV_FLAGS) $(CHECK_FLAGS)
	./$(TEST_EXE)
	@mv *.gcda *.gcno $(OBJ_GCOV_DIR)
	@rm -f $(OBJ_GCOV_DIR)/*tests*
	@lcov/bin/./lcov -c -d $(OBJ_GCOV_DIR)/. -o $(OBJ_GCOV_DIR)/coverage.info
	@lcov/bin/./genhtml $(OBJ_GCOV_DIR)/coverage.info --output-directory out
	open out/index.html

style:
	@cp ../materials/linters/.clang-format ./.clang-format
#	@clang-format -n *.c
	@clang-format -i *.c
	@rm .clang-format

leaks: clean test
	leaks --atExit -- ./$(TEST_EXE)

valgrind:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(TEST_EXE)

clean:
	@rm -rf *.a *.gcda *.gcno *.o *.out
	rm -rf $(OBJ_DIR) $(OBJ_GCOV_DIR) out
	rm -rf matrix_tests.c
