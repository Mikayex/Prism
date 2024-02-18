#[derive(serde::Deserialize, serde::Serialize)]
#[serde(default)]
pub struct PrismApp {
    label: String,
    value: f32,
}

impl Default for PrismApp {
    fn default() -> Self {
        Self {
            // Example stuff:
            label: "Hello World!".to_owned(),
            value: 2.7,
        }
    }
}

impl PrismApp {
    pub fn new(cc: &eframe::CreationContext<'_>) -> Self {
        // This is also where you can customize the look and feel of egui using
        // `cc.egui_ctx.set_visuals` and `cc.egui_ctx.set_fonts`.

        // Load previous app state (if any).
        // Note that you must enable the `persistence` feature for this to work.
        if let Some(storage) = cc.storage {
            return eframe::get_value(storage, eframe::APP_KEY).unwrap_or_default();
        }

        Default::default()
    }
}

impl eframe::App for PrismApp {
    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        // Put your widgets into a `SidePanel`, `TopBottomPanel`, `CentralPanel`, `Window` or `Area`.
        // For inspiration and more examples, go to https://emilk.github.io/egui

        egui::TopBottomPanel::top("top_panel").show(ctx, |ui| {
            // The top panel is often a good place for a menu bar:

            egui::menu::bar(ui, |ui| {
                // NOTE: no File->Quit on web pages!
                let is_web = cfg!(target_arch = "wasm32");
                if !is_web {
                    ui.menu_button("File", |ui| {
                        if ui.button("Quit").clicked() {
                            ctx.send_viewport_cmd(egui::ViewportCommand::Close);
                        }
                    });
                    ui.add_space(16.0);
                }

                egui::widgets::global_dark_light_mode_buttons(ui);
            });
        });

        egui::CentralPanel::default().show(ctx, |ui| {
            // The central panel the region left after adding TopPanel's and SidePanel's
            ui.heading("eframe template");

            ui.horizontal(|ui| {
                ui.label("Write something: ");
                ui.text_edit_singleline(&mut self.label);
            });

            ui.add(egui::Slider::new(&mut self.value, 0.0..=10.0).text("value"));
            if ui.button("Increment").clicked() {
                self.value += 1.0;
            }

            ui.separator();

            if let Some(render_state) = frame.wgpu_render_state() {
                let wgpu_adapter_details_ui = |ui: &mut egui::Ui, adapter: &wgpu::Adapter| {
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
                };

                let wgpu_adapter_ui = |ui: &mut egui::Ui, adapter: &eframe::wgpu::Adapter| {
                    let info = &adapter.get_info();
                    ui.label(format!("{:?}", info.backend)).on_hover_ui(|ui| {
                        wgpu_adapter_details_ui(ui, adapter);
                    });
                };

                egui::Grid::new("wgpu_info").num_columns(2).show(ui, |ui| {
                    ui.label("Renderer:");
                    ui.hyperlink_to("wgpu", "https://wgpu.rs/");
                    ui.end_row();

                    ui.label("Backend:");
                    wgpu_adapter_ui(ui, &render_state.adapter);
                    ui.end_row();

                    #[cfg(not(target_arch = "wasm32"))]
                    if render_state.available_adapters.len() > 1 {
                        ui.label("Others:");
                        ui.vertical(|ui| {
                            for adapter in &*render_state.available_adapters {
                                if adapter.get_info() != render_state.adapter.get_info() {
                                    wgpu_adapter_ui(ui, adapter);
                                }
                            }
                        });
                        ui.end_row();
                    }
                });
            }

            ui.with_layout(egui::Layout::bottom_up(egui::Align::LEFT), |ui| {
                egui::warn_if_debug_build(ui);
            });
        });
    }

    fn save(&mut self, storage: &mut dyn eframe::Storage) {
        eframe::set_value(storage, eframe::APP_KEY, self);
    }
}
