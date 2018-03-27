##
## Makefile for Makefile in /home/bruel_a/rendu/Pamela
##
## Made by jonathan bruel
## Login   <bruel_a@epitech.net>
##
## Started on  Thu Nov 20 10:45:14 2017 jonathan bruel
## Last update Sat Nov 25 18:17:05 2017 bruel_a
##

##
## !*********!
## ! Sources !
## !*********!
##

SRC =		src/pam_overload.c \
		src/container_handler.c \
		src/cryptsetup_cmd.c \
		src/encryption_handler.c \
		src/mount_container.c \
		src/unmount_container.c

##
## !*********!
## !  Utils  !
## !*********:
##

NAME =		lib/pamela.so

CC =		gcc

RM =		rm -rf

OBJS =		$(SRC:.c=.o)

##
## !*********!
## !  Flags  !
## !*********!
##

CFLAGS =	-I./include -D_GNU_SOURCE
CFLAGS +=	-W -Wall -Wextra
CFLAGS +=	-fPIC -DPIC

LDFLAGS =	-shared -rdynamic -lcryptsetup

##
## !***************!
## ! Instructions  !
## !***************!
##

all: 		$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(LDFLAGS) -o $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY: 	all clean fclean re

install:
ifneq ("$(wildcard /lib/security)", "")
	@printf "Pamela is already install."
else
	apt-get install -y gcc make cryptsetup libpam0g-dev libcryptsetup-dev
	make
	mkdir /lib/security
	cp ${NAME} /lib/security/
	@echo "auth    optional                      pamela.so" >> /etc/pam.d/common-auth
	@echo "session	optional	pamela.so" >> /etc/pam.d/common-session
	@echo "account	optional	pamela.so" >> /etc/pam.d/common-account
	@echo "password	optional	pamela.so" >> /etc/pam.d/common-password
	@printf "Pamela is installed."

endif

uninstall:
ifeq ("$(wildcard /lib/security), """)
	@printf "Pamela is not installed, Please install it before unistall."
else
	make fclean
	$(RM) /lib/security
	@head -n -1 /etc/pam.d/common-auth > /tmp/tmp-auth
	@head -n -1 /etc/pam.d/common-session > /tmp/tmp-session
	@head -n -1 /etc/pam.d/common-account > /tmp/tmp-account
	@head -n -1 /etc/pam.d/common-password > /tmp/tmp-password
	@mv /tmp/tmp-auth /etc/pam.d/common-auth
	@mv /tmp/tmp-session /etc/pam.d/common-session
	@mv /tmp/tmp-account /etc/pam.d/common-account
	@mv /tmp/tmp-password /etc/pam.d/common-password
	@printf "Pamela is uninstalled"
endif

check:
ifneq ("$(wildcard /lib/security)", "")
	@printf "Pamela is install."
else
	@printf "Pamela is not install."
endif

test:
	./test-pam/test.sh
