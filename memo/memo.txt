// result = add_cmd(parsing, cmd);
// if (result == -1)
// {
// 	free_tokens(&parsing);
// 	free(line);
// 	return (-1);
// }
// if (result == -2)
// {
// 	free_tokens(&parsing);
// 	free(line);
// 	continue ;
// }

// exec_run(cmd, mini);

// free_tokens(&parsing);
// free(line);

int	exec_run(t_cmd *cmd, t_mini *mini)
{
	// heredoc 선처리
	for (i++; i < nbr_cmd; i = 0)
	{
		if (cmd[i].heredoc == 1)
			delimiter
			cmd[i].fd_in = read();
			while heredoc, ctrl-c a gerer, cmd line to cancel all
	}
	if (nbr_cmd == 1)
		do_bi(is_bi(cmd[0]));
	// 파이프 배열 준비
	ft process_parent;
	set_redir(mini);
	// fork 루프
	for (i++, i < nbr_cmd - 1, i == 0)
	{
		if (is_bi(cmd[i].cmd[0]))
			nbr_cmd == 1 이면 파이프라인 없다는 거임 패런트
			빌트인이 cd export unset exit 경우 패런트
		pid = fork();
		child?
		parent?
			pid 저장, i++;
	}
	return (1);
}

시그널 기본 동작
void	signal(cmd, mini, int)
{
	if (int 기준 parent 판별) // parent 프롬프트 대기 중에
	{
		if (컨트롤씨)
			새 줄 새 프롬프트
		if (ctrl-\)
			무시
	}
	else if (int 기준 child 판별) // child 실행 중에
	{
		if (ctrl-C || ctrl-\)
			프로세스 종료
	}
	else if (int 기준 heredoc 입력 중 판별)
		if (ctrl-C)
			현재 커맨드 라인 취소
}

int	is_bi(cmd)
{
	if (cd)
		return (T_CD);
	else if (export)
		return (T_EXPORT);
	else if (unset)
		return (T_UNSET);
	else if (exit)
		return (T_EXIT);
	else if (echo)
		return (T_ECHO);
	else if (pwd)
		return (T_PWD);
	else if (env)
		return (T_ENV);
	else
		return (6); // ??
} 전역스럽게 되는지 뭔지 체크하기

void do_bi(int	i)
{
	if (i == T_CD)
	{
		execve
		exit(status)
	}
	.
	.
	.
	else if (i == T_ENV)
	{
		execve;
		exit (status);
	}
	if (i == 6)
		return ; // ??
}

void	set_redir(t_mini *mini)
{
	if (<)
		fd_in = open()
	if (> || >>)
		fd_out = open(outfile, O_CREATE | O_WRONLY | O_TRUNC) or O_APPEND
	if (<<)
	{
		create heredoc
		fd_in = read()
	}
}