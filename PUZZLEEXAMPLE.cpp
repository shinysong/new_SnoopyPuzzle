#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bangtal.h>
#include <iostream>
#include <cstdio>
using namespace bangtal;
using namespace std;

bool checked(int j, int blank, int p_width, int p_num) {
	return ((j >= 0 && j < p_num && ((j% p_width > 0 && j - 1 == blank) ||
		(j % p_width < p_width-1 && j + 1 == blank) ||
		(j > p_width - 1 && j - p_width == blank) ||
		(j < p_num - p_width + 1 && j + p_width == blank))));
}


int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	srand((unsigned)time(NULL));
	clock_t starttime, endtime;
	clock_t m_starttime, m_endtime;
	clock_t h_starttime, h_endtime;
	auto scene = Scene::create("스누피퍼즐", "low/white.png");
	auto scene1 = Scene::create("난이도 하", "low/배경.png");
	auto scene2 = Scene::create("난이도 중", "mid/배경.png");
	auto scene3 = Scene::create("난이도 상", "high/배경.png");
	auto low = Object::create("low/원본.png", scene, 50, 300);
	auto mid = Object::create("mid/원본.png", scene, 450, 300);
	auto high = Object::create("high/원본.png", scene, 850, 300);
	low->setScale(0.3f);
	mid->setScale(0.3f);
	high->setScale(0.3f);
	auto RestartButton = Object::create("low/restart.png", scene, 550, 400, false);
	auto endButton = Object::create("low/end.png", scene, 590, 70); // 다시 시작하는 버튼
	auto home = Object::create("low/home.png", scene, 590, 70, false); // 처음화면으로 갈 수 있는 버튼
	home->setScale(0.5f);
	int stage = 0; //현재 있는 단계 1은 하, 2는 중, 3은 상
	int shuffle_num = 200; //섞는 수
	//------------------------------------------------------------------------------------------
	const int p_num = 9;
	const int p_width = 3;
	const int pointX = 426;
	const int pointY = 436;
	const int radius = 142;
	ObjectPtr game_board[p_num];
	ObjectPtr init_board[p_num];
	int blank = 0;
	int besttime = 100000;
	for (int i = 0; i < p_num; i++) {
		string filename = "low/" + to_string(i) + ".png";
		game_board[i] = Object::create(filename, scene1, pointX + (i % p_width) * radius, pointY - (i / p_width) * radius);
		init_board[i] = game_board[i];
		game_board[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
			int j;
			for (j = 0; j < p_num; j++) {
				if (game_board[j] == object) {
					break;
				}
			}
			if (checked(j, blank, p_width, p_num))
			{ //위

				game_board[j]->locate(scene1, pointX + (blank % p_width) * radius, pointY - (blank / p_width) * radius);
				game_board[blank]->locate(scene1, pointX + (j % p_width) * radius, pointY - (j / p_width) * radius);

				game_board[j] = game_board[blank];
				game_board[blank] = object;
				blank = j;

				int k;
				for (k = 0; k < p_num; k++) {
					if (game_board[k] != init_board[k]) break;
				}
				if (k == p_num) {
					endtime = clock();
					int time = (endtime - starttime) / CLOCKS_PER_SEC;
					if (besttime >= time) {
						besttime = time;
					}
					string buf = to_string(time) + "초 만에 완성!\n지금까지 당신의  난이도 하 퍼즐의 최단기록은  " + to_string(besttime) + "초 입니다!";
					showMessage(buf.c_str());
					RestartButton->locate(scene1, 400, 400);
					RestartButton->show();
					endButton->locate(scene1, 600, 400);
					endButton->show();
					home->locate(scene1, 800, 400);
					home->show();
					stage = 1;


				}
			}
			return true;
			});
	}
	game_board[blank]->hide();

	auto count = 0;
	auto timer = Timer::create(0.1f);
	timer->setOnTimerCallback([&](TimerPtr t)->bool {
		int j = 0;
		switch (rand() % 4) {
		case 0:
			j = blank - 1; break;
		case 1:
			j = blank + 1; break;
		case 2:
			j = blank - p_width; break;
		case 3:
			j = blank + p_width; break;
		}
		cout << count << endl;
		if (checked(j, blank, p_width, p_num))
		{
			game_board[j]->locate(scene1, pointX + (blank % p_width) * radius, pointY - (blank / p_width) * radius);
			game_board[blank]->locate(scene1, pointX + (j % p_width) * radius, pointY - (j / p_width) * radius);
			auto object = game_board[j];
			game_board[j] = game_board[blank];
			game_board[blank] = object;
			blank = j;
		}

		count++;
		if (count < shuffle_num) {
			t->set(0.05f);
			t->start();
		}
		else if (count == shuffle_num) {
			starttime = clock();
		}
		return true;
		});

	//난이도 중---------------------------------------
	const int m_num = 16;
	const int m_width = 4;
	const int m_pointX = 340;
	const int m_pointY = 510;
	const int m_radius = 150;
	ObjectPtr m_game_board[m_num];
	ObjectPtr m_init_board[m_num];
	int m_blank = 0;
	int m_besttime = 100000;
	for (int i = 0; i < m_num; i++) {
		string m_filename = "mid/" + to_string(i) + ".png";
		m_game_board[i] = Object::create(m_filename, scene2, m_pointX + (i % m_width) * m_radius, m_pointY - (i / m_width) * m_radius);
		m_init_board[i] = m_game_board[i];
		m_game_board[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
			int m;
			for (m = 0; m < m_num; m++) {
				if (m_game_board[m] == object) {
					break;
				}
			}
			if (checked(m, m_blank, m_width, m_num))
			{ //위

				m_game_board[m]->locate(scene2, m_pointX + (m_blank % m_width) * m_radius, m_pointY - (m_blank / m_width) * m_radius);
				m_game_board[m_blank]->locate(scene2, m_pointX + (m % m_width) * m_radius, m_pointY - (m / m_width) * m_radius);

				m_game_board[m] = m_game_board[m_blank];
				m_game_board[m_blank] = object;
				m_blank = m;

				int k;
				for (k = 0; k < m_num; k++) {
					if (m_game_board[k] != m_init_board[k]) break;
				}
				if (k == m_num) {
					m_endtime = clock();
					int m_time = (m_endtime - m_starttime) / CLOCKS_PER_SEC;
					if (m_besttime >= m_time) {
						m_besttime = m_time;
					}
					string buf = to_string(m_time) + "초 만에 완성!\n지금까지 당신의 난이도 중 퍼즐의 최단기록은  " + to_string(m_besttime) + "초 입니다!";
					showMessage(buf.c_str());
					RestartButton->locate(scene2, 400, 400);
					RestartButton->show();
					endButton->locate(scene2, 600, 400);
					endButton->show();
					home->locate(scene2, 800, 400);
					home->show();
					stage = 2;

				}
			}
			return true;
			});
	}
	m_game_board[m_blank]->hide();

	auto m_count = 0;
	auto m_timer = Timer::create(0.1f);
	m_timer->setOnTimerCallback([&](TimerPtr m_t)->bool {
		
		int m = 0;
		switch (rand() % 4) {
		case 0:
			m = m_blank - 1; break;
		case 1:
			m = m_blank + 1; break;
		case 2:
			m = m_blank - m_width; break;
		case 3:
			m = m_blank + m_width; break;
		}
		cout << m_count << endl;
		if (checked(m, m_blank, m_width, m_num))
		{
			m_game_board[m]->locate(scene2, m_pointX + (m_blank % m_width) * m_radius, m_pointY - (m_blank / m_width) * m_radius);
			m_game_board[m_blank]->locate(scene2, m_pointX + (m % m_width) * m_radius, m_pointY - (m / m_width) * m_radius);
			auto m_object = m_game_board[m];
			m_game_board[m] = m_game_board[m_blank];
			m_game_board[m_blank] = m_object;
			m_blank = m;
		}

		m_count++;
		if (m_count < shuffle_num) {
			m_t->set(0.05f);
			m_t->start();
		}
		else if (m_count == shuffle_num) {
			m_starttime = clock();
		}
		return true;
		});
	//난이도 상---------------------------------------
	const int h_num = 25;
	const int h_width = 5;
	const int h_pointX = 340;
	const int h_pointY = 540;
	const int h_radius = 120;
	ObjectPtr h_game_board[h_num];
	ObjectPtr h_init_board[h_num];
	int h_blank = 0;
	int h_besttime = 100000;
	for (int i = 0; i < h_num; i++) {
		string h_filename = "high/" + to_string(i) + ".png";
		h_game_board[i] = Object::create(h_filename, scene3, h_pointX + (i % h_width) * h_radius, h_pointY - (i / h_width) * h_radius);
		h_init_board[i] = h_game_board[i];
		h_game_board[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
			int h;
			for (h = 0; h < h_num; h++) {
				if (h_game_board[h] == object) {
					break;
				}
			}
			if (checked(h, h_blank, h_width, h_num))
			{ //위

				h_game_board[h]->locate(scene3, h_pointX + (h_blank % h_width) * h_radius, h_pointY - (h_blank / h_width) * h_radius);
				h_game_board[h_blank]->locate(scene3, h_pointX + (h % h_width) * h_radius, h_pointY - (h / h_width) * h_radius);

				h_game_board[h] = h_game_board[h_blank];
				h_game_board[h_blank] = object;
				h_blank = h;

				int k;
				for (k = 0; k < h_num; k++) {
					if (h_game_board[k] != h_init_board[k]) break;
				}
				if (k == h_num) {
					h_endtime = clock();
					int h_time = (h_endtime - h_starttime) / CLOCKS_PER_SEC;
					if (h_besttime >= h_time) {
						h_besttime = h_time;
					}
					string buf = to_string(h_time) + "초 만에 완성!\n지금까지 당신의 난이도 상 퍼즐의 최단기록은  " + to_string(h_besttime) + "초 입니다!";
					showMessage(buf.c_str());
					RestartButton->locate(scene3, 400, 400);
					RestartButton->show();
					endButton->locate(scene3, 600, 400);
					endButton->show();
					home->locate(scene3, 800, 400);
					home->show();
					stage = 3;

				}
			}
			return true;
			});
	}
	h_game_board[h_blank]->hide();

	auto h_count = 0;
	auto h_timer = Timer::create(0.1f);
	h_timer->setOnTimerCallback([&](TimerPtr h_t)->bool {

		int h = 0;
		switch (rand() % 4) {
		case 0:
			h = h_blank - 1; break;
		case 1:
			h = h_blank + 1; break;
		case 2:
			h = h_blank - h_width; break;
		case 3:
			h = h_blank + h_width; break;
		}
		cout << h_count << endl;
		if (checked(h, h_blank, h_width, h_num))
		{
			h_game_board[h]->locate(scene3, h_pointX + (h_blank % h_width) * h_radius, h_pointY - (h_blank / h_width) * h_radius);
			h_game_board[h_blank]->locate(scene3, h_pointX + (h % h_width) * h_radius, h_pointY - (h / h_width) * h_radius);
			auto h_object = h_game_board[h];
			h_game_board[h] = h_game_board[h_blank];
			h_game_board[h_blank] = h_object;
			h_blank = h;
		}

		h_count++;
		if (h_count < shuffle_num) {
			h_t->set(0.05f);
			h_t->start();
		}
		else if (h_count == shuffle_num) {
			h_starttime = clock();
		}
		return true;
		});
	//-----------------------------------------------------------------------------------
	RestartButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		RestartButton->hide();
		home->hide();
		endButton->hide();
		if (stage == 1) {
			scene1->enter();
			count = 0;
			timer->start();
			starttime = clock();
		}
		if (stage == 2) {
			scene2->enter();
			m_count = 0;
			m_timer->start();
			m_starttime = clock();
		}
		if (stage == 3) {
			scene3->enter();
			h_count = 0;
			h_timer->start();
			h_starttime = clock();
		}
		stage = 0;
		

		return 0;
		});
	
	endButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		endGame();
		return 0;
		});

	home->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		RestartButton->hide();
		home->hide();
		endButton->hide();
		scene->enter();
		stage = 0;
		
		return 0;
		});

	low->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		endButton->hide();
		scene1->enter();
		count = 0;
		starttime = clock();
		timer->start();
		return 0;
		});
	mid->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		endButton->hide();
		scene2->enter();
		m_count = 0;
		m_starttime = clock();
		m_timer->start();
		return 0;
		});
	high->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		endButton->hide();
		scene3->enter();
		h_count = 0;
		h_starttime = clock();
		h_timer->start();
		return 0;
		});
	startGame(scene);
	return 0;
}