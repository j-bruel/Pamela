///
/// \file container_handler.c
/// \author jonathan.bruel
/// \version 1.0
/// \brief container handler
/// \date Wed Nov 22 09:42:13 2017
///

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <libcryptsetup.h>
#include "pamela.h"

///
/// \fn char *get_home_directory(const char *user_name)
/// \brief Extract the home directory path of a user
/// \param user_name Unix user name
/// \return Home directory path
///
char		*get_home_directory(const char *user_name)
{
  struct passwd	*passwdEnt;
  char		*home_dir;

  home_dir = getenv("HOME");
  if (home_dir == NULL)
    {
      if ((passwdEnt = getpwnam(user_name)) == NULL)
	return (NULL);
      return (passwdEnt->pw_dir);
    }
  return (home_dir);
}

///
/// \fn static char *get_crypt_file_path(const char *user_name)
/// \brief Generate the user crypt file (container crypted)
/// \param user_name Unix user name
/// \return Crypt file container path
///
char		*get_crypt_file_path(const char *user_name)
{
  char		*path;

  asprintf(&path, "%s/%s", get_home_directory(user_name), CRYPT_FILE_NAME);
  return (path);
}

///
/// \fn static char *get_container_path(const char *user_name)
/// \brief Extract the path of the user container
/// \param user_name Unix user name
/// \return Container path
///
char		*get_container_path(const char *user_name)
{
  char		*path;

  asprintf(&path, "%s/%s", get_home_directory(user_name), CONTAINER_NAME);
  return (path);
}

///
/// \fn static bool is_file_exist(const char *file_path)
/// \brief Said if a file exist or not
/// \param file_path File path to check
/// \return Status function
///
static bool	is_file_exist(const char *file_path)
{
  return (access(file_path, F_OK) != -1 ? true : false);
}

///
/// \fn char *open_container(const char *user_name, const char *user_password, char **path)
/// \brief Open a conntainer
/// \param user_name Unix user name
/// \param user_password Unix user password
/// \param path Directory path for his container
/// \return Status function
///
bool	open_container(const char *user_name,
		       const char *user_password,
		       char **path)
{
  printf("[My PAMela]: Open Container\n");
  if ((*path = get_crypt_file_path(user_name)) == NULL)
    {
      fprintf(stderr, ERR_NO_HOME_DIR);
      return (false);
    }
  printf("[My PAMela]: User : %s\n", user_name);
  printf("[My PAMela]: Crypted container path : %s\n", *path);
  if (!is_file_exist(*path))
    return (false);
  if (!open_crypted_container_file(user_name, user_password) ||
      !mount_container(user_name, true))
    return (false);
  printf("[My PAMela]: Open & mount with success\n");
  return (true);
}

///
/// \fn bool create_container(const char *user_name, const char *user_password, const char *path);
/// \brief Create a new container
/// \param user_name Unix user name
/// \param user_password Unix user password
/// \param path Directory path for his container
/// \return Status function
///
bool	create_container(const char *user_name,
			 const char *user_password,
			 const char *path)
{
  printf("[My PAMela]: Create container\n");
  printf("[My PAMela]: Crypt file path: %s\n", path);
  printf("[My PAMela]: User name: %s\n", user_name);
  if (!create_random_container_file(path, DD_URANDOM_MODE) ||
      !create_crypted_container_file(user_name, user_password) ||
      !open_crypted_container_file(user_name, user_password) ||
      !mount_container(user_name, false))
    return (false);
  printf("[My PAMela]: Container create with success.\n");
  return (true);
}

///
///
/// \fn bool change_password_from_existing_container(const char *user_name, const char *old_user_password, const char *new_user_password)
/// \brief Change user password handler
/// \param user_name Unix user name
/// \param old_user_password Unix user password before change
/// \param new_user_password Unix user password after change
///
bool	change_password_from_existing_container(const char *user_name,
						const char *old_user_password,
						const char *new_user_password)
{
  struct crypt_device   *cryptdevice;
  char			*container_path;

  if ((container_path = get_crypt_file_path(user_name)) == NULL)
    return (false);
  if (crypt_init(&cryptdevice, container_path) < 0 ||
      crypt_load(cryptdevice, CRYPT_LUKS1, NULL) < 0 ||
      crypt_keyslot_add_by_passphrase(cryptdevice, CRYPT_ANY_SLOT,
				      old_user_password, strlen(old_user_password),
				      new_user_password, strlen(new_user_password)) < 0)
    return (false);
  crypt_free(cryptdevice);
  return (true);
}
