#include "minishell.h"

void	ft_exit_sans_arg2(t_mini *mini)
{
	// mini->exit_status = mini->exit_status % 256;
	free_round(mini);
	printf("exit\n");
	exit (mini->exit_status);
}

void	ft_exit_normal_arg2(long long val, t_mini *mini)
{
	mini->exit_status = val % 256;
	free_round(mini);
	printf("exit\n");
	exit (mini->exit_status);
}

// void	ft_exit_pl_arg(t_mini *mini)
// {
// 	mini->exit_status = 1;
// 	// ft_free_all(mini); 종료 함수 아닌데 다 프리해서
// 	printf("exit\n");
// 	printf("minishell: exit: too many arguments\n");
// }

void	ft_exit_wrong_arg2(char *str, t_mini *mini)
{
	mini->exit_status = 2;
	free_round(mini);
	printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", str);
	exit (mini->exit_status);
}

void	ft_exit2(char **tab, t_mini *mini)
{
	long long	temp;
	int			error;

	error = 0;
	if (tab[1] == NULL)
		ft_exit_sans_arg2(mini);
	else if ((tab[1] != NULL) && (tab[2] != NULL))
	{
		ft_exit_pl_arg(mini);
		// return (exit_temp);//엑싯코드랑 상관없는 값이라서 고쳐야 할 거 같음.. 
		return ; //빌트인 함수가 실행만 시키니까 그냥 0 해둠 보이드 해도 될듯
	}
	else if ((tab[1] != NULL) && (tab[2] == NULL))
	{
		if (ft_exit_check_not_int(tab[1]) == 1)
			ft_exit_wrong_arg2(tab[1], mini);
		temp = ft_exit_atoi_long(tab[1], &error);
		if (error == 1)// long long++
			ft_exit_wrong_arg2(tab[1], mini);
		ft_exit_normal_arg2(temp, mini);
	}
}