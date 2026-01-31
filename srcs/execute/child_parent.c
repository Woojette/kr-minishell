
#include "./mini_exec.h"

void	ft_execute(char **options, t_data *data)
{
	char	*cmd;
	char	*cmd_path;

	cmd = options[0];
	if (!cmd)
		exit(0);
	if (is_builtin(cmd))
		exit(ft_builtin(options, data));
	cmd_path = get_cmd_path(cmd, data->path);
	if (!cmd_path)
	{
		g_exit_status = 127;
		error_command_msg(cmd, CMD_ERROR);
		return ;
	}
	if (is_directory(cmd_path))
	{
		error_command_msg(cmd, DIREC_ERROR);
		exit(126);
	}
	if (execve(cmd_path, options, get_env(data->my_env)) == -1)
	{
		error_command(cmd);
		exit(126);
	}
}

void	child_process(t_data *data, t_pipeline *pipeline, int *p_fd, int i)
{
	g_exit_status = 0;
	signal_setting_commandmode();
	if (data->process_number > 1)
	{
		if (i == 0)
		{
			// 첫번째 커맨드 패밀리
			ft_dup2(p_fd[1], 1);
			data->prev_fd = p_fd[0];
			ft_close(p_fd[0]);
			ft_close(p_fd[1]);
		}
		else if (i != 0 && i != data->process_number - 1)
		{
			// 중간 커맨드 패밀리
			ft_dup2(data->prev_fd, 0);
			ft_close(data->prev_fd);
			ft_dup2(p_fd[1], 1);
			data->prev_fd = p_fd[0];
			ft_close(p_fd[0]);
			ft_close(p_fd[1]);
		}
		else
		{
			// 마지막 커맨드 패밀리
			ft_dup2(data->prev_fd, 0);
			ft_close(data->prev_fd);
			ft_close(p_fd[0]);
			ft_close(p_fd[1]);
		}
	}
	if (redirection_center(pipeline->cmd_block->redirect))
		g_exit_status = 1;
	if (g_exit_status == 0)
		ft_execute(pipeline->cmd_block->cmd, data);
	exit(g_exit_status);
}

void	parent_process(t_data *data, int *p_fd, int i)
{
	if (data->process_number == 1)
		return ;

	if (i == 0)
	{
		// 첫번째 커맨드 패밀리
		data->prev_fd = p_fd[READ];
		ft_close(p_fd[WRITE]);
	}
	else if (i != 0 && i != data->process_number - 1)
	{
		// 중간 커맨드 패밀리
		ft_close(data->prev_fd);
		data->prev_fd = p_fd[READ];
		ft_close(p_fd[WRITE]);
	}
	else
	{
		// 마지막 커맨드 패밀리
		ft_close(data->prev_fd);
		ft_close(p_fd[READ]);
		ft_close(p_fd[WRITE]);
	}
}
