///
/// \file pam_overload.c
/// \author jonathan.bruel
/// \version 1.0
/// \brief pam function overloads
/// \date Tue Nov 21 10:50:09 2017
///

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include "pamela.h"

///
/// \fn PAM_EXTERN int free_user_password()
/// \brief free the user password save on auth time in pamela_user_password
///
static void    free_user_password(__attribute__((unused)) pam_handle_t *pamh,
				  void *data,
				  __attribute__((unused)) int error_status)
{
  if (!data)
    return ;
  free(data);
}

///
/// \fn PAM_EXTERN int pam_sm_acct_mgmt()
/// \brief task of establishing whether the user is permitted to gain access at this time
/// \return pam protocole code
///
PAM_EXTERN int	pam_sm_acct_mgmt(__attribute__((unused)) pam_handle_t *pamh,
				 __attribute__((unused)) int flags,
				 __attribute__((unused)) int argc,
				 __attribute__((unused)) const char **argv)
{
  return (PAM_SUCCESS);
}

///
/// \fn PAM_EXTERN int pam_sm_chauthtok()
/// \brief Before open session, after auth, performs the task of altering the credentials of the user
/// \return pam protocole code
///
PAM_EXTERN int pam_sm_setcred(__attribute__((unused)) pam_handle_t *pamh,
			      __attribute__((unused)) int flags,
			      __attribute__((unused)) int argc,
			      __attribute__((unused)) const char **argv)
{
  return (PAM_SUCCESS);
}

///
/// \fn PAM_EXTERN int pam_sm_authenticate()
/// \brief User authentification
/// \return pam protocole code
///
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh,
				   __attribute__((unused)) int flags,
				   __attribute__((unused)) int argc,
				   __attribute__((unused)) const char **argv)
{
  char	*user_name;
  char	*user_password;
  int	pam_code;

  printf("[My PAMela]: Authentification ...\n");
  if ((pam_code = pam_get_user(pamh, (const char **)&user_name, "Pamela: Enter your username : ")) != PAM_SUCCESS)
    return (pam_code);
  if ((pam_code = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&user_password)) != PAM_SUCCESS)
    return (pam_code);
  pam_code = pam_set_data(pamh, "pamela_user_password", strdup(user_password), &free_user_password);
  printf("[My PAMela]: Authentification done with success\n");
  return (pam_code);
}

///
/// \fn PAM_EXTERN int pam_sm_open_session()
/// \brief Open session of the user
/// \return pam protocole code
///
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh,
				   __attribute__((unused)) int flags,
				   __attribute__((unused)) int argc,
				   __attribute__((unused)) const char **argv)
{
  char	*user_name;
  char	*user_password;
  int	pam_code;
  char	*crypt_container_path;

  if ((pam_code = pam_get_user(pamh, (const char **)&user_name, "Pamela: Enter your username : ")) != PAM_SUCCESS)
    return (pam_code);
  if ((pam_code = pam_get_data(pamh, "pamela_user_password", (const void **)&user_password)) != PAM_SUCCESS)
    return (pam_code);
  if (geteuid() != 0)
    {
      fprintf(stderr, ERR_NOT_SUDO);
      return (PAM_SESSION_ERR);
    }
  if (!open_container(user_name, user_password, &crypt_container_path) &&
      !create_container(user_name, user_password, crypt_container_path))
    return (PAM_SESSION_ERR);
  return (pam_code);
}

///
/// \fn PAM_EXTERN int pam_sm_close_session()
/// \brief Close user session
/// \return pam protocole code
///
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh,
				    __attribute__((unused)) int flags,
				    __attribute__((unused)) int argc,
				    __attribute__((unused)) const char **argv)
{
  char	*user_name;
  int	pam_code;

  printf("[My PAMela]: Close session.\n");
  if ((pam_code = pam_get_user(pamh, (const char **)&user_name, "Pamela: Enter your username : ")) != PAM_SUCCESS)
    return (pam_code);
  if (!unmount_container(user_name))
    return (PAM_SESSION_ERR);
  printf("[My PAMela]: Close session with success.\n");
  return (pam_code);
}

///
/// \fn PAM_EXTERN int pam_sm_chauthtok()
/// \brief Change user password
/// \return pam protocole code
///
PAM_EXTERN int pam_sm_chauthtok(__attribute__((unused)) pam_handle_t *pamh,
				__attribute__((unused)) int flags,
				__attribute__((unused)) int argc,
				__attribute__((unused)) const char **argv)
{
  int	pam_code;
  char	*user_name;
  char	*user_password_before_change;
  char	*user_password_after_change;

  printf("[My PAMela]: Change user password\n");
  if ((pam_code = pam_get_user(pamh, (const char **)&user_name, "Pamela: Enter your username : ")) != PAM_SUCCESS)
    return (pam_code);
  if ((pam_code = pam_get_item(pamh, PAM_AUTHTOK,
			       (const void **)&user_password_after_change)) != PAM_SUCCESS)
    return (pam_code);
  if (!user_password_after_change)
    return (PAM_SUCCESS);
  if ((pam_code = pam_get_item(pamh, PAM_OLDAUTHTOK,
			       (const void **)&user_password_before_change)) != PAM_SUCCESS)
    return (pam_code);
  if (setuid(0) != 0)
    return (PAM_SYSTEM_ERR);
  if (!change_password_from_existing_container(user_name, user_password_before_change,
					       user_password_after_change))
    return (PAM_SYSTEM_ERR);
  if (setuid(0) != 0)
    return (PAM_SYSTEM_ERR);
  printf("[My PAMela]: User change is password with success\n");
  return (PAM_SUCCESS);
}
