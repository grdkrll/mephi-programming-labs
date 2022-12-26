#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

/*
 * В данном задании необходимо реализовать несколько классов, моделирующих
 * некоторые свойства определенных геометрических фигур.
 * - IShape2D -- базовый интерфейс, который должны реализовать все классы фигур.
 * Содержит объявления методов GetPerimeter() и GetArea()
 * - Rectangle -- прямоугольник. Принимает в конструкторе длину и ширину. Кроме
 * методов, объявленных в IShape2D, также имеет метод для вычисления радиуса
 * описанной окружности GetCircumscribedCircleRadius()
 * - Square -- квадрат. Принимает в конструкторе длину стороны. Кроме методов,
 * объявленных в IShape2D, также имеет метод для вычисления радиуса описанной
 * окружности GetCircumscribedCircleRadius()
 * - Circle -- окружность. Принимает в конструкторе радиус.
 * Также нужно реализовать функцию PrintProperties(std::stringstream& ss, const
 * IShape2D* shape), которая для данной фигуры формирует запись в лог, пишущийся
 * в поток ss. В лог для данной фигуры должны записаться периметр и площадь, а
 * также радиус описанной окружности, если он определен для неё.
 */

struct IShape2D {
  virtual ~IShape2D() = default;
  [[maybe_unused]] [[nodiscard]] virtual double GetPerimeter() const = 0;

  [[maybe_unused]] [[nodiscard]] virtual double GetArea() const = 0;
};

class Rectangle : public IShape2D {
public:
  Rectangle(double length, double width) : Length_(length), Width_(width) {}
  [[nodiscard]] double GetPerimeter() const override {
    return 2 * (Length_ + Width_);
  }
  [[nodiscard]] double GetArea() const override { return Length_ * Width_; }
  [[nodiscard]] double GetCircumscribedCircleRadius() const {
    return sqrt(Length_ * Length_ + Width_ * Width_) / 2;
  }

private:
  double Length_;
  double Width_;
};

class Square : public Rectangle {
public:
  explicit Square(double Length_) : Rectangle(Length_, Length_) {}
};

class Circle : public IShape2D {
public:
  explicit Circle(double r) : R_(r) {}

  [[nodiscard]] double GetPerimeter() const override { return 2 * PI_ * R_; }
  [[nodiscard]] double GetArea() const override { return PI_ * R_ * R_; }

private:
  const double PI_ = acos(-1);
  double R_;
};

void PrintProperties(std::stringstream &ss, const IShape2D *shape) {
  ss << "A shape with P = " << shape->GetPerimeter()
     << "; S = " << shape->GetArea();
  auto rect = dynamic_cast<const Rectangle *>(shape);
  if (rect) {
    ss << "; R = " << rect->GetCircumscribedCircleRadius();
  }
  ss << '\n';
}

int main() {
  // Код, помогающий в отладке
  std::vector<std::unique_ptr<IShape2D>> shapes;

  shapes.push_back(std::make_unique<Circle>(10));
  shapes.push_back(std::make_unique<Circle>(20));
  shapes.push_back(std::make_unique<Rectangle>(5, 10));
  shapes.push_back(std::make_unique<Rectangle>(1, 10));
  shapes.push_back(std::make_unique<Square>(5));

  std::stringstream ss;
  for (auto &shape : shapes) {
    PrintProperties(ss, shape.get());
  }

  std::cout << ss.str();
  std::string expectedOutput = R"(A shape with P = 62.8319; S = 314.159
A shape with P = 125.664; S = 1256.64
A shape with P = 30; S = 50; R = 5.59017
A shape with P = 22; S = 10; R = 5.02494
A shape with P = 20; S = 25; R = 3.53553
)";
  // Если проверка не проходит из-за точности чисел с точкой, то нестрашно
  assert(ss.str() == expectedOutput);
}
