#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<string.h>
#include<queue>
#include<climits>
#include<list>
#include<stack>
#include<cmath>
#include<algorithm>
using namespace std;
FILE *fp1,*fp2;
queue <int> queue_for_best_move;
char halma_board[16][16];
char board_sec_singles[16][16];
char board_f_e[16][16];
char method_name[50];
char black_or_white[50];
float time_limit;
char store_array[1000];
int halma_value[16][16];
char mysymbol;
char oppsymbol;

int depth;
bool game_over = false;
int blah, blah1, blah2, blah3, blah4, blah5;
int count_for_singles = 1;

int dir_x[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dir_y[] = {-1, 0, 1, 1, 1, 0, -1, -1};
int dir_xx[] = {-2, -2, -2, 0, 2, 2, 2, 0};
int dir_yy[] = {-2, 0, 2, 2, 2, 0, -2, -2};

int acbx[19]={0,0,0,0,0,1,1,1,1,1,2,2,2,2,3,3,3,4,4};
int acby[19]={0,1,2,3,4,0,1,2,3,4,0,1,2,3,0,1,2,0,1};
int acwx[19]={11,11,12,12,12,13,13,13,13,14,14,14,14,14,15,15,15,15,15};
int acwy[19]={14,15,13,14,15,12,13,14,15,11,12,13,14,15,11,12,13,14,15};

struct Posi{
	int halma_value;
	int x_cood;
	int y_cood;
};

struct Cell{
	Posi current_player;
	Posi parent;
	Posi root_parent;
	char type_move;
};

struct minheap{
	bool operator()(Cell const& n1, Cell const& n2)
	{
		return n1.current_player.halma_value > n2.current_player.halma_value;
	}
};

struct maxheap{
	bool operator()(Cell const& n1, Cell const& n2)
	{
		return n1.current_player.halma_value < n2.current_player.halma_value;
	}
};

priority_queue <Cell , vector<Cell> , minheap> queue_best_temp;
priority_queue <Cell , vector<Cell> , minheap> queue_best_of_best;
priority_queue <Cell , vector<Cell> , maxheap> maxheap_MM;
//queue <Cell> queue_best_MaxMove;
stack <Cell> stack_jump;

int is_empty_base_white(char board[16][16]){
	int temp;
	for(int i =0 ; i<19; i++){
		if(board[acwx[i]][acwy[i]] == 'W'){
			temp = i;
			break;
		}
		else{
			temp = -1;
		}
	}
	return temp;
}

int is_empty_base_black(char board[16][16]){
	int temp;
	for(int i =0 ; i<19; i++){
		if(board[acbx[i]][acby[i]] == 'B'){
			temp = i;
			break;
		}
		else{
			temp = -1;
		}
	}
	return temp;
}

int is_in_wc_for_black(char board[16][16], int x, int y){
	int temp;
	for(int i =0 ; i<19; i++){
		if(x==acwx[i] && y==acwy[i]){
			temp = 0;
			break;
		}
		else
			temp = -2;
	}
	return temp;
}

int is_in_bc_for_white(char board[16][16], int x, int y){
	int temp;
	for(int i =0 ; i<19; i++){
		if(x == acbx[i] && y == acby[i]){
			temp = 0;
			break;
		}
		else
			temp = -2;
	}
	return temp;
}

int is_in_bc_for_black(char board[16][16], int x, int y){
	int temp;
	for(int i =0 ; i<19; i++){
		if(x == acbx[i] && y == acby[i]){
			temp = 0;
			break;
		}
		else
			temp = -2;
	}
	return temp;
}

int is_in_wc_for_white(char board[16][16], int x, int y){
	int temp;
	for(int i =0 ; i<19; i++){
		if(x == acwx[i] && y == acwy[i]){
			temp = 0;
			break;
		}
		else{
			temp = -2;
		}
	}
	return temp;
}

int is_game_over_black(char board[16][16]){
	int temp;
	for(int i=0; i<19; i++){
		if(board[acbx[i]][acby[i]] == 'B')
			temp = 1;
		else if(board[acbx[i]][acby[i]] == 'W'){
			temp = -1;
			break;
		}
		else if(board[acbx[i]][acby[i]] == '.'){
			temp = -1;
			break;
		}
	}
	return temp;
}

int is_game_over_white(char board[16][16]){
	int temp;
	for(int i=0; i<19; i++){
		if(board[acbx[i]][acby[i]] == 'W')
			temp = 1;
		else if(board[acbx[i]][acby[i]] == 'B'){
			temp = -1;
			break;
		}
		else if(board[acbx[i]][acby[i]] == '.'){
			temp = -1;
			break;
		}
	}
	return temp;
}

double calculate_distance(char board[16][16], int x, int y, char piece){
	double x1, y1, dist, dist1=0;
	if(piece == 'W'){
		for(int tempo = 0; tempo < 19; tempo++){
			if(board[acbx[tempo]][acby[tempo]] == '.' || board[acwx[tempo]][acwy[tempo]] == '>'){
				x1 = x - acbx[tempo];
				y1 = y - acby[tempo];
				dist = pow (x1,2) + pow (y1,2);
				dist = sqrt(dist);
				dist1 = dist1 + dist;
			}	
		}
	}
	else if(piece == 'B'){
		for(int tempo = 0; tempo < 19; tempo++){
			if(board[acwx[tempo]][acwy[tempo]] == '.' || board[acwx[tempo]][acwy[tempo]] == '>'){
				x1 = x - acwx[tempo];
				y1 = y - acwy[tempo];
				dist = pow (x1,2) + pow (y1,2);
				dist = sqrt(dist);
				dist1 = dist1 + dist;
			}	
		}
	}
	return dist1;
}

int calculate_heuristic(char board[16][16], char piece){
	double heu = 0;
	if(piece == 'W'){
		for(int tempo =0; tempo < 16; tempo++){
			for(int tempo1 =0; tempo1 < 16; tempo1++){
				if(board[tempo][tempo1] == 'W'){
					heu = heu + calculate_distance(board,tempo,tempo1,'W');
				}
			}
		}
	}
	else if(piece == 'B'){
		for(int tempo =0; tempo < 16; tempo++){
			for(int tempo1 =0; tempo1 < 16; tempo1++){
				if(board[tempo][tempo1] == 'B'){
					heu = heu + calculate_distance(board,tempo,tempo1,'B');
				}
			}
		}
	}
	return heu;
}

char print_new_Halma(char board[16][16]){
	cout<<endl;
	for(int tempo=0; tempo<16; tempo++){
		for(int tempo1=0; tempo1<16; tempo1++){
			printf("%c ", board[tempo][tempo1]);
		}
		printf("\n");
	}	
	return 0;
}

char printing_for_jumps(int curr_player_x, int curr_player_y, int root_player_x, int root_player_y){
	Cell temporary_for_stack = stack_jump.top();
	stack_jump.pop();
	int stack_curr_x_temp = temporary_for_stack.current_player.x_cood;
	int stack_curr_y_temp = temporary_for_stack.current_player.y_cood;
	int stack_pare_x_temp = temporary_for_stack.parent.x_cood;
	int stack_pare_y_temp = temporary_for_stack.parent.y_cood;
	int stack_root_x_temp = temporary_for_stack.root_parent.x_cood;
	int stack_root_y_temp = temporary_for_stack.root_parent.y_cood;
	char stack_move_type = temporary_for_stack.type_move;
	if((stack_pare_x_temp == root_player_x) && (stack_pare_y_temp == root_player_y)){
		fprintf(fp2, "%c %d,%d %d,%d", stack_move_type, stack_pare_y_temp, stack_pare_x_temp, stack_curr_y_temp, stack_curr_x_temp);
		return 0;
	}
	else if(stack_curr_x_temp == curr_player_x && stack_curr_y_temp == curr_player_y && stack_root_x_temp == root_player_x && stack_root_y_temp == root_player_y){
		cout<<stack_pare_y_temp<<','<<stack_pare_x_temp;
		printing_for_jumps(stack_pare_x_temp, stack_pare_y_temp, root_player_x, root_player_y);
		fprintf(fp2, "\n%c %d,%d %d,%d", stack_move_type, stack_pare_y_temp, stack_pare_x_temp, stack_curr_y_temp, stack_curr_x_temp);
		return 0;
	}
	else{
		printing_for_jumps(curr_player_x, curr_player_y, root_player_x, root_player_y);
		return 0;
	}
}

int evaluate_board_for_game(char board[16][16], char piece){
	int X,Y;
	int variable_eval;
	X=Y=variable_eval=0;
	if(piece == 'W'){
		X = calculate_heuristic(board, 'W');
		Y = calculate_heuristic(board, 'B');
		variable_eval = -X + Y;
	}
	else if(piece == 'B'){
		X = calculate_heuristic(board, 'B');
		Y = calculate_heuristic(board, 'W');
		variable_eval = -X + Y;
	}
	return variable_eval;
}

char update_board(char board[16][16], int x, int y, int xx, int yy, char update){
	//printf("ENTERED UPDATE BOARD for %d %d | %d %d\n",x,y,xx,yy);
	if(update == 'B'){
		board[xx][yy] = 'B';
		board[x][y] = '.';
	}
	else {
		board[xx][yy] = 'W';
		board[x][y] = '.';
	}
	return 0;
}

int find_jumps_recurr(int x, int y, int parent_x, int parent_y, int root_parent_x, int root_parent_y, queue <Cell> &queue_best_temp){
	
	cout<<"Entered RecurrFind Jumps for - ("<<x<<","<<y<<")"<<endl;
	int xx,yy;
	int xxx,yyy;
	xx=yy=xxx=yyy=0;

	for(int i=0; i<8; i++){
		for(int tempo=0; tempo<16; tempo++){
			for(int tempo1=0; tempo1<16; tempo1++){
				board_sec_singles[tempo][tempo1] = halma_board[tempo][tempo1];
			}
		}

		xx = x + dir_x[i];
		yy = y + dir_y[i];
		xxx = x + dir_xx[i];
		yyy = y + dir_yy[i];

		if((xx < 0) || (yy < 0) || (xx > 15) || (yy > 15) || (xxx < 0) || (yyy < 0) || (xxx > 15) || (yyy > 15)){
			continue;
		}
		if((xxx == parent_x) && (yyy == parent_y)){
			continue;
		}

		if((halma_board[xx][yy] == 'B' || halma_board[xx][yy] == 'W' )&& halma_board[xxx][yyy] == '.'){

			if(halma_board[x][y] == 'B'){
							cout<<"was here"<<endl;

				int op1, op2 ;	
				int yoda1,yoda2,yoda3,yoda4;
				op1 = is_game_over_black(board_sec_singles);

				yoda1 = is_in_wc_for_black(board_sec_singles, x, y);
				yoda3 = is_in_bc_for_black(board_sec_singles, x, y);

				board_sec_singles[xxx][yyy] = 'B';
				board_sec_singles[x][y] = '.';
				halma_value[xxx][yyy] = calculate_heuristic(board_sec_singles, 'B');

				yoda2 = is_in_wc_for_black(board_sec_singles, xxx, yyy);
				yoda4 = is_in_bc_for_black(board_sec_singles, xxx, yy);

				op2 = is_game_over_black(board_sec_singles);


				if (op2 == 1 && op1 == -1){
					halma_value[xxx][yyy] = -100000 + calculate_heuristic(board_sec_singles, 'B');	
				}
				else{
					if(yoda1 == 0 && yoda2 == -2){
						continue;
					}
					else if((yoda1 == -2) && (yoda2 == 0)){
						halma_value[xxx][yyy] = -25000 + calculate_heuristic(board_sec_singles, 'B');
					}

					if(yoda3 == yoda4 == 0){
						if((x > xxx) || (y > yyy)){
							continue;
						}  
						else {
							halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'B');
						}
					}
					else if(yoda3 == 0 && yoda4 == -2){
						halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'B');
					}
				}
				cout<<"was here\n";

				struct Cell new1;
				new1.current_player.x_cood = xxx;
				new1.current_player.y_cood = yyy;
				new1.current_player.halma_value = halma_value[xxx][yyy];
				new1.parent.x_cood = x;
				new1.parent.y_cood = y;
				new1.root_parent.x_cood = x;
				new1.root_parent.y_cood = y;

				new1.type_move = 'J';
				stack_jump.push(new1);	
				queue_best_temp.push(new1);
				halma_board[x][y] = '>';
				cout<<"gogogog"<<endl;	
				find_jumps_recurr(new1.current_player.x_cood, new1.current_player.y_cood, new1.parent.x_cood, new1.parent.y_cood, new1.root_parent.x_cood, new1.root_parent.y_cood, queue_best_temp);
			}		
			else if (halma_board[x][y] == 'W'){
				//printf("x=%d y=%d \nxxx=%d yyy=%d \n",x,y,xxx,yyy);

				int op1, op2 ;	
				int yoda1,yoda2,yoda3,yoda4;
				op1 = is_game_over_white(board_sec_singles);

				yoda1 = is_in_bc_for_white(board_sec_singles, x, y);
				yoda3 = is_in_wc_for_white(board_sec_singles, x, y);

				board_sec_singles[xxx][yyy] = 'W';
				board_sec_singles[x][y] = '.';
				halma_value[xx][yy] = calculate_heuristic(board_sec_singles, 'W');

				yoda2 = is_in_bc_for_white(board_sec_singles, xxx, yyy);
				yoda4 = is_in_wc_for_white(board_sec_singles, xxx, yyy);

				op2 = is_game_over_white(board_sec_singles);
				if (op2 == 1 && op1 == -1)
					halma_value[xxx][yyy] = -100000 + calculate_heuristic(board_sec_singles, 'W');	
				else{
					if(yoda1 == 0 && yoda2 == -2){
						continue;
					}
					else if((yoda1 == -2) && (yoda2 == 0)){
						halma_value[xxx][yyy] = -25000 + calculate_heuristic(board_sec_singles, 'W');
					}
					
					if(yoda3 == yoda4 == 0)
					{
						if((x < xxx) || (y < yyy)){							
							continue;
						}  
						else {
							halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'W');
						}
					}
					else if(yoda3 == 0 && yoda4 == -2){
						halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'W');
					}
				}
				//cout<<"Pushed onto stack"<<endl;
				//cout<<"Halma Value : "<<halma_value[xxx][yyy]<<endl;;

				struct Cell new1;
				new1.current_player.x_cood = xxx;
				new1.current_player.y_cood = yyy;
				new1.current_player.halma_value = halma_value[xxx][yyy];
				new1.parent.x_cood = x;
				new1.parent.y_cood = y;
				new1.root_parent.x_cood = x;
				new1.root_parent.y_cood = y;

				new1.type_move = 'J';
				//queue_best.push(new1);	
				stack_jump.push(new1);	
				//queue_best_M.push(new1);
				queue_best_temp.push(new1);
				halma_board[x][y] = '>';
				cout<<"gogogog"<<endl;	
				find_jumps_recurr(new1.current_player.x_cood, new1.current_player.y_cood, new1.parent.x_cood, new1.parent.y_cood, new1.root_parent.x_cood, new1.root_parent.y_cood, queue_best_temp);
			}
		}	
	}		

	cout<<"exited recurr"<<endl;					
	return 0;
}

