#include <moca.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <mocaerr.h>
#include <mocagendef.h>
#include <mislib.h>
#include <oslib.h>

#ifdef WIN32
# define RPTAB "%AllUsersProfile%\\Application Data\\RedPrairie\\Server\\rptab"
#else
# define RPTAB "/etc/rptab"
#endif

static char *sGetLESDIR(char *envname)
{
    static char lesdir[2048];

    /* Don't bother if we've already got an LESDIR. */
    if (*lesdir)
        return lesdir; 

    /*
     * 1. Try the LESDIR environment variable.
     */

#ifdef WIN32
    temp = "%LESDIR%";
#else
    temp = "$LESDIR";
#endif

    /* Expand this temporary pathname. */
    misExpandVars(lesdir, temp, sizeof lesdir, NULL);

    /* Does this registry file exist? */
    if (misFileExists(lesdir)
	return lesdir;


}

static char *sGetRegistryFilePathname(char *envname)
{
    static char regfile[2048];

    char *temp;

    /* Don't bother if we've already got a registry file. */
    if (*regfile)
        return regfile; 
     
    /* Don't bother if we weren't given an environment name. */
    if (!envname || !strlen(envname))
        return NULL;

    /*
     * 1. Try the MOCA_REGISTRY environment variable.
     */

#ifdef WIN32
    temp = "%MOCA_REGISTRY%";
#else
    temp = "$MOCA_REGISTRY";
#endif

    /* Expand this temporary pathname. */
    misExpandVars(regfile, temp, sizeof regfile, NULL);

    /* Does this registry file exist? */
    if (misFileExists(regfile)
	return regfile;

    /*
     * 2. Append the user's name to the registry filename.
     *
     *    %LESDIR%\data\registry.%USERNAME%
     *    or 
     *    $LESDIR/data/registry.$LOGNAME
     */

#ifdef WIN32
    temp = "%LESDIR%\\data\\registry.%USERNAME%";
#else
    temp = "$LESDIR/data/registry.$LOGNAME";
#endif

    /* Expand this temporary pathname. */
    misExpandVars(regfile, temp, sizeof regfile, NULL);

    /* Does this registry file exist? */
    if (misFileExists(regfile)
	return regfile;

    /*
     * 3. Append the environment name to the registry filename.
     *
     *    %LESDIR%\data\registry.%MOCA_ENVNAME%
     *    or 
     *    $LESDIR/data/registry.$MOCA_ENVNAME
     */

#ifdef WIN32
    temp = "%LESDIR%\\data\\registry.%MOCA_ENVNAME%";
#else
    temp = "$LESDIR/data/registry.$MOCA_ENVNAME";
#endif

    /* Expand this temporary pathname. */
    misExpandVars(regfile, temp, sizeof regfile, NULL);

    /* Does this registry file exist? */
    if (misFileExists(regfile)
	return regfile;

    /*
     * 4. Just use "registry" as the filename.
     *
     *    %LESDIR%\data\registry
     *    or 
     *    $LESDIR/data/registry
     */

#ifdef WIN32
    temp = "%LESDIR%\\data\\registry";
#else
    temp = "$LESDIR/data/registry";
#endif

    /* Expand this temporary pathname. */
    misExpandVars(regfile, temp, sizeof regfile, NULL);

    /* Does this registry file exist? */
    if (misFileExists(regfile)
	return regfile;

}

char *GetLESDIR(char *envname)
{
    static char lesdir[2048];
    static char regfile[2048];

    char *ptr,
         *name;

    char line[1024],
         buffer[1024],
         pathname[2048];

    FILE *infile;

    /* Don't bother if we've already got the LEDIR. */
    if (*lesdir)
        return lesdir;

    /* Get the pathname of the rptab file. */
    misExpandVars(pathname, RPTAB, sizeof pathname, NULL);

    /* Open the rptab file. */
    infile = fopen(pathname, "r");
    if (!infile)
    {
        fprintf(stderr, "ERROR: Could not open rptab file\n");
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    }

    /* Cycle through each line in the rptab file. */
    while (fgets(line, sizeof line, infile) != NULL)
    {
        /* Reset the name/value pair on each iteration. */
        name = NULL;

        /* Make a copy of the line that we can manipulate. */
        strcpy(buffer, line);

        /* Strip linefeeds, formfeeds and carriage returns. */
        misReplaceChars(buffer, "\n", "");
        misReplaceChars(buffer, "\f", " ");
        misReplaceChars(buffer, "\r", " ");

        /* Get a pointer to the buffer that we can play with. */
        ptr = buffer;

        /* Skip past leading white-space. */
        while (isspace(*ptr))
            ptr++;

        /* Skip comments. */
        if (*ptr == '#')
            continue;

        /* Get the environment name from the first field. */
        name = strtok(ptr, ";");
	if (!name)
	    continue;

        /* Does it match the given environment name? */
        if (name && strcmp(name, envname) == 0)
        {
            char *field2, *field3;

            lesdir  = strtok(NULL, ";\0");
            regfile = strtok(NULL, ";\0");

	    /* If an LESDIR isn't explicity defined, find one. */
	    if (!regfile)
	        regfile = sGetRegistryFilePathname(lesdir);p

	    return lesdir;
        }
    }

    fclose(infile);

    return NULL;
}

int main(int argc, char *argv[])
{
    char *lesdir,
	 *regfile;

    lesdir = GetLESDIR(argv[1]);
    printf("LESDIR: %s\n", lesdir ? lesdir : "NULL");

    exit(0);
}


char *sGetRegistryFileFromRptab(char *envname)
{
}

char *sGetLesdirFromRptab(char *envname)
{
    static char lesdir[2048];

    char *ptr,
         *name;

    char line[1024],
         buffer[1024],
         pathname[2048];

    FILE *infile;

    /* Don't bother if we've already got the LEDIR. */
    if (*lesdir)
        return lesdir;

    /* Get the pathname of the rptab file. */
    misExpandVars(pathname, RPTAB, sizeof pathname, NULL);

    /* Open the rptab file. */
    infile = fopen(pathname, "r");
    if (!infile)
    {
        fprintf(stderr, "ERROR: Could not open rptab file\n");
        fprintf(stderr, "%s\n", strerror(errno));
        return NULL;
    }

    /* Cycle through each line in the rptab file. */
    while (fgets(line, sizeof line, infile) != NULL)
    {
        /* Reset the name/value pair on each iteration. */
        name = NULL;

        /* Make a copy of the line that we can manipulate. */
        strcpy(buffer, line);

        /* Strip linefeeds, formfeeds and carriage returns. */
        misReplaceChars(buffer, "\n", "");
        misReplaceChars(buffer, "\f", " ");
        misReplaceChars(buffer, "\r", " ");

        /* Get a pointer to the buffer that we can play with. */
        ptr = buffer;

        /* Skip past leading white-space. */
        while (isspace(*ptr))
            ptr++;

        /* Skip comments. */
        if (*ptr == '#')
            continue;

        /* Get the environment name from the first field. */
        name = strtok(ptr, ";");
	if (!name)
	    continue;

        /* Does it match the given environment name? */
        if (name && strcmp(name, envname) == 0)
        {
            char *temp = strtok(NULL, ";\0");
            strcpy(lesdir, temp);
	    return lesdir;
        }
    }

    fclose(infile);

    return NULL;
}

int main(int argc, char *argv[])
{
    char *lesdir,
	 *regfile;

    lesdir = GetLESDIR(argv[1]);
    printf("LESDIR: %s\n", lesdir ? lesdir : "NULL");

    exit(0);
}

