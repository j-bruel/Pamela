///
/// \file unmount_container.c
/// \author jonathan.bruel
/// \version 1.0
/// \brief unmounts function for containers
/// \date Thu Nov 23 16:24:08 2017
///

#include <stdlib.h>
#include <stdio.h>
#include "pamela.h"

///
/// \fn static bool unmount_filesystem(const char *user_home)
/// \brief unmount a filesystem
/// \param user_home Unix user home directory
/// \return Status function
///
static bool	unmount_filesystem(const char *user_home)
{
  char		*cmd;

  asprintf(&cmd, "umount -f %s/%s", user_home, CONTAINER_NAME);
  return (system(cmd) != 0 ? false : true);
}

///
/// \fn static bool remove_device_mapping(const char *user_name)
/// \brief remove devince mapping, close file
/// \param user_home Unix user name
/// \return Status function
///
static bool	remove_device_mapping(const char *user_name)
{
  char		*cmd;

  asprintf(&cmd, "cryptsetup luksClose %s_device", user_name);
  return (system(cmd) != 0 ? false : true);
}

///
/// \fn static bool remove_directory(const char *user_home)
/// \brief Remove user home
/// \param user_home Unix user home directory
/// \return Status function
///
static bool	remove_directory(const char *user_home)
{
  char		*cmd;

  asprintf(&cmd, "rm -rf %s/%s", user_home, CONTAINER_NAME);
  return (system(cmd) != 0 ? false : true);
}

///
/// \fn bool unmount_container(const char *user_name)
/// \brief
/// \param user_home Unix user name
/// \return Status function
///
bool	unmount_container(const char *user_name)
{
  char	*user_home_directory;

  if ((user_home_directory = get_home_directory(user_name)) == NULL)
    return (false);
  if (!unmount_filesystem(user_home_directory) ||
      !remove_device_mapping(user_name) ||
      !remove_directory(user_home_directory))
    return (false);
  free(user_home_directory);
  return (true);
}
