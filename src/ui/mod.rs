mod about_window;
mod central_panel;
mod top_panel;

pub(super) fn draw(app: &mut crate::PrismApp, ctx: &egui::Context, frame: &eframe::Frame) {
    top_panel::draw(app, ctx);
    about_window::draw(app, ctx, frame);
    central_panel::draw(app, ctx);
}
