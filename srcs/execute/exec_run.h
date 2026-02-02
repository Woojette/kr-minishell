typedef struct s_cmd
{
	char	**cmd;
	// cmd redir
	char	*infile; // fichier de redirection entree (<)
	char	*outfile; // fichier de redirection sortie (>)
	int		out_append; // 1 si redirection en mode append (>>), 0 sinon
	int		heredoc; // 1 si redirection heredoc (<<), 0 sinon
	char	*limiter; // limiteur pour heredoc
	// resultat des fd
	int		fd_in; // resultat ouverture fichier de < or << // 실행이 채울거
	int		fd_out; // resultat ouverture fichier de > or >> // 실행이 채울거
	// erreurs de redirection
	int		in_fail; // 1 si echec ouverture fichier de < or <<, 0 sinon // 실행이 채울거
	int		out_fail; // 1 si echec ouverture fichier de > or >>, 0 sinon //실행이 채울거
}	t_cmd;


typedef struct s_exec
{
	int	**pipefd;
	pid_t *pid;
	int	save_stdin; // parent 빌트인용
	int	save_stdout; // parent 빌트인용
} t_exec;

heredoc 문자열 채워지고 실제 fd 생성 아직 없어서 cmd[i].heredoc == 1 임에도 cmd[i].fd_in 은 계속 -1인 중

mini->nbr_cmd > 1 이면 pipe 있음

파싱 이후 구조체 상태
cat < in.txt | grep hi >> out.txt

cmd[0] = 첫 커맨드 cat < in.txt

cmd[0].cmd
{"cat", NULL}
cmd[0].infile
"in.txt"
cmd[0].outfile
NULL
cmd[0].out_append
0
cmd[0].heredoc
0
cmd[0].limiter
NULL
cmd[0].fd_in
-1 (아직 실행 전이라 안 열림)
cmd[0].fd_out
-1
cmd[0].inf_fail / out_fail
0 / 0

cmd[1] = 두 번째 커맨드 grep hi >> out.txt ...

heredoc 경우
cat << EOF | wc -l
hello
EOF

cmd[0] = cat << EOF
cmd[0].cmd
{"cat", NULL}
cmd[0].heredoc
1
cmd[0].limiter
"EOF"
cmd[0].infile
NULL
cmd[0].fd_in
-1

cmd[1].cmd
{"wc", "-1", NULL}
나머지는 기본값