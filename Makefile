CC= cc 
CFLAGS= -Wall -Wextra -Werror

TARGET= pipex
BONUS_TARGET= pipex_bonus

P_LIB_FILE= libpipex.h
P_LIB_NAME= libpipex.a
BONUS_P_LIB_NAME= libpipex_bonus.a

LIBFT_DIR= ./my_libft
LIBFT_NAME= $(LIBFT_DIR)/libft.a

SRC_FILES= pipex.c pipex_utils.c get_path.c
BONUS_FILES= main_bonus.c pipex_bonus.c pipex_utils.c get_path.c

OBJ_DIR= ./obj
OBJ_FILES= $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))
BONUS_OBJ_FILES= $(addprefix $(OBJ_DIR)/,$(BONUS_FILES:.c=.o))

all: $(TARGET)

$(P_LIB_NAME): $(OBJ_FILES)
	ar rcs $@ $(OBJ_FILES)

$(TARGET): $(OBJ_FILES) $(P_LIB_NAME) $(LIBFT_NAME)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) -L. -lpipex -L$(LIBFT_DIR) -lft 

$(LIBFT_NAME):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c $(P_LIB_FILE) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

bonus: $(BONUS_TARGET) $(BONUS_P_LIB_NAME)

$(BONUS_TARGET): $(BONUS_OBJ_FILES) $(BONUS_P_LIB_NAME) $(LIBFT_NAME)
	$(CC) $(CFLAGS) -o $@ $(BONUS_OBJ_FILES) -L. -lpipex_bonus -L$(LIBFT_DIR) -lft

$(BONUS_P_LIB_NAME): $(BONUS_OBJ_FILES)
	ar rcs $@ $(BONUS_OBJ_FILES)

clean:
	@if [ -d $(OBJ_DIR) ]; then rm -rf $(OBJ_DIR); fi 
	@make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(TARGET) $(BONUS_TARGET) $(P_LIB_NAME) $(BONUS_P_LIB_NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re clean_main bonus