int find_jumps(int x, int y, queue <Cell> &queue_best_temp){

	cout<<"Entered Find Jumps for - ("<<x<<","<<y<<")"<<endl;
	int xx,yy;
	int xxx,yyy;
	xx=yy=xxx=yyy=0;
	
		for(int i=0; i<8; i++){
			for(int tempo=0; tempo<16; tempo++){
				for(int tempo1=0; tempo1<16; tempo1++)
					board_sec_singles[tempo][tempo1] = halma_board[tempo][tempo1];
			}

			xx = x + dir_x[i];																				
			yy = y + dir_y[i];
			xxx = x + dir_xx[i];
			yyy = y + dir_yy[i];

			if((xx < 0) || (yy < 0) || (xx > 15) || (yy > 15) || (xxx < 0) || (yyy < 0) || (xxx > 15) || (yyy > 15)){
				//cout<<"outofbounds\n";
				continue;
			}

			if((halma_board[xx][yy] == 'B' || halma_board[xx][yy] == 'W' )&& halma_board[xxx][yyy] == '.'){
				if(halma_board[x][y] == 'B'){
					//printf("x=%d y=%d \nxxx=%d yyy=%d \n",x,y,xxx,yyy);

					int op1, op2 ;	
					int yoda1,yoda2,yoda3,yoda4;
					op1 = is_game_over_black(board_sec_singles);

					yoda1 = is_in_wc_for_black(board_sec_singles, x, y);
					yoda3 = is_in_bc_for_black(board_sec_singles, x, y);

					board_sec_singles[xxx][yyy] = 'B';
					board_sec_singles[x][y] = '.';
					halma_value[xxx][yyy] = calculate_heuristic(board_sec_singles, 'B');

					yoda2 = is_in_wc_for_black(board_sec_singles, xxx, yyy);
					yoda4 = is_in_bc_for_black(board_sec_singles, xxx, yy);

					op2 = is_game_over_black(board_sec_singles);
					//cout<<op1<<" "<<op2<<endl;

					if (op2 == 1 && op1 == -1){
						//cout<<"Gameover"<<endl;
						halma_value[xxx][yyy] = -100000 + calculate_heuristic(board_sec_singles, 'B');	
					}

					else{
						//cout<<"else mei?\n";
						if(yoda1 == 0 && yoda2 == -2){
							//cout<<"From inside to outside\nSkipped"<<endl;
							continue;
							//halma_value[xxx][yyy] = -25000 + calculate_heuristic(board_sec_singles, 'B');
						}
						else if((yoda1 == -2) && (yoda2 == 0)){
							//cout<<"From outside to inside"<<endl;
							halma_value[xxx][yyy] = -25000 + calculate_heuristic(board_sec_singles, 'B');
						}

						if(yoda3 == 0 && yoda4 == 0){
							//cout<<"From inside to inside(self camp)"<<endl;
							if((x > xxx) || (y > yyy)){
								//cout<<"Not Further from parent"<<endl;
								continue;
							}  
							else 
								halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'B');
						}
						else if(yoda3 == 0 && yoda4 == -2)
							halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'B');
					}

				//	cout<<"Pushed onto stack11"<<endl;

					struct Cell new1;
					new1.current_player.x_cood = xxx;
					new1.current_player.y_cood = yyy;
					new1.current_player.halma_value = halma_value[xxx][yyy];
					new1.parent.x_cood = x;
					new1.parent.y_cood = y;
					new1.root_parent.x_cood = x;
					new1.root_parent.y_cood = y;
					
					new1.type_move = 'J';
					stack_jump.push(new1);	
					halma_board[x][y] = '>';
					queue_best_temp.push(new1);
					cout<<"going:"<<endl;
					find_jumps_recurr(new1.current_player.x_cood, new1.current_player.y_cood, new1.parent.x_cood, new1.parent.y_cood, new1.root_parent.x_cood, new1.root_parent.y_cood, queue_best_temp);
				}			

				else if (halma_board[x][y] == 'W'){
					//printf("x=%d y=%d \nxxx=%d yyy=%d \n",x,y,xxx,yyy);

					int op1, op2 ;	
					int yoda1,yoda2,yoda3,yoda4;
					op1 = is_game_over_white(board_sec_singles);

					yoda1 = is_in_bc_for_white(board_sec_singles, x, y);
					yoda3 = is_in_wc_for_white(board_sec_singles, x, y);

					board_sec_singles[xxx][yyy] = 'W';
					board_sec_singles[x][y] = '.';
					halma_value[xx][yy] = calculate_heuristic(board_sec_singles, 'W');

					yoda2 = is_in_bc_for_white(board_sec_singles, xxx, yyy);
					yoda4 = is_in_wc_for_white(board_sec_singles, xxx, yyy);

					op2 = is_game_over_white(board_sec_singles);
					//cout<<op1<<" "<<op2<<endl;

					if (op2 == 1 && op1 == -1){
						//cout<<"Gameover"<<endl;
						halma_value[xxx][yyy] = -100000 + calculate_heuristic(board_sec_singles, 'W');	
					}
					else{
						if(yoda1 == 0 && yoda2 == -2){
							//cout<<"From inside to outside\nSkipped"<<endl;
							continue;
							//halma_value[xxx][yyy] = -25000 + calculate_heuristic(board_sec_singles, 'W');
						}
						else if((yoda1 == -2) && (yoda2 == 0)){
							//cout<<"From outside to inside"<<endl;
							halma_value[xx][yy] = -25000 + calculate_heuristic(board_sec_singles, 'W');
						}

						if(yoda3 == yoda4 == 0){
							//cout<<"From inside to inside(self camp)"<<endl;
							if((x > xxx) || (y > yyy)){
								//cout<<"Not Further from parent"<<endl;
							}
							else{
								halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'W');
							}
						}
						else if(yoda3 == 0 && yoda4 == -2){
							halma_value[xxx][yyy] = -15000 + calculate_heuristic(board_sec_singles, 'W');
						}
					}

				//	cout<<"Pushed onto stack"<<endl;
				//	cout<<"Halma Value : "<<halma_value[xxx][yyy]<<endl;;

					struct Cell new1;
					new1.current_player.x_cood = xxx;
					new1.current_player.y_cood = yyy;
					new1.current_player.halma_value = halma_value[xxx][yyy];
					new1.parent.x_cood = x;
					new1.parent.y_cood = y;
					new1.root_parent.x_cood = x;
					new1.root_parent.y_cood = y;

					new1.type_move = 'J';
					//queue_best.push(new1);	
					stack_jump.push(new1);
					//queue_best_M.push(new1);
					halma_board[x][y] = '>';
					queue_best_temp.push(new1);
					cout<<"going:"<<endl;
					find_jumps_recurr(new1.current_player.x_cood, new1.current_player.y_cood, new1.parent.x_cood, new1.parent.y_cood, new1.root_parent.x_cood, new1.root_parent.y_cood, queue_best_temp);
				}	
			}
		}					
		
	//cout<<"Exited Jumps\n";
	return 0;
}

