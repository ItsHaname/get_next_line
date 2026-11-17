#include "get_next_line.h"

static  char *lire_fichier(int fd, char* reserve)
{
	int  octets_lus;
	char *buffer;

	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if(!buffer)
		return(NULL);
	octets_lus = 1;
	while(octets_lus > 0 && !ft_strchr(reserve,'\n'))
	{
		octets_lus = read(fd,buffer,BUFFER_SIZE);
		if(octets_lus < 0)
			return (free(reserve),free(buffer),NULL);
		buffer[octets_lus] = '\0';
		reserve = ft_strjoin(reserve,buffer);
		if(reserve == NULL)
			break;
	}
	free(buffer);
	return (reserve);
}

static char* extraire_ligne(char *reserve)
{
	int		i;

	if(reserve[0]=='\0'|| reserve == NULL)
		return(NULL);
	i = 0;
	while(reserve[i] && reserve[i] != '\n')
		i++;
	if(reserve[i] == '\n')
		i++;
	return(ft_substr(reserve,0,i));
}
static  char *nettoyer_reserve(char *reserve)
{
	int		i;
	char	*new;
	if(reserve == NULL || reserve[0] == '\0')
		return(NULL);
	i = 0;
	while(reserve[i] && reserve[i] != '\n')
		i++;
	if(reserve[i] == '\n')
		i++;
	new = ft_substr(reserve,i,ft_strlen(reserve) - i);
	return(new);
}
char  *get_next_line(int fd)
{
	static char* reserve;
	char *ligne;

	if(fd < 0 || BUFFER_SIZE < 0)
		return(NULL);
	reserve = lire_fichier(fd, reserve);
	if(!reserve)
		return(NULL);
	ligne = extraire_ligne(reserve);
	if(!ligne)
	{
		free(reserve);
		reserve = NULL;
		return NULL;
	}
	reserve = nettoyer_reserve(reserve);
	return (ligne);

}
