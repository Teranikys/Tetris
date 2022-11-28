#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>


struct Point {
	int x, y;
}a[4], b[4];


// playing field properties
const int M = 15;
const int N = 9;
int field[M][N] = {0};


//set of figures
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

// collision check
bool collision_check() {
	for (int i = 0; i < 4; ++i) {
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
			return false;
		else if (field[a[i].y][a[i].x])
			return false;
	}
	return true;
}

int main() {
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(502, 544), "Tetris");


	// textures setup
	sf::Texture t1, t2, t3;
	t1.loadFromFile("images/cubes.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");
	sf::Sprite s(t1), background(t2), frame(t3); sf::Text text;

	// score properties setup
	sf::Font font;
	font.loadFromFile("Signature.otf");
	text.setFont(font);
	text.setString("0");
	text.setCharacterSize(54);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	text.move(330, 0);

	//main fields
	int dx = 0;
	bool rotate = false;
	int colorNum = 1;
	bool beginGame = true;
	int n = rand() % 7;
	float timer = 0;
	float delay = 0.3;
	int score = 0;
	sf::Clock clock;

	//main loop
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		if (beginGame) {
			beginGame = false;
			n = rand() % 7;
			for (int i = 0; i < 4; ++i) {
				a[i].x = figures[n][i] % 2 + 4;
				a[i].y = figures[n][i] / 2;
			}
		}

		//keyboard actions proccessing
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();

			if (e.type == sf::Event::KeyPressed){
				if (e.key.code == sf::Keyboard::Up)
					rotate = true;
				else if (e.key.code == sf::Keyboard::Left)
					dx = -1;
				else if (e.key.code == sf::Keyboard::Right)
					dx = 1;
				}
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			delay = 0.05;
		}

		//right-left move proccessing
		for (int i = 0; i < 4; i++) { 
			b[i] = a[i]; a[i].x += dx; 
		}

		//fall proccessing
		if (!collision_check()) {
			for (int i = 0; i < 4; i++) {
				a[i] = b[i];
			}
		}

		//tetramino rotation proccessing
		if (rotate)
		{
			Point p = a[1]; //center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!collision_check()) for (int i = 0; i < 4; i++) a[i] = b[i];
		}

		//next tetramino proccessing
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { 
				b[i] = a[i]; 
				a[i].y += 1; 
			}

			if (!collision_check())
			{
				for (int i = 0; i < 4; i++) {
					field[b[i].y][b[i].x] = colorNum;
				}

				colorNum =  1 + rand() % 6;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2 + 3;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		//check for completed layers
		int k = M - 1;
		int score_cnt = 0;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) {
				k--;
			} else {
				score_cnt++;
			}
		}
		//score proccessing
		switch (score_cnt)
		{
		case 1:
			score++;
			break;
		case 2:
			score += 3;

			break;
		case 3:
			score += 5;
			break;
		case 4:
			score += 10;
			break;
		default:
			break;
		}
		text.setString(std::to_string(score));

		//return to default fields values
		dx = 0;
		rotate = false;
		delay = 0.5;

		//graphics
		window.draw(background);

		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < N; ++j) {
				if (field[i][j] == 0) {
					continue;
				}
				s.setTextureRect(sf::IntRect(field[i][j] * 35 - 1, 0, 35, 35));
				s.setPosition(j * 30 , i * 32);
				s.move(14, 33);
				window.draw(s);
			}
		}
		for (int i = 0; i < 4; ++i) {
			s.setTextureRect(sf::IntRect(colorNum * 35 - 1, 0, 35, 35));
			s.setPosition(a[i].x * 30, a[i].y * 32);
			s.move(14, 33);
			window.draw(s);
		}

		window.draw(text);
		window.draw(frame);
		window.display();
		
	}

	return 0;
}