int find_moves(int x, int y, queue <Cell> &queue_best_temp){

	cout<<"\nEntered find moves for - ("<<x<<","<<y<<")"<<endl;
	int xx,yy;
	xx=yy=0;

		for(int i=0; i<8; i++){
			for(int tempo=0; tempo<16; tempo++)			
				for(int tempo1=0; tempo1<16; tempo1++)	
					board_sec_singles[tempo][tempo1] = halma_board[tempo][tempo1];

			xx = x + dir_x[i];
			yy = y + dir_y[i];

			if((xx < 0) || (yy < 0) || (xx > 15) || (yy > 15)){
				//cout<<"outofbounds\n";
				continue;
			}	

			if(halma_board[xx][yy] == '.'){	
				if(halma_board[x][y] == 'B'){	
					//printf("x=%d y=%d \nxx=%d yy=%d \n",x,y,xx,yy);

					int op1, op2 ;	
					int yoda1,yoda2,yoda3,yoda4;
					op1 = is_game_over_black(board_sec_singles);

					yoda1 = is_in_wc_for_black(board_sec_singles, x, y);
					yoda3 = is_in_bc_for_black(board_sec_singles, x, y);

					board_sec_singles[xx][yy] = 'B';
					board_sec_singles[x][y] = '.';
					halma_value[xx][yy] = calculate_heuristic(board_sec_singles, 'B');

					yoda2 = is_in_wc_for_black(board_sec_singles, xx, yy);
					yoda4 = is_in_bc_for_black(board_sec_singles, xx, yy);

					op2 = is_game_over_black(board_sec_singles);

					if (op2 == 1 && op1 == -1){
						halma_value[xx][yy] = -100000 + calculate_heuristic(board_sec_singles, 'B');	
					}
					else{
						if(yoda1 == 0 && yoda2 == -2)
							continue;
						else if((yoda1 == -2) && (yoda2 == 0))
							halma_value[xx][yy] = -25000 + calculate_heuristic(board_sec_singles, 'B');
						
						if(yoda3 == 0 && yoda4 == 0){
							if((x > xx) || (y > yy))
								continue; 
							else
								halma_value[xx][yy] = -15000 + calculate_heuristic(board_sec_singles, 'B');
						}
						else if(yoda3 == 0 && yoda4 == -2)			
							halma_value[xx][yy] = -15000 + calculate_heuristic(board_sec_singles, 'B');
					}
					//cout<<"Pushed onto stack"<<endl;
					//cout<<"Halma : "<<halma_value[xx][yy]<<endl;

					struct Cell new1;
					new1.current_player.x_cood = xx;
					new1.current_player.y_cood = yy;
					new1.current_player.halma_value = halma_value[xx][yy];
					new1.parent.x_cood = x;
					new1.parent.y_cood = y;
					new1.root_parent.x_cood = x;
					new1.root_parent.y_cood = y;

					new1.type_move = 'E';
					//queue_best.push(new1);	
					stack_jump.push(new1);	
					//queue_best_M.push(new1);
					queue_best_temp.push(new1);
				}

				else if(halma_board[x][y] == 'W'){				

					int op1, op2 ;	
					int yoda1,yoda2,yoda3,yoda4;
					op1 = is_game_over_white(board_sec_singles);

					yoda1 = is_in_bc_for_white(board_sec_singles, x, y);
					yoda3 = is_in_wc_for_white(board_sec_singles, x, y);

					board_sec_singles[xx][yy] = 'W';
					board_sec_singles[x][y] = '.';
					halma_value[xx][yy] = calculate_heuristic(board_sec_singles, 'W');

					yoda2 = is_in_bc_for_white(board_sec_singles, xx, yy);
					yoda4 = is_in_wc_for_white(board_sec_singles, xx, yy);

					op2 = is_game_over_white(board_sec_singles);

					if (op2 == 1 && op1 == -1){
						halma_value[xx][yy] = -100000 + calculate_heuristic(board_sec_singles, 'W');	
					}
					else{
						if(yoda1 == 0 && yoda2 == -2)
							continue;
						else if((yoda1 == -2) && (yoda2 == 0))
							halma_value[xx][yy] = -25000 + calculate_heuristic(board_sec_singles, 'W');						

						if(yoda3 == yoda4 == 0){
							if((x < xx) || (y < yy))
								continue; 
							else 
								halma_value[xx][yy] = -15000 + calculate_heuristic(board_sec_singles, 'W');
						}

						else if(yoda3 == 0 && yoda4 == -2)
							halma_value[xx][yy] = -15000 + calculate_heuristic(board_sec_singles, 'W');
					}
					//cout<<"Pushed onto stack\n";
				//	cout<<"Halma : "<<halma_value[xx][yy]<<endl;

					struct Cell new1;
					new1.current_player.x_cood = xx;
					new1.current_player.y_cood = yy;
					new1.current_player.halma_value = halma_value[xx][yy];
					new1.parent.x_cood = x;
					new1.parent.y_cood = y;
					new1.root_parent.x_cood = x;
					new1.root_parent.y_cood = y;

					new1.type_move = 'E';
					//queue_best.push(new1);	
					stack_jump.push(new1);	
					////queue_best_M.push(new1);
					queue_best_temp.push(new1);				
				}
			}	
		}
	//scout<<"Exited find moves\n";
	return 0;
}

