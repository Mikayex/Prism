use prism_core::{ImageU8, Point2u};

fn main() {
    println!("Hello, world!");
    let image = ImageU8::new(Point2u::new(800, 600), 3);

    let output = image::RgbImage::from_raw(
        image.resolution().x,
        image.resolution().y,
        image.data().into(),
    )
    .unwrap();

    output.save("output.png").unwrap()
}
