use crate::ui;

#[derive(serde::Deserialize, serde::Serialize)]
#[serde(default)]
pub struct PrismApp {
    pub(crate) about_window_open: bool,
    pub(crate) label: String,
    pub(crate) value: f32,
}

impl Default for PrismApp {
    fn default() -> Self {
        Self {
            about_window_open: false,
            label: "Hello World!".to_owned(),
            value: 2.7,
        }
    }
}

impl PrismApp {
    pub fn new(cc: &eframe::CreationContext<'_>) -> Self {
        if let Some(storage) = cc.storage {
            return eframe::get_value(storage, eframe::APP_KEY).unwrap_or_default();
        }

        Default::default()
    }

    pub(crate) fn modal_window_open(&self) -> bool {
        self.about_window_open
    }
}

impl eframe::App for PrismApp {
    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        ui::draw(self, ctx, frame);
    }

    fn save(&mut self, storage: &mut dyn eframe::Storage) {
        eframe::set_value(storage, eframe::APP_KEY, self);
    }
}
