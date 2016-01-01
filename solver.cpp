#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cassert>
#include <ctime>
using namespace std;
int dx[] = {-1,0,1,0};
int dy[] = {0,1,0,-1};

double timelimit;
clock_t cl_start,cl_end;

#define UNKNOWN (-1)
#define WHITE (0)
#define BLACK (1)

#define H_MAX (42)
#define W_MAX (42)
#define N_MAX (1770) // これをW*Hの意味として使っている場所もあるから、H_MAX*W_MAX <= N_MAXとすること
vector<string> inp;
struct P{
	int x,y;
	P(int x,int y) : x(x) , y(y) {}
};
int H,W;
int group[H_MAX][W_MAX];
int number[N_MAX];
int cellcnt[N_MAX];
int res[H_MAX][W_MAX];
vector<P> gr[N_MAX];
vector<int> G[N_MAX];
int WhitePut(int x,int y,int f=1){ // 返り値が1:矛盾発生
	if( f==0 && res[y][x] == BLACK ) return 1;
	//cout << "W:" << x << " " << y << " " << f << "{" << number[group[y][x]] << "}" << " " << cellcnt[group[y][x]] << endl;
	if( res[y][x] == WHITE ){
		if( number[group[y][x]] > cellcnt[group[y][x]] ){
			return 1;
		}
		return 0;
	}
	//cout << x << " " << y << endl;
	assert(res[y][x] != BLACK);
	res[y][x] = WHITE;
	cellcnt[group[y][x]]--;
	
	int result = 0;
	if(!f){
		int prev = -1;
		int cnt = 0;
		for(int i = 1 ; i <= W ; i++){
			if( res[y][i] == WHITE){
				if( group[y][i] != prev ){
					cnt++;
					prev = group[y][i];
				}
				if( cnt >= 3 ){ //3箇所以上跨いでしまった
					result = 1;
				}
			}else{
				cnt = 0;
				prev = -1;
			}
		}
		cnt = 0;
		prev = -1;
		for(int i = 1 ; i <= H ; i++){
			if( res[i][x] == WHITE){
				if( group[i][x] != prev ){
					cnt++;
					prev = group[i][x];
				}
				if( cnt >= 3 ){ //3箇所以上跨いでしまった
					result = 1;
				}
			}else{
				cnt = 0;
				prev = -1;
			}
		}
		if( number[group[y][x]] > cellcnt[group[y][x]] ){
			result = 1;
		}
		res[y][x] = -1;
		cellcnt[group[y][x]]++;
	}
	//cout << "W:" << x << " " << y << " " << f << "|" << number[group[y][x]] << ">" << cellcnt[group[y][x]] << "{" << result << "}" << endl;
	
	return result;
}


int BlackPut(int x,int y,int f=1){ // 返り値が1:矛盾発生
	//cout << "B:" << x << " " << y << " " << f << "{" << number[group[y][x]] << "}" << " " << cellcnt[group[y][x]] << endl;
	if( f==0 && res[y][x] == WHITE ) return 1;
	if( res[y][x] == BLACK ){
		if( number[group[y][x]] > cellcnt[group[y][x]] ){
			return 1;
		}
		return 0;
	}
	int result = 0;
	assert(res[y][x] != WHITE);
	res[y][x] = BLACK;
	cellcnt[group[y][x]]--;
	if(number[group[y][x]] != -114514 ) number[group[y][x]]--;
	if(!f){
		if( number[group[y][x]] != -114514 && number[group[y][x]] < 0 ) result = 1;
		for(int i = 0 ; i < 4 ; i++)
			if( group[y+dy[i]][x+dx[i]] != -1 && WhitePut(x+dx[i],y+dy[i],0) ){
				result = 1;
			}
		if( number[group[y][x]] > cellcnt[group[y][x]] ){
			result = 1;
		}
		res[y][x] = -1;
		if(number[group[y][x]] != -114514 ) number[group[y][x]]++;
		cellcnt[group[y][x]]++;
	}
	if(f){
		for(int i = 0 ; i < 4 ; i++)
			if( group[y+dy[i]][x+dx[i]] != -1 )
				WhitePut(x+dx[i],y+dy[i],1);
	}
	
	return result;
}

