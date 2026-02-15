#include "minishell.h"

// export 이용시 export 라고 뜨게 하기
// ASCII 재정렬
// 밸류 안 넣은건 안들어가게 하기
// 성공시 엑싯코드 0 실패시 1
// unset 역시 export 업뎃 뭉치 기준으로 해야함

void	export_print(t_mini *mini)
{
	int	i;
	int	j;
	int	flag;

	i = -1;
	while (mini->save_ex[++i])
	{
		write(1, "export ", 7);
		j = -1;
		flag = 0;
		while (mini->save_ex[i][++j])
		{
			write(1, &(mini->save_ex[i][j]), 1);
			if (mini->save_ex[i][j] == '=')
			{
				flag = 1;
				write(1, "\"", 1);
			}
		}
		if (flag == 1)
			write(1, "\"", 1);
		write(1, "\n", 1);
	}
}

int	ft_export_all(char **tab, t_mini *mini)
{
	int	i;

	i = 0;
	if (tab[i] && !tab[1])
	{
		export_sort(mini);
		export_print(mini);
		return (0);
	}
	i = 1;
	while(tab[i] != NULL)
	{
		save_export(tab[i], mini);
		i++;
	}
	return (0);
}
