#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

template <uint16_t...> struct victory{
  static uint32_t check(uint32_t, unsigned) { return 0; }
};

template <uint16_t Pattern, uint16_t... R> struct victory<Pattern, R...> {
  static uint32_t check(uint32_t where, unsigned mark) {
    uint32_t mask = 0;
    for (uint16_t p = Pattern, shift = 0; p; p >>= 1, shift += 2) {
      if (p & 1) { mask |= (mark << shift); }
    }
    return ((where & mask) == mask) ? mask : victory<R...>::check(where, mark);
  }
};

struct tic_tac_toe {
  enum mark { nough = 0b01, cross = 0b10 };

  typedef victory<
    0b111000000, 0b000111000, 0b000000111, 0b100100100,
    0b010010010, 0b001001001, 0b100010001, 0b001010100
    > checker;

  tic_tac_toe(): grid_(0), turn_(0), moves_(0) {}

  void move(unsigned y, unsigned x) {
    if (!turn_) { grid_ = 0; turn_ = cross; moves_ = 0; }
    if (get(y, x)) { return; }

    grid_ |= (turn_ << ((y * 3 + x) * 2));
    ++moves_;

    if (auto winner = checker::check(grid_, turn_)) {
      grid_ = winner;
      turn_ = 0;
    } else if (moves_ == 9) {
      turn_ = 0;
    } else {
      turn_ = (~turn_) & 0b11;
    }
  }

  unsigned get(unsigned y, unsigned x) const {
    return (grid_ >> ((y * 3 + x) * 2)) & 0b11;
  }

private:
  uint32_t grid_;
  unsigned turn_;
  unsigned moves_;
};

int main() {
  auto const slot = 100;
  sf::RenderWindow app(
    sf::VideoMode(slot * 3, slot * 3),
    "100 SLOC C++ Tic-Tac-Toe"
  );

  sf::Texture X, O;
  if (!X.loadFromFile("x.png") || !O.loadFromFile("o.png")) { return -1; }

  sf::Music bgm;
  if (bgm.openFromFile("bgm.ogg")) {
    bgm.setLoop(true);
    bgm.play();
  }

  tic_tac_toe game;
  while (app.isOpen()) {
    for (sf::Event e; app.pollEvent(e); ) {
      if ((e.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          || e.type == sf::Event::Closed
      ) {
        app.close();
      } else if (e.type == sf::Event::MouseButtonPressed) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(app);
        game.move(mousePosition.y / slot, mousePosition.x / slot);
      }
    }

    app.clear();
    for (unsigned y = 0; y < 3; ++y) {
      for (unsigned x = 0; x < 3; ++x) {
        if (auto which = game.get(y, x)) {
          sf::Sprite sprite;
          sprite.setTexture((which == tic_tac_toe::cross) ? X : O);
          sprite.setPosition(x * slot, y * slot);
          app.draw(sprite);
        }
      }
    }
    app.display();
  }

  if (bgm.getStatus() == sf::Sound::Playing) { bgm.stop(); }
  return 0;
}
