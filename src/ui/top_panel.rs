use crate::ui::frame_time_widget::FrameTimeWidget;

pub(super) fn draw(app: &mut crate::PrismApp, ctx: &egui::Context) {
    egui::TopBottomPanel::top("top_panel").show(ctx, |ui| {
        ui.set_enabled(!app.modal_window_open());
        menu_bar(app, ctx, ui);
    });
}

fn menu_bar(app: &mut crate::PrismApp, ctx: &egui::Context, ui: &mut egui::Ui) {
    egui::menu::bar(ui, |ui| {
        let is_web = cfg!(target_arch = "wasm32");
        if !is_web {
            file_menu(ctx, ui);
        }
        help_menu(app, ui);

        ui.with_layout(egui::Layout::right_to_left(egui::Align::Center), |ui| {
            egui::warn_if_debug_build(ui);
            ui.add(FrameTimeWidget::new(
                &app.frame_times,
                egui::Id::new("frame_time"),
            ));
        });
    });
}

fn file_menu(ctx: &egui::Context, ui: &mut egui::Ui) {
    ui.menu_button("File", |ui| {
        if ui.button("Quit").clicked() {
            ctx.send_viewport_cmd(egui::ViewportCommand::Close);
        }
    });
}

fn help_menu(app: &mut crate::PrismApp, ui: &mut egui::Ui) {
    ui.menu_button("Help", |ui| {
        if ui.button("About").clicked() {
            app.about_window_open = true;
            ui.close_menu();
        }
    });
}
