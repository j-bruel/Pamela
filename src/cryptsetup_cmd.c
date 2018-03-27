///
/// \file cryptsetup_cmd.c
/// \author jonathan.bruel
/// \version 1.0
/// \brief Cryptsetup cmd unix line
/// \date Thu Nov 23 09:45:42 2017
///

#include <stdio.h>
#include <stdlib.h>
#include "pamela.h"

///
/// \fn bool create_empty_container_file(const char *path, empty_container_file_mode mode)
/// \brief Create a empty file for futur container
/// \brief More faster, Less secure
/// \param path Container path
/// \param mode Creation mode
/// \return Status function
///
bool	create_empty_container_file(const char *path,
				    t_empty_container_file_mode mode)
{
  char	*cmd;
  bool	ret;

  asprintf(&cmd, (mode == DD_MODE ? DD_DEV_ZERO_CMD : FALLOCATE_CMD), CONTAINER_SIZE, path);
  ret = (system(cmd) == 0 ? true : false);
  free(cmd);
  return (ret);
}

///
/// \fn bool create_random_container_file(const char *path, random_container_mode mode)
/// \brief Create a random file for futur container
/// \brief More secure (mimic encrypted date), Less faster
/// \param path Container path
/// \param mode Create mode
/// \return Status function
///
bool	create_random_container_file(const char *path,
				     t_random_container_mode mode)
{
  char	*cmd;
  bool	ret;

  asprintf(&cmd, (mode == DD_URANDOM_MODE ? DD_URANDOM_CMD : DD_RANDOM_CMD), path, CONTAINER_SIZE);
  ret = (system(cmd) == 0 ? true : false);
  free(cmd);
  return (ret);
}
