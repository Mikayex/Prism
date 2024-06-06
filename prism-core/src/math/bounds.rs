#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct Bounds<T: nalgebra::Scalar, const D: usize> {
    pub min: nalgebra::Point<T, D>,
    pub max: nalgebra::Point<T, D>,
}

pub type Bounds2f = Bounds<f32, 2>;
pub type Bounds2i = Bounds<i32, 2>;

pub type Bounds3f = Bounds<f32, 3>;
pub type Bounds3i = Bounds<i32, 3>;

impl<T: nalgebra::Scalar + nalgebra::SimdPartialOrd, const D: usize> Bounds<T, D> {
    pub fn new(p1: &nalgebra::Point<T, D>, p2: &nalgebra::Point<T, D>) -> Self {
        let (min, max) = p1.inf_sup(p2);
        Self { min, max }
    }
}

impl<T: nalgebra::Scalar + PartialOrd, const D: usize> Bounds<T, D> {
    pub fn is_empty(&self) -> bool {
        self.min
            .iter()
            .zip(self.max.iter())
            .any(|(min, max)| min >= max)
    }
}

impl<T: nalgebra::Scalar + num_traits::Bounded> Default for Bounds<T, 2> {
    fn default() -> Self {
        Self {
            min: nalgebra::Point2::new(T::min_value(), T::min_value()),
            max: nalgebra::Point2::new(T::max_value(), T::max_value()),
        }
    }
}

impl<T: nalgebra::Scalar + num_traits::Bounded> Default for Bounds<T, 3> {
    fn default() -> Self {
        Self {
            min: nalgebra::Point3::new(T::min_value(), T::min_value(), T::min_value()),
            max: nalgebra::Point3::new(T::max_value(), T::max_value(), T::max_value()),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{Point2f, Point2i, Point3f, Point3i};

    #[test]
    fn is_empty() {
        assert!(!Bounds2i::default().is_empty());
        let bounds = Bounds2i::new(&Point2i::default(), &Point2i::default());
        assert!(bounds.is_empty());

        assert!(!Bounds2f::default().is_empty());
        let bounds = Bounds2f::new(&Point2f::default(), &Point2f::default());
        assert!(bounds.is_empty());

        assert!(!Bounds3i::default().is_empty());
        let bounds = Bounds3i::new(&Point3i::default(), &Point3i::default());
        assert!(bounds.is_empty());

        assert!(!Bounds2f::default().is_empty());
        let bounds = Bounds3f::new(&Point3f::default(), &Point3f::default());
        assert!(bounds.is_empty());
    }
}
