///
/// \fileencryption_handler.c
/// \author jonathan.bruel
/// \version 1.0
/// \brief encryption handler
/// \date Thu Nov 23 11:21:28 2017
///

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcryptsetup.h>
#include "pamela.h"

///
/// \fn bool create_crypted_container_file(const char *user_name, const char *user_password)
/// \brief Create a crypted container file
/// \param user_name Unix user name
/// \param user_password Unix user password
/// \return Status of the function
///
bool	create_crypted_container_file(const char *user_name,
				      const char *user_password)
{
  struct crypt_params_luks1	luks_params;
  struct crypt_device		*cryptdevice;
  char				*crypt_container_path;

  if (user_name == NULL || user_password == NULL)
    return (false);
  asprintf(&crypt_container_path, "%s/%s", get_home_directory(user_name), CRYPT_FILE_NAME);
  if (crypt_init(&cryptdevice, crypt_container_path) < 0)
    return (false);
  luks_params.hash = "sha1";
  luks_params.data_alignment = 0;
  luks_params.data_device = NULL;
  if ((crypt_format(cryptdevice, CRYPT_LUKS1, "aes", "xts-plain64", NULL, NULL, 256 / 8, &luks_params) < 0) ||
      (crypt_keyslot_add_by_volume_key(cryptdevice, CRYPT_ANY_SLOT, NULL, 0, user_password, strlen(user_password)) < 0))
    return (false);
  free(crypt_container_path);
  return (true);
}

///
/// \fn bool open_crypted_container_file(const char *user_name, const char *user_password)
/// \brief Open crypted container file
/// \param user_name Unix user name
/// \param user_password Unix user password
/// \return Function status
///
bool			open_crypted_container_file(const char *user_name,
						    const char *user_password)
{
  struct crypt_device	*cryptdevice;
  char			*crypt_container_path;
  char			*crypt_device_name;

  if (user_name == NULL || user_password == NULL)
    return (false);
  asprintf(&crypt_container_path, "%s/%s", get_home_directory(user_name), CRYPT_FILE_NAME);
  asprintf(&crypt_device_name, "%s_device", user_name);
  if (crypt_init(&cryptdevice, crypt_container_path) < 0 ||
      crypt_load(cryptdevice, CRYPT_LUKS1, NULL) < 0 ||
      crypt_activate_by_passphrase(cryptdevice, crypt_device_name, CRYPT_ANY_SLOT, user_password, strlen(user_password), 0) < 0)
    return (false);
  crypt_free(cryptdevice);
  return (true);
}