bool visited[N_MAX];
bool isAP[N_MAX]; // AriticulationPoint
int prenum[N_MAX]; int parent[N_MAX]; int lowest[N_MAX]; int timer;


void dfs( int current, int prev ){
	
    // ノード current を訪問した直後の処理
    prenum[current] = timer; lowest[current] = timer;
    timer++;

    visited[current] = true;

    int next;

	for(int i = 0 ; i < G[current].size() ; i++){
		next = G[current][i];
		if( res[next/W+1][next%W+1] == BLACK ) continue;
		
        if ( !visited[next] ){
            // ノード current からノード next へ訪問する直前の処理
            parent[next] = current;
            dfs( next, current );
            // ノード next の探索が終了した直後の処理
            lowest[current] = min( lowest[current], lowest[next] );
        } else if ( next != prev ){
            // エッジ current --> next が Back-edge の場合の処理
            lowest[current] = min( lowest[current], prenum[next] );
        }
    }
    // ノード current の探索が終了した直後の処理

}

int depthFirstSearchScheme(){ // 返り値が1のとき、書き込みは行わない
	//memset(prenum,0,sizeof(prenum));
	//memset(parent,-1,sizeof(parent));
	//memset(lowest,0,sizeof(lowest));
	//memset(visited,0,sizeof(visited));
	
	int res_memory[H_MAX][W_MAX];
	int number_memory[N_MAX];
	int cellcnt_memory[N_MAX];
	memcpy(res_memory,res,sizeof(res)); // O(WH)
	memcpy(number_memory,number,sizeof(number)); // O(WH)
	memcpy(cellcnt_memory,cellcnt,sizeof(cellcnt)); // O(WH)
    for ( int i = 0; i < H*W ; i++ ) parent[i] = visited[i] = isAP[i] = false;
    int root = -1;
	for ( int i = 1 ; i <= H ; i++){
		for(int j = 1 ; j <= W ; j++){
			if( res[i][j] != BLACK ){
				root = (i-1)*W+(j-1);
				break;
			}
		}
		if(root!=-1)break;
	}
    timer = 1;
    // lowest の計算
	parent[root] = -1;
    dfs( root, -1 );
	int cnt = 0;
	
	for(int i = 0 ; i < H*W ; i++){
		if( !visited[i] ) continue;
		if( parent[i] == root ) cnt++;
		if( res[i/W+1][i%W+1] != BLACK && root != i && parent[i] != root && prenum[parent[i]] <= lowest[i] ){
			isAP[parent[i]] = true;
		}
	}
	
	int result = 0;
	if( cnt >= 2 ) isAP[root] = true;
	for(int i = 0 ; i < H*W ; i++){
		if( isAP[i] ){
			if( res[i/W+1][i%W+1] != WHITE )
				if( WhitePut(i%W+1,i/W+1,0) ){
					memcpy(res,res_memory,sizeof(res)); // O(WH)
					memcpy(number,number_memory,sizeof(number)); // O(WH)
					memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)
					return 1;
				}else{
					WhitePut(i%W+1,i/W+1,1);
				}
		}
	}
	return 0;
}

/*
	ToDo
	仮定なしソルバー(貪欲)
	
	・関節点の検出
	・数字が書かれているところで、一通りに定まるところから決定していく
	・ある場所を空白と仮定してみてWH(W+H)あれだったら決定
	
	// ・ある場所に置いてみて即座に矛盾が生じたらポコポン～～～（優先度ってもんがあるやろ
*/


