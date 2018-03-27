///
/// \file mount_container.c
/// \author jonathan.bruel
/// \version 1.0
/// \brief mount function for containers
/// \date Thu Nov 23 12:42:11 2017
///

#include <stdlib.h>
#include <stdio.h>
#include "pamela.h"

///
/// \fn static bool create_and_format_filesystem(const char *user_name)
/// \brief Create and format a filesystem
/// \param user_name Unix user name
/// \return Status function
///
static bool	create_and_format_filesystem(const char *user_name)
{
  char		*cmd;
  bool		ret;

  asprintf(&cmd, "mkfs.ext4 /dev/mapper/%s_device", user_name);
  ret = (system(cmd) != 0 ? false : true);
  free(cmd);
  return (ret);
}

///
/// \fn static bool create_mount_location(const char *user_name)
/// \brief Create mount location
/// \param user_name Unix user name
/// \return Status function
///
static bool	create_mount_location(const char *user_name)
{
  char		*cmd;
  bool		ret;

  asprintf(&cmd, "mkdir %s/%s", get_home_directory(user_name), CONTAINER_NAME);
  ret = (system(cmd) != 0 ? false : true);
  printf("[My PAMela]: Create a mount location.\n");
  free(cmd);
  return (ret);
}

///
/// \fn static bool mount_filesystem(const char *user_name)
/// \brief Mount filesystem
/// \param user_name Unix user name
/// \return Status function
///
static bool	mount_filesystem(const char *user_name)
{
  char		*cmd;
  bool		ret;

  asprintf(&cmd, "mount /dev/mapper/%s_device %s/%s", user_name, get_home_directory(user_name), CONTAINER_NAME);
  ret = (system(cmd) != 0 ? false : true);
  printf("[My PAMela]: Mount with sucess.\n");
  free(cmd);
  return (ret);
}

///
/// \fn static bool change_own(const char *user_name)
/// \brief Change own directory
/// \param user_name Unix user name
/// \return Status function
///
static bool	change_own(const char *user_name)
{
  char		*cmd_own;
  bool		ret;
  char		*container_path;
  char		*crypted_container_file_path;
  char		*cmd_chmod_container;
  char		*cmd_chmod_crypted_container;

  if ((container_path = get_container_path(user_name)) == NULL ||
      (crypted_container_file_path = get_crypt_file_path(user_name)) == NULL)
    return (false);
  asprintf(&cmd_own, "chown %s %s", user_name, container_path);
  asprintf(&cmd_chmod_container, "chmod u=rwx,g=,o= %s", container_path);
  asprintf(&cmd_chmod_crypted_container, "chmod u=rw,g=,o= %s", crypted_container_file_path);
  ret = ((system(cmd_own) != 0 || system(cmd_chmod_container) ||
	  system(cmd_chmod_crypted_container)) ? false : true);
  printf("[My PAMela]: Change own & right to partition.\n");
  free(cmd_own);
  free(crypted_container_file_path);
  free(cmd_chmod_container);
  free(cmd_chmod_crypted_container);
  return (ret);
}

///
/// \fn bool mount_container(const char *user_name, bool is_already_created)
/// \brief Mount container
/// \param user_name Unix user name
/// \return Status function
///
bool     mount_container(const char *user_name, bool is_already_created)
{
  if (!is_already_created && !create_and_format_filesystem(user_name))
    return (false);
  if (!create_mount_location(user_name) ||
      !mount_filesystem(user_name) ||
      !change_own(user_name))
    return (false);
  return (true);
}
