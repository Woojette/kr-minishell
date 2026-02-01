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
	// 파이프 배열 준비
	if (nbr_cmd == 1)
		do_bi(is_bi(cmd[0]));
	if (nbr_cmd > 1)
	{
		i = 0;
		while (i < nbr_cmd - 1)
		{
			pipe nbr_cmd - 1 개 생성
			pipefd[k][2] 생성 동적 할당
		}
	}
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

// child 프로세스 함수
int work_child(t_cmd cmd, mini)
{
	// 1) 입력 연걸
	if (pipeline 에서 i > 0)
		ft_dup2(pipefd[i - 1][0], STDIN_FILENO);
	if (cmd[i].fd_in != -1)
		ft_dup2(cmd[i].fd_in, STDIN_FILENO);
	// 2) 출력 연결
	if (i < nmbr_cmd - 1)
		dup2(pipefd[i][1], STDOUT_FILEND);
	if (cmd[i].fd_out != -1)
		dup2(cmd[i].fd_out, 1);
	if (pipeline 에서 i < n_cmd - 1)
		ft_dup2(pipefd[i][1], STDOUT_FILENO)
	// 3) 클로즈
	모든 pipefd[*][0/1] close
	redir 용 오픈한 fd 도 dup2 했으면 클로즈
	히어독 fd 도 dup2 후 클로즈
	// 4) 실행
	do_bi(if_bi(cmd, mini))
	else PATH 탐색 실행파일 경로 결정
	if (cmd[i].cmd[0] "/" 큰 따옴표인지 뭔지는 기재 방식 확인하기)
		excve(path, argv, ini->env)
	if (cmd[i].cmd[0] !!! "/")
		ft_split (PATH) 후보 경로들 만들고
		exceve
	execve(path, argv, mini->env)
	실패시
	perror error
	exit (127) 
}

// parent 프로세스 함수
int process (t_cmd cmd, mini)
{
	int	i = 0;
	int	j = 0;
	while (cmd[i])
	{
		ft_시그널 // 프롬프트 대기 중
		while (cmd[j].cmd .. 끝까지 읽다가 // {"echo", "hello", NULL} 이거 내부를 읽다가)
		{
			if (heredoc 찾으면)
				ft_dup2(fd(heredoc 만들어 둔 거), STDIN_FILENO);
			j++;
		}
		j = 0;
		ft pipe 생성
		do_bi(is_bi(cmd, mini));
		for (each cmd)
			fork;
		if (pid = child process)
			work_child(cmd, mini);
		while (each pipe for created child process)
			close (pipefd 모두)
		while (each child process)
			모든 pid waitpid(pid, &status, 0)
		mini->exit_status = 마지막 커맨드 종료코드 // $? 가 해당 값으로 확장
		i++;
	}
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