void Input(vector<string> tmp){
	memset(res,-1,sizeof(res));
	inp = tmp;
	H = (tmp.size()-1) / 2;
	W = (tmp[0].size()-1) / 4;
	memset(group,-1,sizeof(group));
	memset(cellcnt,0,sizeof(cellcnt));
	
	for(int i = 0 ; i < W*H ; i++)
		gr[i].clear() , G[i].clear();
	int y = 1 , x = 1, groupCnt = 0;
	
	for(int i = 0 ; i < W*H ; i++) number[i] = -114514;
	for(int i = 1 ; i < 2*H+1 ; i += 2){
		x = 1;
		for(int j = 1 ; j < 4*W+1 ; j += 4 ){
			int cnt = -114514;
			for(int k = 0 ; k < 3 ; k++){ //書かれている数字の処理
				if( tmp[i][j+k] != ' ' ){
					if( cnt == -114514 ) cnt = 0;
					cnt = cnt * 10 + tmp[i][j+k] - '0';
				}
			}
			if( group[y][x] == -1 ){ //新しい領域か？
				group[y][x] = groupCnt++;
			}
			cellcnt[group[y][x]]++;
			if( number[group[y][x]] == -114514 ) number[group[y][x]] = cnt;
			gr[group[y][x]].push_back(P(x,y));
			
			if( tmp[i][j+3] == ' ' ){//横に連結
				group[y][x+1] = group[y][x];
			}
			if( tmp[i+1][j] == ' ' ){ //縦に連結
				group[y+1][x] = group[y][x];
			}
			x++;
		}
		y++;
	}
	for(int i = 1 ; i <= H ; i++){
		for(int j = 1 ; j <= W ; j++){
			for(int d = 0 ; d < 4 ; d++){
				int tx = j + dx[d];
				int ty = i + dy[d];
				if( group[ty][tx] != -1 ){
					G[(i-1)*W+(j-1)].push_back((ty-1)*W+(tx-1));
				}
			}
		}
	}
}

vector<string> Output(int stringflag=0){
	int x,y;
	y = 1;
	vector<string> o = inp;
	for(int i = 1 ; i <= H ; i++){
		x = 1;
		for(int j = 1 ; j <= W ; j++){
			if( res[i][j] == BLACK ){
				for(int k = 0 ; k < 3 ; k++){
					if( o[y][x+k] == ' ' ){
						o[y][x+k] = '#';
					}
				}
			}else if( res[i][j] == WHITE ){
				if( o[y][x+2] != ' ' ){
					o[y][x+3] = '.';
				}else{
					o[y][x+2] = '.';
				}
			}
			x += 4;
		}
		y += 2;
	}
	if(!stringflag){
		for(int i = 0 ; i < o.size() ; i++){
			cout << o[i] << endl;
		}
		cout << endl;
	}
	return o;
	
}

vector<int> tmparr;
vector< vector<int> > tmps;
void backTrack(int id,int pos){
	/*for(int i = 1 ; i <= H ; i++){
		for(int j = 1 ; j <= W ; j++){
			cout << res[i][j] << " ";
		}
		cout << endl;
	}*/

	if( pos == 0 ) tmparr.resize(gr[id].size()) , tmps.clear();
	if( tmps.size() >= 3 ) return;
	if( pos == gr[id].size() ){
		tmps.push_back(tmparr);
		return;
	}
	int &x = gr[id][pos].x;
	int &y = gr[id][pos].y;
	if( res[y][x] != UNKNOWN ){
		if( !(res[y][x] == WHITE && WhitePut(x,y,0)) && !(res[y][x] == BLACK && BlackPut(x,y,0)) ){
			tmparr[pos] = res[y][x];
			backTrack(id,pos+1);
		}
	}else{
		if( depthFirstSearchScheme() ){
			return;
		}

		int res_memory[H_MAX][W_MAX];
		int number_memory[N_MAX];
		int cellcnt_memory[N_MAX];
		memcpy(res_memory,res,sizeof(res)); // O(WH)
		memcpy(number_memory,number,sizeof(number)); // O(WH)
		memcpy(cellcnt_memory,cellcnt,sizeof(cellcnt)); // O(WH)
		if( res[y][x] == WHITE ){
			tmparr[pos] = res[y][x];
			backTrack(id,pos+1);
		}else{
			int flag = BlackPut(x,y,0) | ( WhitePut(x,y,0) << 1 );
			//cout << flag << endl;
			if( flag == 0 ){
				BlackPut(x,y,1);
				tmparr[pos] = res[y][x];
				backTrack(id,pos+1);
				memcpy(res,res_memory,sizeof(res)); // O(WH)
				memcpy(number,number_memory,sizeof(number)); // O(WH)
				memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)
				WhitePut(x,y,1);
				tmparr[pos] = res[y][x];
				backTrack(id,pos+1);
			}else if( flag == 1 ){
				WhitePut(x,y,1);
				tmparr[pos] = res[y][x];
				backTrack(id,pos+1);
			}else if( flag == 2 ){
				BlackPut(x,y,1);
				tmparr[pos] = res[y][x];
				backTrack(id,pos+1);
			}
		}
		memcpy(res,res_memory,sizeof(res)); // O(WH)
		memcpy(number,number_memory,sizeof(number)); // O(WH)
		memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)
	}
}

