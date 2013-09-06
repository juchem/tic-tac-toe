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

  sf::Image X, O;
  if (!X.LoadFromFile("x.png") || !O.LoadFromFile("o.png")) { return -1; }

  sf::Music bgm;
  if (bgm.OpenFromFile("bgm.ogg")) {
    bgm.SetLoop(true);
    bgm.Play();
  }

  tic_tac_toe game;

  while (app.IsOpened()) {
    for (sf::Event e; app.GetEvent(e); ) {
      if ((e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)
          || e.Type == sf::Event::Closed
      ) {
        app.Close();
      } else if (e.Type == sf::Event::MouseButtonPressed) {
        game.move(e.MouseButton.Y / slot, e.MouseButton.X / slot);
      }
    }

    app.Clear();
    for (unsigned y = 0; y < 3; ++y) {
      for (unsigned x = 0; x < 3; ++x) {
        if (auto which = game.get(y, x)) {
          sf::Sprite sprite;
          sprite.SetImage((which == tic_tac_toe::cross) ? X : O);
          sprite.SetPosition(x * slot, y * slot);
          app.Draw(sprite);
        }
      }
    }
    app.Display();
  }

  if (bgm.GetStatus() == sf::Sound::Playing) { bgm.Stop(); }
  return 0;
}
