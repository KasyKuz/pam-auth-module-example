#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    const char *cPass = NULL;
    int retval;
    const char* pUsername;
    
    retval = pam_get_user(pamh, &pUsername, "Username: ");
    if (retval != PAM_SUCCESS) {
        return retval;
    }
    
    printf("Welcome %s\n", pUsername);
    
    retval = pam_get_authtok(pamh, PAM_AUTHTOK, &cPass, NULL);
    if (retval != PAM_SUCCESS) {
        return retval;
    }
    
    if (strcmp(cPass, "ohmysosecretpass") == 0) {
        return PAM_SUCCESS;
    }
    
    return PAM_AUTH_ERR;
}