int Solver(int depth){

	//cout << (double)(cl_end - cl_start)/ CLOCKS_PER_SEC << " " << timelimit << endl;
	int res_memory[H_MAX][W_MAX];
	int number_memory[N_MAX];
	int cellcnt_memory[N_MAX];

	while(1){
		cl_end = clock();
		if( (double)(cl_end - cl_start) / CLOCKS_PER_SEC > timelimit ){
			return 1;
			
		}
		
		if( depthFirstSearchScheme() ){
			return 1;
		}
		int unkflag = 0;
		int ok = 0;
		for(int i = 1 ; i <= H ; i++){
			for(int j = 1 ; j <= W ; j++){
				if( res[i][j] == UNKNOWN ){
					int flag = BlackPut(j,i,0) | (WhitePut(j,i,0)<<1);
					unkflag = 1;
					if( flag == 1 ){
						WhitePut(j,i,1);
						ok = 1;
						break;
					}else if(flag==2 ){
						BlackPut(j,i,1);
						ok = 1;
						break;
					}else if(flag==3){
						return 1;
					}
				}
			}
			if( ok ) break;
		}
		if(ok) continue;
		if(!unkflag) break;
		for(int i = 0 ; i < H*W ; i++){
			if( cellcnt[i] != 0 ){
				backTrack(i,0);
				if( tmps.size() == 0 ){
					return 1;
				}
				//cout << i << ": " << tmps.size() << endl;
				//if(i==1)cout << tmps.size() << endl;

				
				if( tmps.size() == 1 ){
					for(int j = 0 ; j < tmps[0].size() ; j++){
						if( tmps[0][j] == BLACK ){
							BlackPut(gr[i][j].x,gr[i][j].y,1);
						}else{
							WhitePut(gr[i][j].x,gr[i][j].y,1);
						}
					}
					ok = 1;
					break;
				}
			}
		}
		if(ok) continue;
		for(int i = 0 ; i < H*W ; i++){
			if( cellcnt[i] != 0 ){
				backTrack(i,0);
				//cout << i << ": " << tmps.size() << endl;
				vector< vector<int> > tmps = ::tmps;
				if( tmps.size() == 2 ){
					memcpy(res_memory,res,sizeof(res)); // O(WH)
					memcpy(number_memory,number,sizeof(number)); // O(WH)
					memcpy(cellcnt_memory,cellcnt,sizeof(cellcnt)); // O(WH)
					/*cout << "{" << BLACK << "}" << "~~~~ " << i << " ~~~~" << endl;
					for(int k = 0 ; k < tmps.size() ; k++){
						for(int j = 0 ; j < tmps[k].size() ; j++){
							cout << "(" << gr[i][j].x << "," << gr[i][j].y << ")" << tmps[k][j] << " ";
						}
						cout << endl;
					}*/
					int rs = 0;
					for(int j = 0 ; j < tmps[0].size() ; j++){
						if( tmps[0][j] == BLACK ){
							BlackPut(gr[i][j].x,gr[i][j].y,1);
						}else{
							WhitePut(gr[i][j].x,gr[i][j].y,1);
						}
					}
					
					if( rs || Solver(depth+1) ){
						memcpy(res,res_memory,sizeof(res)); // O(WH)
						memcpy(number,number_memory,sizeof(number)); // O(WH)
						memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)
						for(int j = 0 ; j < tmps[1].size() ; j++){
							if( tmps[1][j] == BLACK ){
								BlackPut(gr[i][j].x,gr[i][j].y,1);
							}else{
								WhitePut(gr[i][j].x,gr[i][j].y,1);
							}
						}
						if( Solver(depth+1) ){
							memcpy(res,res_memory,sizeof(res)); // O(WH)
							memcpy(number,number_memory,sizeof(number)); // O(WH)
							memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)
							return 1;
						}
					}
					ok = 1;
					break;
				}
			}
		}
		if(ok) continue;
		for(int i = 1 ; i <= H ; i++){
			for(int j = 1 ; j <= W ; j++){
				if( res[i][j] == UNKNOWN ){				
					/*if(i == 1 && j == 1 ){
						cout << i << "," << j << ":" << " "<< " " << res[i][j] << endl;
					}*/
					int flag = BlackPut(j,i,0) | (WhitePut(j,i,0)<<1);
					if( flag == 0 ){
						memcpy(res_memory,res,sizeof(res)); // O(WH)
						memcpy(number_memory,number,sizeof(number)); // O(WH)
						memcpy(cellcnt_memory,cellcnt,sizeof(cellcnt)); // O(WH)
						BlackPut(j,i,1);
						if( Solver(depth+1) ){
							memcpy(res,res_memory,sizeof(res)); // O(WH)
							memcpy(number,number_memory,sizeof(number)); // O(WH)
							memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)							
							WhitePut(j,i,1);
							if( Solver(depth+1) ){
								return 1;
							}
						}
						ok = 1;
						break;
					}
				}
			}
		}
		if(ok) continue;
	}
	
	return 0;
	
}

