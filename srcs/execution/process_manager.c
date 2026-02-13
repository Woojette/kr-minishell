#include "minishell.h"

// i 위치 첫 중간 마지막에 따라 stdin stdout 을 pipe로 dup2 변신, 리다이렉션 적용 후 빌트인이면 빌트인 실행
// 아니면 execve 실행, 차일드는 exit으로 종료
void	child_center(t_mini *mini, t_cmd cmd, int *pipe_fd, int i)
{
	const int	type = is_built_in(mini->cmd_array[i].cmd[0]); // 현재 커맨드가 빌트인인지

	// printf("cmd.cmd[0] : %s\n", cmd.cmd[0]); // 디버깅용
	// g_exit_status = 0;
	// signal_setting_commandmode();
	// if (mini->cmd_array[i].in_fail || mini->cmd_array[i].out_fail)
 	// {
	// 	if (pipe_fd[0] >= 0)
	// 		close(pipe_fd[0]);
	// 	if (pipe_fd[1] >= 0)
	// 		close(pipe_fd[1]);
	// 	if (mini->pipe_read_end >= 0)
	// 		close(mini->pipe_read_end);
	// 	exit(mini->exit_status ? mini->exit_status : 1);
  	// }
	if (!cmd.cmd || !cmd.cmd[0])
		exit(mini->exit_status); // 체크하기
	if (mini->nbr_cmd > 1)
	{
		if (i == 0)
		{
			//

			// 첫번째 커맨드 패밀리
			close(pipe_fd[0]); // Close unused readend
			dup2(pipe_fd[1], 1); // first argument will be useless, cuz it will be camouflaged
			close(pipe_fd[1]); // Close the first argument of dup2
			// mini->pipe_read_end = pipe_fd[0]; // Save read end for the next command
		}
		else if (i != 0 && i != mini->nbr_cmd - 1)
		{
			// 중간 커맨드 패밀리
			dup2(mini->pipe_read_end, 0); // Camouflage saved read end to 0
			close(mini->pipe_read_end); // first argument of dup2 useless close
			dup2(pipe_fd[1], 1);
			close(pipe_fd[1]);
			// mini->pipe_read_end = pipe_fd[0];
			close(pipe_fd[0]);
		}
		else
		{
			// 마지막 커맨드 패밀리
			dup2(mini->pipe_read_end, 0);
			close(mini->pipe_read_end);
		}
	}
	apply_redirection_child(mini, &cmd);
	if (type == T_NOT_BUILT_IN) // 빌트인 분기
	{
		ft_execute(mini, &mini->cmd_array[i]); // 성공시 안돌아옴
		exit(mini->exit_status ? mini->exit_status : 127); //exit status 세팅시 그값, 아니면 커맨드 못찾음 127
	}
	else
	{
		execute_built_in(mini, mini->cmd_array[i].cmd, type); // 빌트인 현재 차일드 프로세스 실행후 엑싯
		exit(mini->exit_status);
	}
	// exit(g_exit_status); 위에 애들 때문에 도달 못할 거 같은데 실제로 제거하는게 필요할지 체크하기~
	if (pipe_fd[0] >= 0)
		close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	if (mini->pipe_read_end >= 0)
		close(mini->pipe_read_end);
  	exit(126);
}

// 다음 차일드 위해 pipe read end (프리 리드엔드 이런식으로 이름 다는..) 저장, write end 닫아서 파이프 EOF 처리 정상화, 마지막 커맨드에선 저장해둔 리드엔드 처리
void parent_center(t_mini *mini, int pipe_fd[2], int i)
{
	if (!mini->cmd_array)//고치기
		return ;
	if (i < mini->nbr_cmd - 1)
	{
		if (mini->pipe_read_end != -1)
		{	
			close(mini->pipe_read_end);
			mini->pipe_read_end = pipe_fd[0];
		}
		else
		{	
			mini->pipe_read_end = pipe_fd[0];
		}
		close(pipe_fd[1]); // 패런트는 write end 사용 안해서 계속 닫음..
	}
	else
	{
		if (mini->pipe_read_end != -1)
			close(mini->pipe_read_end);
	}
}

//waitpid status 엑싯코드 규칙으로 변환, 정상종료시 해당값, 시그널 종료시 셈법 맞게
static int	status_exit(int	status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

// 파이프라인 전체 실행 위함이고 각 커맨드 필요시 파이프 생성, 포크, 차일드에선 실행 덮2, 패런트는 위의 적힌 에프디 관리
//마지막 차일드 프로세스 아이디 업뎃, waitpid 와일문으로 싹 수거 / 엑싯스테이터스 마지막 커맨드 기준으로 셋
void  fork_center(t_mini *mini)
{
  int i;
  int pipe_fd[2];
  pid_t child_id;
  pid_t	*pids;
  pid_t	last_pid;
  int	status;

  last_pid = -1;

  set_signal_parent_wait();
  redirection_center(mini);
  pids = malloc(sizeof(pid_t) * mini->nbr_cmd);
  if (!pids)
	fatal_error(mini, "pids_malloc_error");
  mini->pipe_read_end = -1;
  i = 0;
  while(i < mini->nbr_cmd)
  {
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
    if (i < mini->nbr_cmd - 1) // 미니가 포인터이니 화살표(함수 인자기준)
		if (pipe(pipe_fd) == -1)
			fatal_error(mini, "pipe_error");
    child_id = fork();
	if (child_id < 0)
		fatal_error(mini, "fork_error");
    if (child_id == 0)
	{
		set_signal_exec_child();
      	child_center(mini, mini->cmd_array[i], pipe_fd, i);
	}
	pids[i] = child_id;
    parent_center(mini, pipe_fd, i);
	i++;
	}
	i = 0;
	while (i < mini->nbr_cmd)
	{
		last_pid = waitpid(pids[i], &status, 0);
		if (last_pid == pids[mini->nbr_cmd - 1])
			mini->exit_status = status_exit(status);
		i++;
	}
	fprintf(stderr, "DEBUG after waits exit_status=%d\n", mini->exit_status);
	init_signaux();
	free(pids);
}
