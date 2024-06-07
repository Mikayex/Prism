use crate::Point2u;
use half::f16;
use num_traits::Zero;

#[derive(Clone, Debug)]
pub enum DynamicImage {
    UInt8(ImageU8),
    Float16(ImageF16),
    Float32(ImageF32),
}

macro_rules! forward_method(
        ($dynimage: expr, $image:pat_param, $action: expr) => (
            match $dynimage {
                DynamicImage::UInt8($image) => $action,
                DynamicImage::Float16($image) => $action,
                DynamicImage::Float32($image) => $action,
            }
        );
);

impl DynamicImage {
    pub fn new_u8(resolution: Point2u, channels: usize) -> Self {
        DynamicImage::UInt8(ImageU8::new(resolution, channels))
    }

    pub fn new_f16(resolution: Point2u, channels: usize) -> Self {
        DynamicImage::Float16(ImageF16::new(resolution, channels))
    }

    pub fn new_f32(resolution: Point2u, channels: usize) -> Self {
        DynamicImage::Float32(ImageF32::new(resolution, channels))
    }

    pub fn resolution(&self) -> Point2u {
        forward_method!(self, ref image, image.resolution())
    }
}

#[derive(Clone, Debug)]
pub struct Image<T> {
    data: Vec<T>,
    resolution: Point2u,
    channels: usize,
}

pub type ImageU8 = Image<u8>;
pub type ImageF16 = Image<f16>;
pub type ImageF32 = Image<f32>;

impl<T: Zero + Clone> Image<T> {
    pub fn new(resolution: Point2u, channels: usize) -> Self {
        Self {
            data: vec![T::zero(); Self::container_len(resolution, channels)],
            resolution,
            channels,
        }
    }

    pub fn resolution(&self) -> Point2u {
        self.resolution
    }

    pub fn channels(&self) -> usize {
        self.channels
    }

    pub fn data(&self) -> &[T] {
        self.data.as_slice()
    }

    pub fn data_mut(&mut self) -> &mut [T] {
        self.data.as_mut_slice()
    }

    fn container_len(resolution: Point2u, channels: usize) -> usize {
        resolution.x as usize * resolution.y as usize * channels
    }
}