int ok[H_MAX][W_MAX]={};
int Multiple(){
	memset(ok,0,sizeof(ok));
	memset(res,-1,sizeof(res));
	
	int res_memory[H_MAX][W_MAX];
	int number_memory[N_MAX];
	int cellcnt_memory[N_MAX];
	memcpy(res_memory,res,sizeof(res)); // O(WH)
	memcpy(number_memory,number,sizeof(number)); // O(WH)
	memcpy(cellcnt_memory,cellcnt,sizeof(cellcnt)); // O(WH)

	for(int i = 1 ; i <= H ; i++){
		for(int j = 1 ; j <= W ; j++){
			for(int k = 0 ; k < 2 ; k++){
				if( !(ok[i][j] & (1<<k)) ){
					memcpy(res,res_memory,sizeof(res)); // O(WH)
					memcpy(number,number_memory,sizeof(number)); // O(WH)
					memcpy(cellcnt,cellcnt_memory,sizeof(cellcnt)); // O(WH)
					//cout << j << "," << i << " " << k << endl;
					//Output();

					if(k){
						if( BlackPut(j,i,0) ) continue;
						BlackPut(j,i,1);
					}else{
						if( WhitePut(j,i,0) ) continue;
						WhitePut(j,i,1);
					}
					cl_start = clock();
					int res2 = Solver(0);
					if( (double)(cl_end-cl_start) / CLOCKS_PER_SEC > timelimit - 1e-9 ) return 1;
					if( !res2 ){
						for(int y = 1 ; y <= H ; y++){
							for(int x = 1 ; x <= W ; x++){
								ok[y][x] |= (1<<res[y][x]);
								if( ok[y][x] == 3 ) return 1;
							}
						}
					}
				}
			}
		}
	}
	/*for(int i = 1 ; i <= H ; i++){
		for(int j = 1 ; j <= W ; j++){
			cout << ok[i][j] << endl;
		}
	}*/
	return ok[1][1] == 0 ? -1 : 0;
}

int main(int argc,char *argv[]){
	if( argc >= 2 ){
		timelimit = atof(argv[1]);
	}else{
		timelimit = 2e9;
	}
	
	vector<string> tmp;
	string s;
	while(getline(cin,s)){
		s.erase(remove(s.begin(),s.end(),'\r'),s.end());
		if( s != "" ) tmp.push_back(s);
	}
	
	Input(tmp);
	cl_start = clock();
	int status = Solver(0);
	if( status == 1 ){
		if(  (double)(cl_end - cl_start) / CLOCKS_PER_SEC > timelimit ){
			cerr << "Time Limit Exceeded! The solver Stopped." << endl;
			Output();
			return 2;
		}
		cerr << "This puzzle has no solution." << endl;
		return 1;
	}
	Output();
	return 0;
}

