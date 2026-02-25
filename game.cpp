#include "./game.h"

namespace GAME {
	void numberdamn() {
		cls
		setfont(300);
		cout << "数";
		S(100);
		cout << "字";
		S(100);
		cout << "炸";
		S(150);
		gotoxy(0, 0);
		cls
		S(100);
		setfont(500);
		SetColorAndBackground(6, 4);
		cout << "DAMN";
		SetColorAndBackground(7, 0);
		S(700);
		cls
		setfont(30);
		system("title 数字炸damn");
		long long min = 0, max = 100, ans = 0, input = 0;
		cout << "请输入范围\n";
		while (true) {
			cout << "最大:";
			cin >> max;
			if (max < 2 or cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "最大值必须大于或等于2,或输入错误\n";
			} else {
				break;
			}
		}
		while (true) {
			cout << "最小:";
			cin >> min;
			if (min < 0 or min >= max - 1 or cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "最小值必须大于或等于0,且最小值必须小于最大值-1,或输入错误\n";
			} else {
				break;
			}
		}
		cout << "输入完成，正在取数...\n";
		while (ans == 0) {
			ans = rand() % (max - min);
		}
		ans = ans + min;
		S(500);
		cout << "取数完成，开始游戏！\n";
		S(500);
		cls
		while (true) {
			if (max - min == 1) {
				cout << "程序错误，请重试\n";
				cout << min << "~" << max << "\n";
				break;
			}
			if (max - min == 2) {
				SetColorAndBackground(6, 4);
				cout << "DAMN!!!!!";
				SetColorAndBackground(0, 7);
				cout << "\n下一个人输了！\n炸弹是:" << ans << "\n";
				break;
			}
			cout << min << "~" << max << "\n";
			if (max - min == 3) {
				SetColorAndBackground(6, 4);
				cout << "二选一!!!!!";
				SetColorAndBackground(7, 0);
			}
			cin >> input;
			while (input <= min or input >= max or cin.fail()/*cin数字错误*/) {
				cout << "输入错误\n";
				cin.clear();//清除错误数据
				cin.ignore(numeric_limits<streamsize>::max(), '\n');//丢弃错误输入
				cout << min << "~" << max << "\n";
				cin >> input;
			}
			if (input == ans) {
				SetColorAndBackground(6, 4);
				cout << "DAMN!!!!!";
				SetColorAndBackground(0, 7);
				cout << "\n你输了！\n炸弹是:" << ans << "\n";
				if (max - ans == ans - min) {
					cout << "中位数小丑！";
				}
				break;
			} else {
				if (input < ans) {
					min = input;
				} else {
					max = input;
				}
			}
		}
		system("pause");
		SetColorAndBackground(7, 0);
		setfont(30);
	}
	struct WZQ {
		LL Q, GG;
		void color(LL a) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
		}
		struct Gomoku {
			void color(LL a) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
			}
			LL fx[4][2] = {{1, 1}, {1, 0}, {0, 1}, {1, -1}};
			string C[20] = {"●", "○", "+", "=", "|", "|", "|", "|", "|", "?"};
			LL m[50][50], nx, ny;
			void reset() {
				system("cls");
				memset(m, -1, sizeof(m));
				color(7);
				for (LL i = 1; i <= N; i++) {
					gotoxy(0, i);
					cout << C[4];
					gotoxy(N + 1, i);
					cout << C[4];
					gotoxy(i, 0);
					cout << C[3];
					gotoxy(i, N + 1);
					cout << C[3];
				}
				gotoxy(0, 0);
				cout << C[5];
				gotoxy(0, N + 1);
				cout << C[6];
				gotoxy(N + 1, 0);
				cout << C[7];
				gotoxy(N + 1, N + 1);
				cout << C[8];
				color(3);
				Forij(N) {
					gotoxy(i, j);
					cout << C[2];
				}
				nx = ny = N / 2 + 1;
				gotoxy(nx, ny);
			}
			void _drop(LL x, LL i, LL j) {
				m[i][j] = x;
				gotoxy(i, j);
				color(15);
				cout << C[x];
			}
			LL check() {
				Forij(N) {
					for (LL Fx = 0, tmp, lst, xx, yy; Fx < 4; Fx++) if (m[i][j] != -1) {
						xx = i, yy = j, tmp = 0, lst = m[i][j];
						for (LL k = 1; k <= 5; k++) {
							if (xx > N || yy > N) break;
							if (m[xx][yy] == (lst ^ 1)) {
								break;
							}
							if (m[xx][yy] == lst) tmp++;
							xx += fx[Fx][0], yy += fx[Fx][1];
						}
						if (tmp == 5) {
							return lst;
						}
					}
				}
				return -1;
			}
			LL arnd(LL x, LL y) {
				LL cnt = 0;
				for (LL i = x - 1; i <= x + 1; i++) if (i > 0 && i <= N)
					for (LL j = y - 1; j <= y + 1; j++) if (j > 0 && j <= N)
						if (m[i][j] > -1) cnt++;
				return cnt;
			}
			void get_val(LL x, LL y, LL &val) {
				val = 0;
				Forij(N) {
					for (LL Fx = 0, tmp, tk, xx, yy; Fx < 4; Fx++) {
						xx = i, yy = j, tmp = tk = 0;
						for (LL k = 1; k <= 5; k++) {
							if (xx > N || yy > N) {
								tmp = 0;
								break;
							}
							if (m[xx][yy] == (x ^ 1)) {
								tmp = 0;
								break;
							}
							if (m[xx][yy] == x) tmp++, tk += (1 << (k - 1));
							xx += fx[Fx][0], yy += fx[Fx][1];
						}
						switch (tmp) {
						case 5:
							val += 8000000000;
							break;
						case 4:
							val += 1000 + 350 * y;
							break;
						case 3:
							val += (tk == 14) ? (300 + 600 * y) : (300 + 200 * y);
							break;
						case 2:
							val += 3 + 2 * y;
							break;
						case 1:
							val += 1 + y;
							break;
						}
					}
				}
			}
			void AI(LL x) {
				LL best, brnd, bi, bj, v1, v2, kkk;
				best = -2147483647;
				brnd = -2147483647;
				Forij(N) if (m[i][j] == -1) {
					m[i][j] = x;
					get_val(x, 10, v1); //gotoxy(N+5,N/2);printf("%d                ",v1);
					get_val(x ^ 1, 80, v2); //gotoxy(N+5,N/2+1);printf("%d             ",v2);
					if (v1 - v2 > best) bi = i, bj = j, best = v1 - v2;
					if (v1 - v2 == best)
						if ((kkk = arnd(i, j)) > brnd)
							brnd = kkk, bi = i, bj = j;
					m[i][j] = -1;
				}
				_drop(x, bi, bj);
			}
			void HM(LL x) {
				char ch = getch();
				for (;; ch = getch()) {
					if (ch == 'w') {
						if (ny > 1) ny--;
					} else if (ch == 's') {
						if (ny < N) ny++;
					} else if (ch == 'a') {
						if (nx > 1) nx--;
					} else if (ch == 'd') {
						if (nx < N)nx++;
					} else if (m[nx][ny] == -1) {
						_drop(x, nx, ny);
						return;
					}
					gotoxy(nx, ny);
				}
			}
		} A;
		void HMAI() {
			A.reset();
			for (GG = -1;;) {
				gotoxy(A.nx, A.ny);
				A.HM(1);
				GG = A.check();
				if (GG > -1) break;
				A.AI(0);
				GG = A.check();
				if (GG > -1) break;
			}
			gotoxy(5, N + 3);
			if (GG == 0) printf("White wins!");
			if (GG == 1) printf("Black wins!");
			gotoxy(5, N + 4);
			printf("按任意键继续");
			getch();
			return;
		}
		void HMHM() {
			A.reset();
			for (GG = -1;;) {
				gotoxy(A.nx, A.ny);
				A.HM(1);
				GG = A.check();
				if (GG > -1) break;
				A.HM(0);
				GG = A.check();
				if (GG > -1) break;
			}
			gotoxy(5, N + 3);
			if (GG == 0) printf("White wins!");
			if (GG == 1) printf("Black wins!");
			gotoxy(5, N + 4);
			printf("按任意键继续");
			getch();
			return;
		}
		void AIAI() {
			A.reset();
			gotoxy(A.nx, A.ny);
			A.HM(1);
			A.HM(0);
			for (GG = -1;;) {
				gotoxy(A.nx, A.ny);
				A.AI(1);
				GG = A.check();
				if (GG > -1) break;
				A.AI(0);
				GG = A.check();
				if (GG > -1) break;
				Sleep(100);
			}
			gotoxy(5, N + 3);
			if (GG == 0) printf("White wins!");
			if (GG == 1) printf("Black wins!");
			gotoxy(5, N + 4);
			printf("按任意键继续");
			getch();
			return;
		}
		void wzqmain() {
			cout << "按1与AI对战  按2双人对战  按3观看AI对战\n";
			char ch = getch();
			for (;; ch = getch()) {
				if (ch == '1') {
					cout << "你选择了与AI决斗！！！\n";
					Sleep(1000);
					system("title 棋盘:人机对战");
					HMAI();
					cls
					return;
				} else if (ch == '2') {
					cout << "你选择了双人对战。\n";
					Sleep(1000);
					system("title 棋盘:人vs人模式");
					HMHM();
					cls
					return;
				} else if (ch == '3') {
					cout << "你选择了观看两个AI对战！！！\n";
					system("title 棋盘:AIvsAI");
					cout << "你需要先在棋盘上随意下2个棋子\n确定？";
					system("pause");
					AIAI();
					cls
					return;
				}
			}
			return;
		}
	} wzq;
	namespace feijidazhan {
		bool end = false;
		typedef struct Frame {
			COORD position[2];
			int flag;
		} Frame;
		void SetPos(COORD a) {
			HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(out, a);
		}
		void SetPos(int i, int j) {
			COORD pos = {static_cast<SHORT>(i), static_cast<SHORT>(j)};
			SetPos(pos);
		}
		void HideCursor() {
			CONSOLE_CURSOR_INFO cursor_info = {1, 0};
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		}
		
		void drawRow(int y, int x1, int x2, char ch) {
			SetPos(x1, y);
			for (int i = 0; i <= (x2 - x1); i++)
				cout << ch;
		}
		
		void drawRow(COORD a, COORD b, char ch) {
			if (a.Y == b.Y)
				drawRow(a.Y, a.X, b.X, ch);
			else {
				SetPos(0, 25);
				cout << "error code 01：无法填充行，因为两个坐标的纵坐标(x)不相等";
				system("pause");
			}
		}
		
		void drawCol(int x, int y1, int y2, char ch) {
			int y = y1;
			while (y != y2 + 1) {
				SetPos(x, y);
				cout << ch;
				y++;
			}
		}
		
		void drawCol(COORD a, COORD b, char ch) {
			if (a.X == b.X)
				drawCol(a.X, a.Y, b.Y, ch);
			else {
				SetPos(0, 25);
				cout << "error code 02：无法填充列，因为两个坐标的横坐标(y)不相等";
				system("pause");
			}
		}
		
		void drawFrame(COORD a, COORD b, char row, char col) {
			drawRow(a.Y, a.X + 1, b.X - 1, row);
			drawRow(b.Y, a.X + 1, b.X - 1, row);
			drawCol(a.X, a.Y + 1, b.Y - 1, col);
			drawCol(b.X, a.Y + 1, b.Y - 1, col);
		}
		void drawFrame(int x1, int y1, int x2, int y2, char row, char col) {
			COORD a = {static_cast<SHORT>(x1), static_cast<SHORT>(y1)};
			COORD b = {static_cast<SHORT>(x2), static_cast<SHORT>(y2)};
			drawFrame(a, b, row, col);
		}
		void drawFrame(Frame frame, char row, char col) {
			COORD a = frame.position[0];
			COORD b = frame.position[1];
			drawFrame(a, b, row, col);
		}
		void drawPlaying() {
			drawFrame(0, 0, 48, 24, '=', '|');
			drawFrame(49, 0, 79, 4, '-', '|');
			drawFrame(49, 4, 79, 9, '-', '|');
			drawFrame(49, 9, 79, 20, '-', '|');
			drawFrame(49, 20, 79, 24, '-', '|');
			SetPos(52, 6);
			cout << "得分：";
			SetPos(52, 7);
			cout << "称号：";
			SetPos(52, 10);
			cout << "操作方式：";
			SetPos(52, 12);
			cout << " a,s,d,w 控制战机移动。";
			SetPos(52, 14);
			cout << " p 暂停游戏。";
			SetPos(52, 16);
			cout << " e 退出游戏。";
			SetPos(52, 18);
			cout << " 空格 发射子弹。";
		}
		
		int random(int a, int b) {
			int c = (rand() % (a - b)) + a;
			return c;
		}
		
		COORD random(COORD a, COORD b) {
			int x = random(a.X, b.X);
			int y = random(a.Y, b.Y);
			COORD c = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
			return c;
		}
		bool judgeCoordInFrame(Frame frame, COORD spot) {
			if (spot.X >= frame.position[0].X)
				if (spot.X <= frame.position[1].X)
					if (spot.Y >= frame.position[0].Y)
						if (spot.Y <= frame.position[0].Y)
							return true;
			return false;
		}
		void printCoord(COORD a) {
			cout    << "( " << a.X << " , " << a.Y << " )";
		}
		void printFrameCoord(Frame a) {
			printCoord(a.position[0]);
			cout    << " - ";
			printCoord(a.position[1]);
		}
		int drawMenu() {
			SetPos(30, 1);
			cout << "飞 机 大 战";
			drawRow(3, 0, 79, '-');
			drawRow(5, 0, 79, '-');
			SetPos(28, 4);
			cout << "w 和 s 选择， 空格 确定";
			SetPos(15, 11);
			cout << "1. 简单的敌人";
			SetPos(15, 13);
			cout << "2. 冷酷的敌人";
			drawRow(20, 0, 79, '-');
			drawRow(22, 0, 79, '-');
			SetPos(47, 11);
			cout << "简单的敌人：";
			SetPos(51, 13);
			cout << "简单敌人有着较慢的移动速度。";
			SetPos(24, 21);
			cout << " ☆○☆○☆○☆○☆○☆ ";
			int j = 11;
			SetPos(12, j);
			cout << ">>";
			while (1) {
				if ( _kbhit() ) {
					char x = _getch();
					switch (x) {
						case 'w' : {
						if ( j == 13) {
							SetPos(12, j);
							cout << "　";
							j = 11;
							SetPos(12, j);
							cout << ">>";
							SetPos(51, 13);
							cout << "　　　　　　　　　　　　";
							SetPos(47, 11);
							cout << "简单的敌人：";
							SetPos(51, 13);
							cout << "简单敌人有着较慢的移动速度。";
						}
						break;
					}
						case 's' : {
							if ( j == 11 ) {
								SetPos(12, j);
								cout << "　";
								j = 13;
								SetPos(12, j);
								cout << ">>";
								SetPos(51, 13);
								cout << "　　　　　　　　　　　　　　";
								SetPos(47, 11);
								cout << "冷酷的敌人：";
								SetPos(51, 13);
								cout << "冷酷的敌人移动速度较快。";
							}
							break;
						}
						case ' ' : {
							if (j == 8)    return 1;
							else return 2;
						}
					}
				}
			}
		}
		class Game {
		public:
			COORD position[10] = {};
			COORD bullet[10] = {};
			Frame enemy[8] = {};
			int score = 0;
			int rank = 0;
			int rankf = 0;
			string title = "";
			int flag_rank = 0;
			Game ();
			
			void initPlane();
			void initBullet();
			void initEnemy();
			
			void planeMove(char);
			void bulletMove();
			void enemyMove();
			
			void drawPlane();
			void drawPlaneToNull();
			void drawBullet();
			void drawBulletToNull();
			void drawEnemy();
			void drawEnemyToNull();
			
			void drawThisBulletToNull( COORD );
			void drawThisEnemyToNull( Frame );
			void Pause();
			void Playing();
			void judgePlane();
			void judgeEnemy();
			void Shoot();
			void GameOver();
			void printScore();
		};
		Game::Game() {
			initPlane();
			initBullet();
			initEnemy();
			score = 0;
			rank = 25;
			rankf = 0;
			flag_rank = 0;
		}
		void Game::initPlane() {
			COORD centren = {39, 22};
			position[0].X = position[5].X = position[7].X = position[9].X = centren.X;
			position[1].X = centren.X - 2;
			position[2].X = position[6].X = centren.X - 1;
			position[3].X = position[8].X = centren.X + 1;
			position[4].X = centren.X + 2;
			for (int i = 0; i <= 4; i++)
				position[i].Y = centren.Y;
			for (int i = 6; i <= 8; i++)
				position[i].Y = centren.Y + 1;
			position[5].Y = centren.Y - 1;
			position[9].Y = centren.Y - 2;
		}
		void Game::drawPlane() {
			for (int i = 0; i < 9; i++) {
				SetPos(position[i]);
				if (i != 5)
					cout << "o";
				else if (i == 5)
					cout << "|";
			}
		}
		void Game::drawPlaneToNull() {
			for (int i = 0; i < 9; i++) {
				SetPos(position[i]);
				cout << " ";
			}
		}
		void Game::initBullet() {
			for (int i = 0; i < 10; i++)
				bullet[i].Y = 30;
		}
		void Game::drawBullet() {
			for (int i = 0; i < 10; i++) {
				if ( bullet[i].Y != 30) {
					SetPos(bullet[i]);
					cout << "^";
				}
			}
		}
		void Game::drawBulletToNull() {
			for (int i = 0; i < 10; i++)
				if ( bullet[i].Y != 30 ) {
					COORD pos = {static_cast<SHORT>(bullet[i].X), static_cast<SHORT>(bullet[i].Y + 1)};
					SetPos(pos);
					cout << " ";
				}
		}
		void Game::initEnemy() {
			COORD a = {1, 1};
			COORD b = {45, 15};
			for (int i = 0; i < 8; i++) {
				enemy[i].position[0] = random(a, b);
				enemy[i].position[1].X = enemy[i].position[0].X + 3;
				enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
			}
		}
		void Game::drawEnemy() {
			for (int i = 0; i < 8; i++)
				drawFrame(enemy[i].position[0], enemy[i].position[1], '-', '|');
		}
		void Game::drawEnemyToNull() {
			for (int i = 0; i < 8; i++) {
				drawFrame(enemy[i].position[0], enemy[i].position[1], ' ', ' ');
			}
		}
		void Game::Pause() {
			SetPos(61, 2);
			cout << " ";
			SetPos(61, 2);
			cout << "暂停中...";
			char c = _getch();
			while (c != 'p')
				c = _getch();
			SetPos(61, 2);
			cout << "         ";
		}
		void Game::planeMove(char x) {
			if (x == 'a')
				if (position[1].X != 1)
					for (int i = 0; i <= 9; i++)
						position[i].X -= 2;
			if (x == 's')
				if (position[7].Y != 23)
					for (int i = 0; i <= 9; i++)
						position[i].Y += 1;
			if (x == 'd')
				if (position[4].X != 47)
					for (int i = 0; i <= 9; i++)
						position[i].X += 2;
			if (x == 'w')
				if (position[5].Y != 3)
					for (int i = 0; i <= 9; i++)
						position[i].Y -= 1;
		}
		void Game::bulletMove() {
			for (int i = 0; i < 10; i++) {
				if ( bullet[i].Y != 30) {
					bullet[i].Y -= 1;
					if ( bullet[i].Y == 1 ) {
						COORD pos = {static_cast<SHORT>(bullet[i].X), static_cast<SHORT>(bullet[i].Y + 1)};
						drawThisBulletToNull( pos );
						bullet[i].Y = 30;
					}
				}
			}
		}
		void Game::enemyMove() {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 2; j++)
					enemy[i].position[j].Y++;
				if (24 == enemy[i].position[1].Y) {
					COORD a = {1, 1};
					COORD b = {45, 3};
					enemy[i].position[0] = random(a, b);
					enemy[i].position[1].X = enemy[i].position[0].X + 3;
					enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
				}
			}
		}
		void Game::judgePlane() {
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 9; j++)
					if (judgeCoordInFrame(enemy[i], position[j])) {
						SetPos(62, 1);
						cout << "坠毁";
						drawFrame(enemy[i], '+', '+');
						Sleep(1000);
						GameOver();
						return;
					}
		}
		void Game::drawThisBulletToNull( COORD c) {
			SetPos(c);
			cout << " ";
		}
		void Game::drawThisEnemyToNull( Frame f ) {
			drawFrame(f, ' ', ' ');
		}
		void Game::judgeEnemy() {
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 10; j++)
					if ( judgeCoordInFrame(enemy[i], bullet[j]) ) {
						score += 5;
						drawThisEnemyToNull( enemy[i] );
						COORD a = {1, 1};
						COORD b = {45, 3};
						enemy[i].position[0] = random(a, b);
						enemy[i].position[1].X = enemy[i].position[0].X + 3;
						enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
						drawThisBulletToNull( bullet[j] );
						bullet[j].Y = 30;
					}
		}
		void Game::Shoot() {
			for (int i = 0; i < 10; i++)
				if (bullet[i].Y == 30) {
					bullet[i].X = position[5].X;
					bullet[i].Y = position[5].Y - 1;
					break;
				}
		}
		void Game::printScore() {
			if (score == 120 && flag_rank == 0) {
				rank -= 3;
				flag_rank = 1;
			} else if ( score == 360 && flag_rank == 1) {
				rank -= 5;
				flag_rank = 2;
			} else if ( score == 480 && flag_rank == 2) {
				rank -= 5;
				flag_rank = 3;
			}
			int x = rank / 5;
			SetPos(60, 6);
			cout << score;
			if ( rank != rankf ) {
				SetPos(60, 7);
				if ( x == 5)
					title = "高级飞行员";
				else if ( x == 4)
					title = "神级飞行员";
				else if ( x == 3)
					title = "死神级飞行员";
				else if ( x == 2 )
					title = "海神级飞行员";
				else if ( x == 1 )
					title = "修罗神飞行员";
				cout << title;
			}
			rankf = rank;
		}
		void Game::Playing() {
			drawEnemy();
			drawPlane();
			int flag_bullet = 0;
			int flag_enemy = 0;
			while (1) {
				Sleep(8);
				if (_kbhit()) {
					char x = _getch();
					if ('a' == x || 's' == x || 'd' == x || 'w' == x) {
						drawPlaneToNull();
						planeMove(x);
						drawPlane();
						judgePlane();
						if (end == true) {
							return;
						}
					}
					if ('p' == x)
						Pause();
					if ( ' ' == x)
						Shoot();
					if ( 'e' == x) {
						GameOver();
						return;
					}
				}
				
				if ( 0 == flag_bullet ) {
					bulletMove();
					drawBulletToNull();
					drawBullet();
					judgeEnemy();
				}
				flag_bullet++;
				if ( 5 == flag_bullet )
					flag_bullet = 0;
				
				if ( 0 == flag_enemy ) {
					drawEnemyToNull();
					enemyMove();
					drawEnemy();
					judgePlane();
					if (end == true) {
						return;
					}
				}
				flag_enemy++;
				if ( flag_enemy >= rank )
					flag_enemy = 0;
				printScore();
			}
		}
		void Game::GameOver() {
			system("cls");
			COORD p1 = {28, 9};
			COORD p2 = {53, 15};
			drawFrame(p1, p2, '=', '|');
			SetPos(36, 12);
			string str = "Game Over!";
			for (int i = 0; i < str.size(); i++) {
				Sleep(80);
				cout << str[i];
			}
			Sleep(1000);
			system("cls");
			drawFrame(p1, p2, '=', '|');
			SetPos(31, 11);
			cout << "击落敌机：" << score / 5 << " 架";
			SetPos(31, 12);
			cout << "得　　分：" << score;
			SetPos(31, 13);
			cout << "获得称号：" << title;
			SetPos(30, 16);
			Sleep(1000);
			cout << "重来？ 是（y）| 否（n）";
			as:
			char x = _getch();
			if (x == 'n') {
				end = true;
				return;
			} else if (x == 'y') {
				end = false;
				system("cls");
				Game game;
				int a = drawMenu();
				if (a == 2)
					game.rank = 20;
				system("cls");
				drawPlaying();
				game.Playing();
			} else goto as;
		}
		void main() {
			end = false;
			srand((int)time(0));
			Game game;
			int a = drawMenu();
			if (a == 2)
				game.rank = 20;
			system("cls");
			drawPlaying();
			game.Playing();
			return;
		}
	}
	namespace emlpd {
		bool end = false;
		int Your = 0, Other = 0;
		string daojuname[] = {"放大镜", "手铐", "小刀", "烟", "饮料"};
		double Yourmoney = 0;
		int huihe = 0;
		bool emoqbq = 1;
		bool ndqbq = 1;
		
		int shi = 0, kong = 0;
		int q[10] = {}, qlen = 0;
		int Rand(int x, int y) {
			int A = rand(), B = rand();
			return A * 1ll * B % (y - x + 1) + x;
		}
		int T = 0;
		int daojulen = 0;
		int daoju[10] = {};
		int daojulen1 = 0;
		int daoju1[10] = {};
		void build_gun() {
			kong = Rand(1, 4);
			shi = Rand(1, 4);
			qlen = 0;
			printf("%d发实弹,%d发空弹\n", shi, kong);
			int a1 = kong, a2 = shi;
			for (int i = 1; i <= kong + shi; i++) {
				int sum = Rand(1, a1 + a2);
				if (sum <= a1) {
					a1--;
					q[++qlen] = 2;
				} else {
					a2--;
					q[++qlen] = 1;
				}
			}
			if (huihe != 1) {
				int maxn = min(4, 8 - daojulen);
				printf("你获得了%d个道具:\n", maxn);
				daojulen += maxn;
				for (int i = 1; i <= maxn; i++) {
					int kkk = Rand(0, 4);
					daoju[kkk]++;
					cout << daojuname[kkk];
					if (i != maxn) {
						printf(",");
					}
				}
				printf("\n");
				maxn = min(4, 8 - daojulen1);
				printf("恶魔获得了%d个道具:\n", maxn);
				daojulen1 += maxn;
				for (int i = 1; i <= maxn; i++) {
					int kkk = Rand(0, 4);
					daoju1[kkk]++;
					cout << daojuname[kkk];
					if (i != maxn) {
						printf(",");
					}
				}
			}
			printf("\n");
			system("pause");
			system("cls");
		}
		void IsOver() {
			if (Your <= 0) {
				printf("你输了\n");
				system("pause");
				end = true;
				return;
			}
			if (Other <= 0) {
				if (huihe == 3) {
					Yourmoney /= 3;
					printf("你赢了\n你获得了奖金$%.2lf\n", Yourmoney);
					system("pause");
					end = true;
					return;
				} else if (huihe == 2) {
					printf("进入第三回合\n");
					huihe = 3;
					Your = 6;
					Other = 6;
					system("pause");
					system("cls");
					build_gun();
				} else if (huihe == 1) {
					printf("进入第二回合\n");
					huihe = 2;
					Your = 4;
					Other = 4;
					system("pause");
					system("cls");
					build_gun();
				}
			}
		}
		void wait() {
			for (int i = 1; i <= 3; i++) {
				Sleep(500);
				printf(".");
			}
			Sleep(500);
		}
		int Hurt = 1;
		int shoukao_you = 0;
		void Timeyou() {
			int x;
			while (1) {
				if (!ndqbq) {
					printf("你的生命:@#%&%%&@#%@%*&*\n");
				} else {
					printf("你的生命:%d/%d\n", Your, huihe * 2);
				}
				if (!emoqbq) {
					printf("恶魔生命:@$%#^%%^%&*&%%$\n");
				} else {
					printf("恶魔生命:%d/%d\n", Other, huihe * 2);
				}
				printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
				if (huihe != 1) {
					printf("你现在拥有的道具:\n");
					for (int i = 0; i <= 4; i++) {
						cout << daojuname[i];
						printf("%d", daoju[i]);
						printf("个");
						if (i != 4) {
							printf(",");
						}
					}
					printf("\n");
					printf("恶魔现在拥有的道具:\n");
					for (int i = 0; i <= 4; i++) {
						cout << daojuname[i];
						printf("%d", daoju1[i]);
						printf("个");
						if (i != 4) {
							printf(",");
						}
					}
				}
				printf("\n");
				printf("现在是你的回合\n");
				printf("你要\n1.向恶魔开枪\n2.向自己开枪\n");
				if (huihe != 1) {
					for (int i = 0; i <= 4; i++) {
						printf("%d.使用", i + 3);
						cout << daojuname[i] << '\n';
					}
				}
				scanf("%d", &x);
				if ((huihe == 1 && 1 <= x && x <= 2) || (huihe != 1 && 1 <= x && x <= 7)) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("你决定向恶魔开枪");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为你使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Other -= Hurt;
					
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver();
					if (end == true) {
						return;
					}
					if (huihe == 3 && Other <= 2) {
						printf("恶魔的起搏器断开!\n");
						emoqbq = 1;
						Yourmoney += 5000.0;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						Yourmoney += 1000.0;
						printf("因为你使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("你决定向自己开枪");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Your -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver();
					if (end == true) {
						return;
					}
					if (huihe == 3 && Your <= 2) {
						printf("你的起搏器断开!\n");
						ndqbq = 1;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为你使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) {
				if (daoju[0]) {
					daoju[0]--;
					daojulen--;
					printf("你使用了放大镜\n");
					wait();
					printf("\n你看到了");
					if (q[qlen] == 1) {
						printf("实弹\n");
						Yourmoney += 2500.0;
					} else {
						printf("空弹\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("你现在没有放大镜\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju[1]) {
					if (!shoukao_you) {
						daoju[1]--;
						daojulen--;
						printf("你使用了手铐\n");
						printf("你获得了连开两枪的机会\n");
						shoukao_you = 1;
					} else {
						printf("你已经用过手铐了\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("你现在没有手铐\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju[2]) {
					daoju[2]--;
					daojulen--;
					printf("你使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍\n");
					Hurt *= 2;
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
				} else {
					printf("你现在没有小刀\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju[3]) {
					if (Your ^ 6) {
						daoju[3]--;
						daojulen--;
						printf("你使用了烟\n");
						printf("你回复了一点生命\n");
						Yourmoney += 500.0;
						Your++;
					} else {
						printf("你现在不需要烟\n");
					}
					Sleep(1145);
				} else {
					printf("你现在没有烟\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju[4]) {
					daoju[4]--;
					daojulen--;
					printf("你使用了饮料\n");
					wait();
					printf("\n");
					printf("你退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("你现在没有饮料\n");
					Sleep(1145);
					system("cls");
				}
			}
			Sleep(1000);
			system("cls");
		}
		int Know = 0;
		int shoukaoemo = 0;
		void fightyou() {
			printf("恶魔决定向你开枪");
			T++;
			wait();
			if (q[qlen] == 2) {
				Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
				kong--;
				qlen--;
				Hurt = 1;
				Know = 0;
				printf("是空弹\n");
				if (shoukaoemo) {
					printf("因为恶魔使用了手铐，所以可以再来一次\n");
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			} else {
				Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
				shi--;
				qlen--;
				Your -= Hurt;
				printf("是实弹\n");
				printf("造成了%d点伤害\n", Hurt);
				Hurt = 1;
				Know = 0;
				Sleep(500);
				IsOver();
				if (end == true) {
					return;
				}
				if (huihe == 3 && Your <= 2) {
					printf("你的起搏器断开!\n");
					ndqbq = 1;
				}
				if (shoukaoemo) {
					printf("因为恶魔使用了手铐，所以可以再来一次\n");
					Yourmoney += 1000.0;
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			}
		}
		void fightemo() {
			printf("恶魔决定向自己开枪");
			wait();
			if (q[qlen] == 2) {
				Yourmoney += 2000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
				kong--;
				qlen--;
				printf("是空弹\n");
				Know = 0;
			} else {
				Yourmoney += 5000.0 * (1 + (double)(kong) * 1.0 / (double)(shi + kong));
				shi--;
				T++;
				qlen--;
				Other -= Hurt;
				printf("是实弹\n");
				printf("造成了%d点伤害\n", Hurt);
				Hurt = 1;
				Know = 0;
				Sleep(500);
				IsOver();
				if (end == true) {
					return;
				}
				if (huihe == 3 && Other <= 2) {
					printf("恶魔的起搏器断开!\n");
					emoqbq = 1;
					Yourmoney += 5000.0;
				}
				if (shoukaoemo) {
					printf("因为恶魔使用了手铐，所以可以再来一次\n");
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			}
		}
		void Timeother() {
			if (!ndqbq) {
				printf("你的生命:@#%&%%&@#%@%*&*\n");
			} else {
				printf("你的生命:%d/%d\n", Your, huihe * 2);
			}
			if (!emoqbq) {
				printf("恶魔生命:@$%#^%%^%&*&%%$\n");
			} else {
				printf("恶魔生命:%d/%d\n", Other, huihe * 2);
			}
			printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
			if (huihe != 1) {
				printf("恶魔现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
			}
			printf("现在是恶魔的回合\n");
			Sleep(1500);
			if (Other != 6) {
				if (daoju1[3]) {
					daoju1[3]--;
					daojulen1--;
					printf("恶魔使用了烟\n");
					printf("恶魔回复了一点生命\n");
					Other++;
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
					return ;
				}
			}
			if (Know == 0 && kong == 0) {
				Know = 1;
			}
			if (Know == 0) {
				if (abs(shi - kong) < 2 && kong != 0) {
					if (daoju1[0]) {
						daoju1[0]--;
						daojulen1--;
						printf("恶魔使用了放大镜\n");
						wait();
						printf("\nVery Interesting");
						if (q[qlen] == 1) {
							Yourmoney += 2500.0;
							Know = 1;
						} else {
							Know = 2;
						}
						Sleep(1145);
						system("cls");
						return ;
					}
				}
			} else if (Know == 1) {
				if (daoju1[2]) {
					daoju1[2]--;
					daojulen1--;
					Hurt *= 2;
					printf("恶魔使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍");
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
					return;
				} else {
					if (shi >= kong + 1 && daoju1[1] && shoukaoemo != 1) {
						daoju1[1]--;
						daojulen1--;
						shoukaoemo = 1;
						printf("恶魔使用了手铐\n");
						printf("恶魔获得了连开两枪的机会\n");
						Sleep(1145);
						system("cls");
						return ;
					}
					fightyou();
					system("cls");
					return ;
				}
			} else {
				if (daoju1[4]) {
					daoju1[4]--;
					daojulen1--;
					printf("恶魔使用了饮料\n");
					wait();
					printf("\n");
					printf("恶魔退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						shi--;
					}
					Know = 0;
					qlen--;
					Sleep(500);
					Sleep(1145);
					system("cls");
					return ;
				} else {
					fightemo();
					Sleep(1145);
					system("cls");
					return ;
				}
			}
			if (shi >= kong) {
				fightyou();
			} else {
				fightemo();
			}
			Sleep(1145);
			system("cls");
		}
		void Play() {
			while (1) {
				if (end == true) {
					return;
				}
				if (shi == 0) {
					build_gun();
					T = 0;
					continue;
				}
				if (T % 2 == 0) {
					Timeyou();
				} else {
					Timeother();
				}
			}
		}
		void danrenplay() {
			for (int i = 1; i <= 3; i++) {
				printf(".");
//      Sleep(1000);
			}
			printf("\n");
			printf("又来了一位挑战者...\n");
			Sleep(1000);
			int x;
			while (1) {
				printf("准备好参与恶魔的游戏吗？胜者带走奖金，败者将会在此长眠\n1.好的\n2.没问题\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			while (1) {
				printf("你清楚我们的规则吗？\n1.清楚\n2.不清楚\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 2) {
				for (int i = 1; i <= 3; i++) {
					printf(".");
					Sleep(1000);
				}
				printf("\n");
				printf("规则:\n");
				printf("你和恶魔都各有6点生命\n") ;
				printf("每轮开始前，你将知道一共有几发实弹，几发空弹\n");
				printf("每轮你可以选择对自己开枪，对恶魔开枪或者使用道具\n");
				printf("如果你对自己开枪，若为空弹，则可以继续行动，否则，停止行动\n");
				printf("如果你对恶魔开枪，无论如何，都将停止行动\n");
				printf("第二、三回合每轮开始前双方都将获得4个道具作为补给（上限为8个）\n");
				printf("到了第三回合，若有一方只剩两点生命，则该方的起搏器将被销毁，无法看见自己血量！\n");
				printf("道具一览：\n");
				printf("放大镜：可以知道下一发子弹是空弹还是实弹\n");
				printf("手铐：增加一次本回合的行动次数\n");
				printf("小刀：若下一发子弹为实弹，则伤害翻倍\n");
				printf("烟：可以回复1点体力\n");
				printf("饮料：可以退一发子弹\n");
				system("pause");
				system("cls");
			}
			printf("好吧\n");
			Sleep(1145);
			printf("游戏将要开始了哦\n");
			Sleep(1145);
			huihe = 1;
			Your = 2;
			Other = 2;
			system("cls");
			Play();
			return;
		}
		void IsOver_duo() {
			if (Your <= 0) {
				printf("玩家B赢了\n玩家B获得了奖金$%.2lf\n", Yourmoney);
				system("pause");
				end = true;
				return;
			} else if (Other <= 0) {
				printf("玩家A赢了\n玩家A获得了奖金$%.2lf\n", Yourmoney);
				system("pause");
				end = true;
				return;
			}
			
		}
		void build_gun_duo() {
			kong = Rand(1, 4);
			shi = Rand(1, 4);
			qlen = 0;
			printf("%d发实弹,%d发空弹\n", shi, kong);
			int a1 = kong, a2 = shi;
			for (int i = 1; i <= kong + shi; i++) {
//      Sleep(50);
				int sum = Rand(1, a1 + a2);
				if (sum <= a1) {
					a1--;
					q[++qlen] = 2;
				} else {
					a2--;
					q[++qlen] = 1;
				}
			}
			int maxn = min(2, 8 - daojulen);
			printf("玩家A获得了%d个道具:\n", maxn);
			daojulen += maxn;
			for (int i = 1; i <= maxn; i++) {
//      Sleep(50);
				int kkk = Rand(0, 4);
				daoju[kkk]++;
				cout << daojuname[kkk];
				if (i != maxn) {
					printf(",");
				}
			}
			printf("\n");
			maxn = min(2, 8 - daojulen1);
			printf("玩家B获得了%d个道具:\n", maxn);
			daojulen1 += maxn;
			for (int i = 1; i <= maxn; i++) {
				int kkk = Rand(0, 4);
				daoju1[kkk]++;
				cout << daojuname[kkk];
				if (i != maxn) {
					printf(",");
				}
			}
			printf("\n");
			system("pause");
			system("cls");
		}
		void Timeyou_duo() {
			int x;
			while (1) {
				printf("玩家A的生命:%d/4\n玩家B的生命:%d/4\n", Your, Other);
				printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
				printf("玩家A现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("玩家B现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("现在是玩家A的回合\n");
				printf("玩家A要\n1.向玩家B开枪\n2.向自己开枪\n");
				for (int i = 0; i <= 4; i++) {
					printf("%d.使用", i + 3);
					cout << daojuname[i] << '\n';
				}
				scanf("%d", &x);
				if (1 <= x && x <= 7) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("玩家A决定向玩家B开枪");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为玩家A使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Other -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						Yourmoney += 1000.0;
						printf("因为玩家A使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("玩家A决定向自己开枪");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Your -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为玩家A使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) { //{"放大镜","手铐","小刀","烟","饮料"};
				if (daoju[0]) {
					daoju[0]--;
					daojulen--;
					printf("玩家A使用了放大镜\n");
					wait();
					printf("\n玩家A看到了");
					if (q[qlen] == 1) {
						printf("实弹\n");
						Yourmoney += 2500.0;
					} else {
						printf("空弹\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("玩家A现在没有放大镜\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju[1]) {
					if (!shoukao_you) {
						daoju[1]--;
						daojulen--;
						printf("玩家A使用了手铐\n");
						printf("玩家A获得了连开两枪的机会\n");
						shoukao_you = 1;
					} else {
						printf("玩家A已经用过手铐了\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家A现在没有手铐\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju[2]) {
					daoju[2]--;
					daojulen--;
					printf("玩家A使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍\n");
					Yourmoney += 500.0;
					Hurt *= 2;
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家A现在没有小刀\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju[3]) {
					if (Your ^ 4) {
						daoju[3]--;
						daojulen--;
						printf("玩家A使用了烟\n");
						printf("玩家A回复了一点生命\n");
						Yourmoney += 500.0;
						Your++;
					} else {
						printf("玩家A现在不需要烟\n");
					}
					Sleep(1145);
				} else {
					printf("玩家A现在没有烟\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju[4]) {
					daoju[4]--;
					daojulen--;
					printf("玩家A使用了饮料\n");
					wait();
					printf("\n");
					printf("玩家A退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("玩家A现在没有饮料\n");
					Sleep(1145);
					system("cls");
				}
			}
			Sleep(1000);
			system("cls");
		}
		void Timeother_duo() {
			int x;
			while (1) {
				printf("玩家A的生命:%d/4\n玩家B的生命:%d/4\n", Your, Other);
				printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
				printf("玩家A现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("玩家B现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("现在是玩家B的回合\n");
				printf("玩家B要\n1.向玩家A开枪\n2.向自己开枪\n");
				for (int i = 0; i <= 4; i++) {
					printf("%d.使用", i + 3);
					cout << daojuname[i] << '\n';
				}
				scanf("%d", &x);
				if (1 <= x && x <= 7) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("玩家B决定向玩家A开枪");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
					if (shoukaoemo == 1) {
						shoukaoemo = 0;
						printf("因为玩家B使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Your -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukaoemo == 1) {
						shoukaoemo = 0;
						Yourmoney += 1000.0;
						printf("因为玩家B使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("玩家B决定向自己开枪");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Other -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为玩家B使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) {
				if (daoju1[0]) {
					daoju1[0]--;
					daojulen1--;
					printf("玩家B使用了放大镜\n");
					wait();
					printf("\n玩家B看到了");
					if (q[qlen] == 1) {
						printf("实弹\n");
						Yourmoney += 2500.0;
					} else {
						printf("空弹\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("玩家B现在没有放大镜\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju1[1]) {
					if (!shoukaoemo) {
						daoju1[1]--;
						daojulen1--;
						printf("玩家B使用了手铐\n");
						printf("玩家B获得了连开两枪的机会\n");
						shoukaoemo = 1;
					} else {
						printf("玩家B已经用过手铐了\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家B现在没有手铐\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju1[2]) {
					daoju1[2]--;
					daojulen1--;
					printf("玩家B使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍\n");
					Yourmoney += 500.0;
					Hurt *= 2;
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家B现在没有小刀\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju1[3]) {
					if (Other ^ 4) {
						daoju1[3]--;
						daojulen1--;
						printf("玩家B使用了烟\n");
						printf("玩家B回复了一点生命\n");
						Yourmoney += 500.0;
						Other++;
					} else {
						printf("玩家B现在不需要烟\n");
					}
					Sleep(1145);
				} else {
					printf("玩家B现在没有烟\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju1[4]) {
					daoju1[4]--;
					daojulen1--;
					printf("玩家B使用了饮料\n");
					wait();
					printf("\n");
					printf("玩家B退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("玩家B现在没有饮料\n");
					Sleep(1145);
					system("cls");
				}
			}
			Sleep(1000);
			system("cls");
		}
		int asdasd = 0;
		void duorenplay() {
			while (1) {
				if (end == true) {
					return;
				}
				if (shi == 0) {
					build_gun_duo();
					T = asdasd;
					asdasd++;
					continue;
				}
				if (T % 2 == 0) {
					Timeyou_duo();
				} else {
					Timeother_duo();
				}
			}
		}
		int main() {
			end = false;
			srand(time(0));
			int x;
			while (1) {
				printf("请选择你想要的模式：\n1.单人\n2.双人（此模式中，生命值为4，道具补给为2）\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			system("cls");
			if (x == 1) {
				danrenplay();
			} else {
				Your = Other = 4;
				duorenplay();
			}
			return 0;
		}
	}
	void game(string name){
		if(name=="numberdamn"){
			numberdamn();
		}
		if(name=="wzq"){
			wzq.wzqmain();
		}
		if(name=="fjdz"){
			feijidazhan::main();
		}
		if(name=="emlpd"){
			emlpd::main();
		}
		return;
	}
}
