#include "minishell.h"


// ihoa 랑 inoutfile 로 바꾼 건 필요해서 한건데 in fail out fail 쓰나 몰라서 일단 합침
// 어차피 인덱스 기반이라 같아지지 않나함 ihoa 밑에도 해야지..

//cmd_array[i].ihoa infile heredoc outfile 인아웃 섞은 애 / inoutfile 역시 섞을 애
//ihoa 상태는 리다이렉션 타입의 인트 배열이어야 함 이넘으로 볼 수 있게 1trunc append 2 inf 3 히어독은 4 넣어야겠거니..
//근데 리다이렉션 다 따서 넣으면 히어독 경우는 이거 밑에서 어케하지 순서상 안겹치게 한다든지 해야하는건지
//<- 이걸 위해 inoutfile[n] 에 히어독 temp 경로를 넣을 것임
// 리미터를 일단 저장해둔다음에 temp path 만들어서 inoutfile 에 푸쉬한다 ihoa 에 히어독 푸쉬를 한다
// k = cmd[j].compter_heredoc 현재까지 본 히어독 개수
// limiter[k]에 문자열 저장
// temp_heredoc[k] 에 temp 경로 저장
// redir 순서 배열 히어독 위치에도 같은 케이의 텀프 경로 넣어주기 
// inoutfile[pos] 에 히어독 자리엔 더미를 넣든 한다 근데 밑의 함수에서 heredoc 만나면
// open(mini->cmd_array[i].temp_heredoc[k], O_RDONLY) k++; 

