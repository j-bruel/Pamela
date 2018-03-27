///
/// \file pamela.h
/// \author jonathan.bruel
/// \version 1.0
/// \brief Pamela header
/// \date Wed Nov 22 10:01:32 2017
///

#ifndef PAMELA_H_
# define PAMELA_H_

# include <stdbool.h>

///
/// \def ERR_NO_HOME_DIR
/// \brief Error when a user as no home directory
///
# define ERR_NO_HOME_DIR	"[Error] Pamela: User as no home directory, impossible to open or create a safe container."
///
/// \def ERR_NOT_SUDO
/// \brief Error while user is not sudo
///
# define ERR_NOT_SUDO		"[Error] Pamela: You must be sudo to create/open a container.\n"
///
/// \def CONTAINER_NAME
/// \brief Container name
///
# define CONTAINER_NAME		"secure_data-rw"
///
/// \def CONTAINER_SIZE
/// \brief Define the size of the container
///
# define CONTAINER_SIZE		512
///
/// \def DD_DEV_ZERO_CMD
/// \brief write zeroes to the entire area that we are provisioning to our file by using the /dev/zero pseudo-device
///
# define DD_DEV_ZERO_CMD	"dd if=/dev/zero bs=1M count=%d of=%s"
///
/// \def FALLOCATE_CMD
/// \brief Create a empty sparse file
/// \brief will not overwrite whatever old, deleted data that used to be used by those blocks with zeros or random data.
///
# define FALLOCATE_CMD		"fallocate -l %d %s"
///
/// \def DD_URANDOM_CMD
/// \brief Write random data on file
///
# define DD_URANDOM_CMD		"dd if=/dev/urandom of=%s bs=1M count=%d"
///
/// \def DD_RANDOM_CMD
/// \brief Same as DD_URANDOM_CMD but more secure and less faster
///
# define DD_RANDOM_CMD		"dd if=/dev/random of=%s bs=1M count=%d"
///
/// \def CRYPT_FILE_NAME
/// \brief container crypted into file (name)
///
# define CRYPT_FILE_NAME	".crypted_secure_data-rw"

///
/// \enum empty_container_file_mode
/// \brief Define the type of creation of empty container file
///
typedef enum    e_empty_container_file_mode
  {
    DD_MODE,
    FALLOCATE
  }		t_empty_container_file_mode;

///
/// \enum random_container_mode
/// \brief Define the mode of create of random container mode
///
typedef enum	e_random_container_mode
  {
    DD_URANDOM_MODE,
    DD_RANDOM_MODE
  }		t_random_container_mode;

/*
** container_handler.c
*/
char	*get_home_directory(const char *user_name);
char	*get_crypt_file_path(const char *user_name);
char	*get_container_path(const char *user_name);
bool	open_container(const char *user_name,
		       const char *user_password,
		       char **path);
bool	create_container(const char *user_name,
			 const char *user_password,
			 const char *path);
bool	change_password_from_existing_container(const char *user_name,
						const char *old_user_password,
						const char *new_user_password);

/*
** cryptsetup_cmd.c
*/
bool	create_empty_container_file(const char *path,
				    t_empty_container_file_mode mode);
bool	create_random_container_file(const char *path,
				     t_random_container_mode mode);

/*
** encryption_handler.c
*/
bool	create_crypted_container_file(const char *user_name,
				      const char *user_passwor);
bool	open_crypted_container_file(const char *user_name,
				    const char *user_passwor);

/*
** mount/unmount_container.c
*/
bool	mount_container(const char *user_name, bool is_already_created);
bool	unmount_container(const char *user_name);

#endif /* !PAMELA_H_ */
