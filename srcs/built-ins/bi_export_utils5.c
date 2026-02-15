#include "minishell.h"

void	re_ex_and_env(char *str, t_mini *mini) // 어떤 줄을 수작칠지 보는 것
{
	int	i;
	int	change_pos;

	i = 0;
	change_pos = key_index(str, mini->save_ex);
	exp_update2(str, mini, change_pos); // 그 줄 기준으로 수작질하기
	env_update2(str, mini); // save_ex[i] 라는애를 빼버리고,,  
}

void	save_export(char *str, t_mini *mini)
{
	if (!has_valid_key(str))
	{	
		printf("export : not a valid identifier\n");
		mini->exit_status = 1;
		return ;
	}
	else	
	{	
		mini->exit_status = 0;
		if (has_samekey(str, *mini) == -1)
		{	
			if (equal_checker(str) == 1) // 있으니까.. 양쪽 다 없뎃
				ex_and_env(str, mini);
			else
				exp_update(str, mini); // 없으면 키만 있다는 거니까.. 
		}
		else if (has_samekey(str, *mini) == 0)
			return ;
		else if (has_samekey(str, *mini) == 1)// str 에 = 있는데 키 같은 거 확인 한 경우, mini 에 있는 애가 통째로 같은지 : 통재로 같으면 업뎃x 다르면 업뎃
		{
			if (same_checker(str, *mini) == 1)
				return ;
			re_ex_and_env(str, mini);
		}
	}
}

void	bubble_pointer(char **str1, char **str2)
{
	char *temp;

	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}

void	export_sort(t_mini *mini)
{
	int	i;
	int	j;
	char **sorted;

	i = 0;
	sorted = mini->save_ex;
	while (i< ft_env_len(mini->save_ex) - 1)
	{
		j = 0;
		while (j < ft_env_len(mini->save_ex) - 1)
		{
			if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
				bubble_pointer(&sorted[j], &sorted[j + 1]);
			j++;
		}
		i++;
	}
}