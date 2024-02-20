pub(super) fn draw(app: &mut crate::PrismApp, ctx: &egui::Context, frame: &eframe::Frame) {
    egui::Window::new("About")
        .resizable(false)
        .collapsible(false)
        .anchor(egui::Align2::CENTER_CENTER, egui::Vec2::ZERO)
        .open(&mut app.about_window_open)
        .show(ctx, |ui| window_ui(ui, frame));
}

fn window_ui(ui: &mut egui::Ui, frame: &eframe::Frame) {
    if let Some(render_state) = frame.wgpu_render_state() {
        ui.horizontal(|ui| {
            ui.label("Renderer:");
            ui.label("wgpu");
        });
        ui.horizontal(|ui| {
            ui.label("Backend:");
            wgpu_adapter_ui(ui, &render_state.adapter);
        });

        #[cfg(not(target_arch = "wasm32"))]
        if render_state.available_adapters.len() > 1 {
            ui.horizontal_top(|ui| {
                ui.label("Other Backends:");
                ui.vertical(|ui| {
                    for adapter in &*render_state.available_adapters {
                        if adapter.get_info() != render_state.adapter.get_info() {
                            wgpu_adapter_ui(ui, adapter);
                        }
                    }
                });
            });
        }
    }
}

fn wgpu_adapter_ui(ui: &mut egui::Ui, adapter: &wgpu::Adapter) {
    let info = &adapter.get_info();
    ui.label(format!("{:?}", info.backend)).on_hover_ui(|ui| {
        wgpu_adapter_details_ui(ui, adapter);
    });
}

fn wgpu_adapter_details_ui(ui: &mut egui::Ui, adapter: &wgpu::Adapter) {
    let info = &adapter.get_info();

    let wgpu::AdapterInfo {
        name,
        vendor,
        device,
        device_type,
        driver,
        driver_info,
        backend,
    } = &info;

    // Example values:
    // > name: "llvmpipe (LLVM 16.0.6, 256 bits)", device_type: Cpu, backend: Vulkan, driver: "llvmpipe", driver_info: "Mesa 23.1.6-arch1.4 (LLVM 16.0.6)"
    // > name: "Apple M1 Pro", device_type: IntegratedGpu, backend: Metal, driver: "", driver_info: ""
    // > name: "ANGLE (Apple, Apple M1 Pro, OpenGL 4.1)", device_type: IntegratedGpu, backend: Gl, driver: "", driver_info: ""

    egui::Grid::new("adapter_info").show(ui, |ui| {
        ui.label("Backend:");
        ui.label(format!("{backend:?}"));
        ui.end_row();

        ui.label("Device Type:");
        ui.label(format!("{device_type:?}"));
        ui.end_row();

        if !name.is_empty() {
            ui.label("Name:");
            ui.label(format!("{name:?}"));
            ui.end_row();
        }
        if !driver.is_empty() {
            ui.label("Driver:");
            ui.label(format!("{driver:?}"));
            ui.end_row();
        }
        if !driver_info.is_empty() {
            ui.label("Driver info:");
            ui.label(format!("{driver_info:?}"));
            ui.end_row();
        }
        if *vendor != 0 {
            ui.label("Vendor:");
            ui.label(format!("0x{vendor:04X}"));
            ui.end_row();
        }
        if *device != 0 {
            ui.label("Device:");
            ui.label(format!("0x{device:02X}"));
            ui.end_row();
        }
    });
}