int gen_moves(int x, int y, queue <Cell> &queue_best_temp){
	//printf("ENTERED GENERATE MOVES\n");
	find_moves(x, y, queue_best_temp);	
	find_jumps(x, y, queue_best_temp);
	//printf("DONE WITH GENERATE MOVES\n");
	//Cell just = queue_best_temp.front();
	//cout<<"best move : "<< just.root_parent.x_cood << " " << just.root_parent.y_cood << " -> " << just.current_player.x_cood << " " <<just.current_player.y_cood<<endl;
	return 0;
}

int Minimax_func(char board[16][16], Cell node, int depth, int alpha, int beta, char piece){
	queue <Cell> queue_best_MaxMove;
	Cell Tempo1;
	if(piece == 'B'){
		int interim1 = is_empty_base_black(board);
		if(interim1 != -1){
			for(int i =0 ; i<19; i++){
				if(board[acbx[i]][acby[i]] == 'B'){
				//	cout<<"depth : "<< depth<<endl;
					gen_moves(acbx[i], acby[i], queue_best_MaxMove);

				}
			}
			while(!queue_best_MaxMove.empty()){
				Tempo1 = queue_best_MaxMove.front();
				queue_best_MaxMove.pop();
				
				if(depth == 2){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					alpha = max(alpha, evaluate_board_for_game(board_f_e, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);			
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
				else if(depth == 1){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, oppsymbol);
					beta = min(beta, Minimax_func(board_f_e, Tempo1, 2, alpha, beta, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, oppsymbol);;	
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return beta;
					};
				}
				else if(depth == 0){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					Tempo1.current_player.halma_value = Minimax_func(board_f_e, Tempo1, 1, alpha, beta, oppsymbol);
					alpha = max(alpha, Tempo1.current_player.halma_value);
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);
					maxheap_MM.push(Tempo1);
					cout<< "alpha = "<<alpha<<endl;
					cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
			}
		}

		if(interim1 == -1){
			for(int i=9; i<10; i++){
				for(int j=0; j<16; j++){
					if (board[i][j] == 'B'){
						struct Cell player_3;
						player_3.current_player.x_cood = i;
						player_3.current_player.y_cood = j;
						player_3.current_player.halma_value = halma_value[i][j];
						//cout<<"depth : "<< depth<<endl;
						gen_moves(player_3.current_player.x_cood, player_3.current_player.y_cood, queue_best_MaxMove);
					}
				}
			}
			while(!queue_best_MaxMove.empty()){
				Tempo1 = queue_best_MaxMove.front();
				queue_best_MaxMove.pop();
				cout<<"huhuhu  ;"<<Tempo1.current_player.x_cood<<" "<<Tempo1.current_player.y_cood<<endl;
				
				if(depth == 2){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					alpha = max(alpha, evaluate_board_for_game(board_f_e, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);			
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
				else if(depth == 1){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, oppsymbol);
					beta = min(beta, Minimax_func(board_f_e, Tempo1, 2, alpha, beta, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, oppsymbol);	
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return beta;
					}
				}
				else if(depth == 0){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					Tempo1.current_player.halma_value = Minimax_func(board_f_e, Tempo1, 1, alpha, beta, oppsymbol);
					alpha = max(alpha, Tempo1.current_player.halma_value);
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);
					maxheap_MM.push(Tempo1);
					cout<< "alpha = "<<alpha<<endl;
					cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
			}
		}		
	}

	else if(piece == 'W'){
		int interim1 = is_empty_base_white(board);
		if(interim1 != -1) {
			for(int i =0 ; i<19; i++) {
				if(board[acwx[i]][acwy[i]] == 'W'){
					//cout<<"depth : "<< depth<<endl;
					gen_moves(acwx[i], acwy[i], queue_best_MaxMove);
				}
			}
			while(!queue_best_MaxMove.empty()){
				Tempo1 = queue_best_MaxMove.front();
				queue_best_MaxMove.pop();
				
				if(depth == 2){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					alpha = max(alpha, evaluate_board_for_game(board_f_e, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);			
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
				else if(depth == 1){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, oppsymbol);
					beta = min(beta, Minimax_func(board_f_e, Tempo1, 2, alpha, beta, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, oppsymbol);;	
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return beta;
					};
				}
				else if(depth == 0){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					Tempo1.current_player.halma_value = Minimax_func(board_f_e, Tempo1, 1, alpha, beta, oppsymbol);
					alpha = max(alpha, Tempo1.current_player.halma_value);
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);
					maxheap_MM.push(Tempo1);
					cout<< "alpha = "<<alpha<<endl;
					cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
			}
		}
		
		else if(interim1 == -1){
			for(int i=0; i<16; i++){
				for(int j=0; j<16; j++){
					if (board[i][j] == 'W'){
						struct Cell player_4;
						player_4.current_player.x_cood = i;
						player_4.current_player.y_cood = j;
						player_4.current_player.halma_value = halma_value[i][j];
						//cout<<"depth : "<< depth<<endl;
						gen_moves(player_4.current_player.x_cood, player_4.current_player.y_cood, queue_best_MaxMove);
					}
				}
			}
			while(!queue_best_MaxMove.empty()){
				Tempo1 = queue_best_MaxMove.front();
				queue_best_MaxMove.pop();
				
				if(depth == 2){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					alpha = max(alpha, evaluate_board_for_game(board_f_e, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);			
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
				else if(depth == 1){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, oppsymbol);
					beta = min(beta, Minimax_func(board_f_e, Tempo1, 2, alpha, beta, mysymbol));
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, oppsymbol);;	
					//cout<< "alpha = "<<alpha<<endl;
					//cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return beta;
					};
				}
				else if(depth == 0){
					update_board(board_f_e, Tempo1.parent.x_cood, Tempo1.parent.y_cood, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, mysymbol);
					Tempo1.current_player.halma_value = Minimax_func(board_f_e, Tempo1, 1, alpha, beta, oppsymbol);
					alpha = max(alpha, Tempo1.current_player.halma_value);
					update_board(board_f_e, Tempo1.current_player.x_cood, Tempo1.current_player.y_cood, Tempo1.parent.x_cood, Tempo1.parent.y_cood, mysymbol);
					maxheap_MM.push(Tempo1);
					cout<< "alpha = "<<alpha<<endl;
					cout<< "beta = "<<beta<<endl;
					if(alpha >= beta){
						return alpha;
					}
				}
			}
		}
	}

	if(depth == 1){
			return beta;
	}
	else if(depth == 0 ) {
		cout<< "alphaf = "<<alpha<<endl;
		return alpha;
	}
	if(depth == 2){
		return alpha;
	}		
}

int main() {

	fp1 = fopen("input.txt","r");

	fscanf(fp1, "%s \n", method_name);
	fscanf(fp1, "%s \n", black_or_white);
	fscanf(fp1, "%f \n", &time_limit);

	int inte = 0;

	while(fscanf(fp1, "%c ", &store_array[inte])!=EOF){
		inte++;
	}
	
	int i=0;
	int tempo,tempo1;
	
	for(tempo=0; tempo<16; tempo++){
		for(tempo1=0; tempo1<16; tempo1++){
			halma_board[tempo][tempo1] = store_array[i];
			i++;
		}
	}

	for(tempo=0; tempo<16; tempo++){
		for(tempo1=0; tempo1<16; tempo1++){
			board_f_e[tempo][tempo1] = halma_board[tempo][tempo1];
		}
	}

	if(strcmp (black_or_white, "WHITE") == 0){
		mysymbol = 'W';
		oppsymbol = 'B';
	}

	else if(strcmp (black_or_white, "BLACK") == 0){
		mysymbol = 'B';
		oppsymbol = 'W';
	}

	fclose(fp1);

	fp2 = fopen("output.txt","w");

	Cell Tempopo;
	if(strcmp (method_name, "SINGLE") == 0){
		Minimax_func(board_f_e, Tempopo, 0, -999999, +999999, mysymbol);
	}
	else if(strcmp (method_name, "GAME") == 0){
		Minimax_func(board_f_e, Tempopo, 0, -999999, +999999, mysymbol);
	}

	Cell Top;
	Top = maxheap_MM.top();
	printing_for_jumps(Top.current_player.x_cood, Top.current_player.y_cood, Top.root_parent.x_cood, Top.root_parent.y_cood);

	fclose(fp2);

	return 0;

}