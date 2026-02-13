#include "minishell.h"

// pour chaque commande dans cmd_array, on traite les redirections d'entrée et de sortie
// on ouvre les fichiers dans l'ordre d'apparition des redir (gauche a droite)
// et on ferme les anciens descripteurs avant de les remplacer par les nouveaux, pour eviter les fuites de fd
int	redirection_center(t_mini *mini)
{
	int	i;
	i = 0;

	if (!mini || !mini->cmd_array || mini->nbr_cmd <= 0)
		return (-1);
	while (i < mini->nbr_cmd)
	{
		t_cmd *c = &mini->cmd_array[i];	// utiliser le pointeur pour acceder a la structure cmd de chaque commande dans le tableau cmd_array
		// ex. cat < a | grep hi > out
		// cmd_array[0] correspond a "cat < a"
		// inoutfile = {"a", NULL}
		// ihoa = {INF}  (type de redirection d'entrée)
		if (!c->inout_fail) // s'il n'y a pas d'erreur de redirection, on applique la redirection pour chaque commande
			inout_redir(mini, i);
		i++;
	}
	return (0);
}

void	apply_redirection_child(t_mini *mini, t_cmd *cmd)
{
	if (cmd->inout_fail)
		exit(mini->exit_status ? mini->exit_status : 1);
	if (cmd->fd_in >= 0)
	{
		dup2(cmd->fd_in, 0);
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out >= 0)
	{
		dup2(cmd->fd_out, 1);
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}

//파이프 없는 빌트인 하나를 부모 프로세스에서 실행할 때 씀 // 보통 부모에서 실행해 쉘 상태 변경
// 리다이렉션 있으면 부모의 스탠다드 인 아웃 임시로 바꿔줌
// 현재 스탠다드인아웃 덮으로 백업(덥2아님),, 에프디인아웃 준비되면 각각 0/1로 덮2 ,, 빌트인 실행,, 백업해둔 애들로 복구
int	one_builtin_avec_redirs(t_mini *mini)
{
	t_cmd *c = &mini->cmd_array[0];
	const int	type = is_built_in(c->cmd[0]);
	const int	in_save = dup(0);
	const int	out_save = dup(1);

	if (c->inout_fail)
		return (mini->exit_status);
	if (!c->cmd || !c->cmd[0])
	{
		mini->exit_status = 1;
		return (mini->exit_status);
	}
	if (c->fd_in >= 0) // 리다이렉션 적용 부모에서 임시 교체
	{
		dup2(c->fd_in, 0);
		close(c->fd_in);
		c->fd_in = -1;
	}
	if (c->fd_out >= 0)
	{
		dup2(c->fd_out, 1);
		close(c->fd_out);
		c->fd_out = -1;
	}
	execute_built_in(mini, c->cmd, type);
	//표준 입출력 원상 복구 시키기
	dup2(in_save, 0);
	dup2(out_save, 1);
	close(in_save);
	close(out_save);
	return (mini->exit_status);
}
