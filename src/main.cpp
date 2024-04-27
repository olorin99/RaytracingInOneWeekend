
#include <Ende/io.h>
#include <Ende/math/Vec.h>
#include <format>
#include <iostream>

void writeColour(ende::fs::File& outFile, const ende::math::Vec3f& colour) {
    outFile.write(std::format("{} {} {}\n",
                              static_cast<i32>(255.999 * colour.x()),
                              static_cast<i32>(255.999 * colour.y()),
                              static_cast<i32>(255.999 * colour.z())));
}

class Ray {
public:

    Ray() = default;
    Ray(const ende::math::Vec3f& origin, const ende::math::Vec3f& direction)
        : _origin(origin),
        _direction(direction)
    {}

    auto origin() const -> const ende::math::Vec3f& { return _origin; }
    auto direction() const -> const ende::math::Vec3f& { return _direction; }

    auto at(f32 t) const -> ende::math::Vec3f {
        return origin() + (direction() * t);
    }

private:
    ende::math::Vec3f _origin = { 0, 0, 0 };
    ende::math::Vec3f _direction = { 0, 0, 1 };
};

auto computeColour(const Ray& ray) -> ende::math::Vec3f {
    auto unitDirection = ray.direction().unit();
    auto t = 0.5f * (unitDirection.y() + 1.0f);
    return ende::math::Vec3f{ 1.0, 1.0, 1.0 }.lerp({0.5, 0.7, 1.0}, t);
}

int main() {

    const auto aspectRatio = 16.f / 9.f;
    const i32 imageWidth = 400;
    const i32 imageHeight = static_cast<i32>(imageWidth / aspectRatio);

    auto viewPortHeight = 2.0f;
    auto viewPortWidth = aspectRatio * viewPortHeight;
    auto focalLength = 1.0f;

    auto origin = ende::math::Vec3f{ 0, 0, 0 };
    auto horizontal = ende::math::Vec3f{ viewPortWidth, 0, 0 };
    auto vertical = ende::math::Vec3f{ 0, viewPortHeight, 0 };
    auto lowerLeftCorner = origin - horizontal / 2.f - vertical / 2.f - ende::math::Vec3f{ 0, 0, focalLength };

    auto outFile = ende::fs::File::open("image.ppm", ende::fs::out).value();

    outFile.write(std::format("P3\n{} {}\n255\n", imageWidth, imageHeight));

    for (i32 j = imageHeight - 1; j >= 0; --j) {
        ende::stdout().write(std::format("\rScanlines remaining: {} ", j));
        std::cerr << std::flush;
        for (i32 i = 0; i < imageWidth; ++i) {
            auto u = f32(i) / (imageWidth - 1);
            auto v = f32(j) / (imageHeight - 1);

            auto ray = Ray(origin, lowerLeftCorner + horizontal * u + vertical * v - origin);
            auto colour = computeColour(ray);
            writeColour(outFile, colour);
        }
    }

    ende::stdout().write("\nDone\n");

    return 0;
}