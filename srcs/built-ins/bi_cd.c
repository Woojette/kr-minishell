#include "minishell.h"
//free((*env)[j]); 메인의 env 그대로 쓰면서 프리하는 UB라서 에러 가능
// deep copy 해서 복사본만 수정하기
// dup_env(envp) 깊은 복사 후 env_set(&mini->envm "PWD", value) 같은 형태로 PWD/OLDPWD업데이트 하라는데
// 중요한 건 내가 할당하지 않은 메모리를 프리한는거란거
// 내 메모리로 만들고 환경 변수 수정은 항상 mini->env 만 수정

char *ft_cd_val_env(char *str, t_mini *mini)
{
  int   j;
  char	*str_path;

  j = 0;
  while ((mini->env)[j] != NULL)
  {
    if (ft_strncmp((mini->env)[j], str, ft_strlen(str)) == 0)
	{
		str_path = ft_strdup((mini->env)[j] + ft_strlen(str));
		if (!str_path)
			return (NULL);
		return (str_path);
	}
    j++;
  }
  return (NULL);
}

int	ft_cd_sans_av(char **val, char **path, char *str, t_mini *mini)
{
	char	*new_oldpwd;
	char	*new_pwd;

    (*val) = ft_cd_val_env(str, mini);
	if (!(*val))
	{
		if (ft_strncmp(str, "HOME=", 5) == 0)
			printf("minishell: cd: HOME not set\n");
		else if (ft_strncmp(str, "OLDPWD=", 7) == 0)
			printf("minishell: cd: OLDPWD not set\n");
		return (-1);
	}
    (*path) = (*val);
	new_oldpwd = getcwd(NULL, 0);
	if (chdir((*path)) == -1)
	{
		perror ("chdir");
		return (-1);
	}
	new_pwd = getcwd(NULL, 0);
	ft_cd_env_update(new_oldpwd, new_pwd, mini);
	return (0);
}

int	ft_cd_tiret(char *oldpwd, char **path, t_mini *mini)
{
	char	new_oldpwd[1024];
	char	new_pwd[1024];

	oldpwd = ft_cd_val_env("OLDPWD=", mini);
	if ((oldpwd) == NULL)
    {
      printf("minishell: cd: OLDPWD not set\n");
      return (-1);
    }
	(*path) = (oldpwd);
	if (getcwd(new_oldpwd, sizeof(new_oldpwd)) == NULL)
		return (perror("minishell: cd"), -1);
	if (chdir((*path)) == -1)
	{
		printf("cd: %s", (*path));
		printf(": No such file or directory\n");
		return (-1);
	}
	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
		return (perror("minishell: cd"), -1);
	ft_cd_env_update(new_oldpwd, new_pwd, mini);
	printf("%s\n", (*path));
	return (0);
}

// void	ft_cd_env_val_vide(int j, char *str, char ***env)
// {
// 	int	i;
// 	int	len;

// 	i = ft_strlen(str);
// 	len = ft_strlen((*env)[j]);
// 	while (i < len)
// 	{
// 		(*env)[j][i] = '\0';
// 		i++;
// 	}
// }
 
int	ft_cd_env_update(char *oldpwd, char *pwd, t_mini *mini)
{
	int		j;
	char	*temp;

	j = 0;
	while ((mini->env)[j] != NULL)
	{
		if (ft_strncmp((mini->env)[j], "OLDPWD=", 7) == 0)
		{
			temp = ft_strjoin("OLDPWD=", oldpwd);
			if (!temp)
				return (-1);
			free ((mini->env)[j]);
			(mini->env)[j] = temp;
		}
		else if (ft_strncmp((mini->env)[j], "PWD=", 4) == 0)
		{
			temp = ft_strjoin("PWD=", pwd);
			if (!temp)
				return (-1);
			free ((mini->env)[j]);
			(mini->env)[j] = temp;
		}
		j++;
	}
	return (0);
}

int	ft_cd_all(char **tab, t_mini *mini)
{
	char 	oldpwd[1024];
	char	pwd[1024];
	char	*home;
	char	*path;

	if ((tab[1] != NULL) && (tab[2] != NULL))
	{
		printf("cd: too many arguments\n");
		return (mini->exit_status = 1);
	}
	if (tab[1] == NULL || (tab[1][0] == '~' && tab[1][1] == '\0'))
	{
		if (ft_cd_sans_av(&home, &path, "HOME=", mini) == -1)
			return (mini->exit_status = 1);
		return (mini->exit_status = 0);
	}
	if (tab[2] == NULL)
	{
		if (tab[1][0] == '-' && tab[1][1] == '\0')
		{
			if (ft_cd_tiret(oldpwd, &path, mini) == -1)
				return (mini->exit_status = 1);
			return (mini->exit_status = 0);
		}
		if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		{
			mini->exit_status = 1;
			return (perror("minishell: cd"), -1);
		}
		if (chdir(tab[1]) == -1)
		{
			mini->exit_status = 1;
			return (perror("chdir"), -1);
		}
		if (getcwd(pwd, sizeof(pwd)) == NULL)
		{	
			mini->exit_status = 1;
			return (perror("minishell: cd"), -1);
		}
		ft_cd_env_update(oldpwd, pwd, mini);
		return (mini->exit_status = 0);
	}
	return (mini->exit_status = 0);
}