// en parcourant les redir de gauche a droite dans cmd_array[i],
// on ouvre les fichiers de redirection dans l'ordre d'apparition, et on met a jour fd_in/fd_out
// avant de remplacer fd_in/fd_out, on ferme les anciens descripteurs s'ils existent pour eviter les fuites de fd
// fd_in (<, <<), fd_out (> ou >>)
void	inout_redir(t_mini *mini, int i)
{
	int	n; // index pour parcourir les tableaux ihoa et inoutfile de cmd_array[i]
	int	k; // index pour parcourir les tableaux limiter et temp_heredoc de cmd_array[i] pour les redirections heredoc, pour savoir quel limiter et quel fichier temporaire correspond a chaque redirection heredoc
	int	type_redir; // variable pour stocker le type de redirection (trunc, append, inf, heredoc) de la redirection courante, pour savoir comment ouvrir le fichier de redirection
	int	fd_temp; // variable pour stocker le résultat de l'ouverture du fichier de redirection de la redirection courante, pour vérifier s'il y a une erreur d'ouverture

	n = 0;
	k = 0;
	type_redir = DEFAULT;
	fd_temp = -1;
	if (!mini || i < 0 || i >= mini->nbr_cmd) 
		return ;
	if (!mini->cmd_array || !mini->cmd_array[i].inoutfile)
		return ;
	if (mini->cmd_array[i].inout_fail) // si deja echec de redir in ou out, on ne fait rien
		return ;
	while (mini->cmd_array[i].inoutfile[n]) // parcourir jusqu'a la fin du tableau inoutfile de cmd_array[i] (jusqu'a la fin de toutes les redirections)
	{
		type_redir = DEFAULT; // pour eviter les valeurs aleatoires de type_redir, on le remet a DEFAULT a chaque iteration, pour que si jamais on a une redirection sans type (ce qui ne devrait pas arriver), on puisse detecter l'erreur et ne pas faire de redirection avec un type aleatoire
		fd_temp = -1;
		if (!mini->cmd_array[i].ihoa || n >= len_tab_char(mini->cmd_array[i].inoutfile))
			type_redir = DEFAULT; // 원래 이닛 있어서 따온건데 잘 모르겠음
		else
			type_redir = mini->cmd_array[i].ihoa[n];
		if (type_redir == TRUNC) // > 
		{
			fd_temp = open(mini->cmd_array[i].inoutfile[n], O_WRONLY | O_TRUNC | O_CREAT, 0644);
			
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].inoutfile[n]); // afficher l'erreur
				mini->exit_status = 1; // mettre le code de sortie a 1
				mini->cmd_array[i].inout_fail = 1; // marquer que l'ouverture a echoue
				if (mini->cmd_array[i].fd_out != -1) // si un ancien fichier de sortie existe, on le ferme avant de faire le return pour eviter les fuites de fd
				{
					close(mini->cmd_array[i].fd_out);
					mini->cmd_array[i].fd_out = -1;
				}
				return ;
			}
		}
		else if (type_redir == APPEND)
		{
			fd_temp = open(mini->cmd_array[i].inoutfile[n], O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].inoutfile[n]); // afficher l'erreur
				mini->exit_status = 1; // mettre le code de sortie a 1
				mini->cmd_array[i].inout_fail = 1; // marquer que l'ouverture a echoue
				if (mini->cmd_array[i].fd_out != -1)
				{
					close(mini->cmd_array[i].fd_out);
					mini->cmd_array[i].fd_out = -1;
				}
				return ;
			}
		}
		else if (type_redir == INF) // <
		{
			fd_temp = open(mini->cmd_array[i].inoutfile[n], O_RDONLY);
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].inoutfile[n]); // afficher l'erreur
				mini->exit_status = 1; // mettre le code de sortie a 1
				mini->cmd_array[i].inout_fail = 1; // marquer que l'ouverture a echoue
				if (mini->cmd_array[i].fd_in != -1)
				{
					close(mini->cmd_array[i].fd_in);
					mini->cmd_array[i].fd_in = -1;
				}
				return ;
			}
		}
		else if (type_redir == HEREDOC) // <<
		{
			if (!mini->cmd_array[i].temp_heredoc || !mini->cmd_array[i].temp_heredoc[k])
			{
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_in != -1)
				{
					close(mini->cmd_array[i].fd_in);
					mini->cmd_array[i].fd_in = -1;
				}
				return ;
			}
			fd_temp = open(mini->cmd_array[i].temp_heredoc[k], O_RDONLY); // 걍 짝 맞추는 용 템프 파일을 해당 인덱스에 넣을 것
			// vu que les contenus de heredoc ont deja ete stockes dans les fichiers temporaires, on ouvre directement les fichiers temporaires pour appliquer la redirection de heredoc, au lieu de faire un processus heredoc a part pour chaque redirection de heredoc pendant l'execution de la commande, ce qui serait plus complique et moins efficace
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].temp_heredoc[k]);
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_in != -1)
				{
					close(mini->cmd_array[i].fd_in);
					mini->cmd_array[i].fd_in = -1;
					mini->cmd_array[i].temp_heredoc[k] = NULL; // 누수관리하기
					init_signaux();
				}
				return ;
			}
			k++;
		}
		if ((type_redir == INF || type_redir == HEREDOC) && (mini->cmd_array[i].fd_in != -1))
		// si le cas concerne in/heredoc (<, <<), et un ancien fichier existe,
		{	
			close(mini->cmd_array[i].fd_in); // on le ferme avant de le remplacer
			mini->cmd_array[i].fd_in = -1; // on remet a -1 pour eviter les valeurs aleatoires de fd_in, pour que si jamais on a une redirection sans type (ce qui ne devrait pas arriver), on puisse detecter l'erreur et ne pas faire de redirection avec un fd_in aleatoire
		}
		if ((type_redir == APPEND || type_redir == TRUNC) && mini->cmd_array[i].fd_out != -1)
		// si le cas concerne out/append (>, >>), et un ancien fichier out existe,
		{
			close(mini->cmd_array[i].fd_out); // on le ferme avant de le remplacer
			mini->cmd_array[i].fd_out = -1;
		}
		if (type_redir == TRUNC || type_redir == APPEND)
			mini->cmd_array[i].fd_out = fd_temp;
		else if (type_redir == INF || type_redir == HEREDOC)
			mini->cmd_array[i].fd_in = fd_temp;
		if (mini->cmd_array[i].inout_fail != 1)
			mini->exit_status = 0;
		n++;
	}
}

