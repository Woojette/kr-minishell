void create_child(t_cmd *cmd, t_mini *mini, int i)
{
	px->pid[i] = fork();
	if (px->pid[i] < 0)
		msg_error(px, "fork error");
	if (px->pid[i] == 0)
	{
		set_child(cmd, mini);
		run_child(*px, argv, envp);
	}
}

void	set_child(t_cmd *cmd, t_mini *mini, int id)
// 이걸 t_cmd cmd, mini 로 바꿔야
{
	1) -> parent 파이프 배열 /fd / 명령 배열 참조하기 / 복사가 아니라 참조..
	px.cmd[뭐꼬] = NULL;
	px.cmd_args[뭐꼬] = NULL; // 파싱에서 이미 받아오나?
	if (px[id].infile < 0)
	{
		close(px.tube[0]);
		close(px.tube[1]);
		pipex_free(&px);
		pipex_free(&parent);
		exit(1);
	}
	setup_fds(&px, 1);
	close_fds(cmd, mini);
	run_child(&parent, &px, av[2], envp) // exec or builtin
}

setup_fds(&px, 1)
{
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
}

close_fds(t_cmd cmd, mini)
{
	// 3) 클로즈
	모든 pipefd[*][0/1] close
	redir 용 오픈한 fd 도 dup2 했으면 클로즈
	히어독 fd 도 dup2 후 클로즈
	child 는 exec 전에 닫기
	parent 는 각 fork 이후 바로 닫아야 
}

void	run_child(t_pipex *parent, t_pipex *px, t_mini *mini)
{
	do_bi(if_bi(cmd, mini) < enum으로 나오는거로 패런트 차일드 갈라도 되나 전역 뭐시기인지 체크)
	else PATH 탐색 실행파일 경로 결정
	if (cmd[i].cmd[0] "/" 큰 따옴표인지 뭔지는 기재 방식 확인하기)
		excve(path, argv, ini->env)
	if (cmd[i].cmd[0] !!! "/")
		ft_split (PATH) 후보 경로들 만들고
		exceve
	// 위에 거 이상한 소리 한 걸 수 있고 cmd[i].argv 기준 체크.. 
	////// 클로즈 여기서 해야하나,,??
	exceve(cmd_path, cmd[i].argv, mini->env);
	pipex_free(px);
	pipex_free(parent);
	perror("execve");
	exit(127);
}

int parent_process (t_mini *mini)
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
		if (nbr_cmd > 1)
		{
			i = 0;
			ft_bzero(&px, sizeof(t_pipex));
			while (i < nbr_cmd - 1)
			{
				create_child(px, argv, envp, i)
				close (pipefd) - parent 는 각 fork 이후에 close 필요한데 이게 여기여야 하나..??
				// 이 함수는 pipe nbr_cmd - 1 개 생성
				// pipefd[k][2] 생성 동적 할당
		}
		// while (each pipe for created child process i < nbr_cmd - 1)
		// 	close (pipefd 모두) // 불필요한 pipe end 각 포크 이후 바로 닫아야.. EOF 없어서 영원히 대기하게 됨
		// 					// 얘 위치 이상한 거 같음..
		waitpid(px.pid1..., NULL, 0);
		while (each pipe for created child process)
			close_pipe(&px);
		while (each child process)
			모든 pid waitpid(pid, &status, 0)
		mini->exit_status = wait_child(마지막 애의 &px)
		open_files(&px, argv, argc);
		do_bi(is_bi(cmd, mini));
		if (pipe(px.tube) < 0)
			msg_error(&px, ERR_PIPE);
		get_cmd_paths(&px, envp);
		// $? 가 해당 가ㅄ으로 확장
		i++;
	}
}