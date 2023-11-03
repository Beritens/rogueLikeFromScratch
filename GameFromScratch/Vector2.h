#pragma once
struct Vector2 {
	float x;
	float y;
    // Define the addition operator
    Vector2 operator+(const Vector2 other) const {
        return { x + other.x, y + other.y };
    }

    // Define the subtraction operator
    Vector2 operator-(const Vector2 other) const {
        return { x - other.x, y - other.y };
    }

    // Define the multiplication operator
    Vector2 operator*(const Vector2 other) const {
        return { x * other.x, y * other.y };
    }

    // Define the multiplication operator
    Vector2 operator*(const float s) const {
        return { x * s, y * s};
    }


    // Define the division operator
    Vector2 operator/(const Vector2 other) const {
        return { x / other.x, y / other.y };
    }
};

float vLength(Vector2 v);
float vSqrLength(Vector2 v);
