use crate::{Point3f, Vector3f};

#[derive(Debug, Clone, PartialEq, Default)]
pub struct Ray {
    pub origin: Point3f,
    pub direction: Vector3f,
}

impl Ray {
    pub fn new(origin: Point3f, direction: Vector3f) -> Self {
        Self { origin, direction }
    }

    pub fn evaluate(&self, t: f32) -> Point3f {
        self.origin + self.direction * t
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn evaluate() {
        let ray = Ray::new(Point3f::origin(), Vector3f::new(0f32, 1f32, 0f32));
        assert_eq!(ray.origin, ray.evaluate(0f32));
        assert_eq!(ray.direction, ray.evaluate(1f32).coords);

        let ray = Ray::new(
            Point3f::new(1f32, 2f32, 3f32),
            Vector3f::new(0f32, 1f32, 0f32),
        );
        assert_eq!(ray.origin, ray.evaluate(0f32));
        assert_eq!(Point3f::new(1f32, 3.5, 3f32), ray.evaluate(1.5));
    }
}
