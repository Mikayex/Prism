use crate::ui;

#[derive(serde::Deserialize, serde::Serialize)]
#[serde(default)]
pub struct PrismApp {
    pub(crate) about_window_open: bool,
    pub(crate) label: String,
    pub(crate) value: f32,

    #[serde(skip)]
    pub(crate) frame_times: egui::util::History<f32>,
}

impl Default for PrismApp {
    fn default() -> Self {
        Self {
            about_window_open: false,
            label: "Hello World!".to_owned(),
            value: 2.7,
            frame_times: egui::util::History::new(0..300, 1.0),
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

    fn update_frame_time(&mut self, now: f64, previous_frame_time: Option<f32>) {
        let previous_frame_time = previous_frame_time.unwrap_or_default();
        if let Some(latest) = self.frame_times.latest_mut() {
            *latest = previous_frame_time; // rewrite history now that we know
        }
        self.frame_times.add(now, previous_frame_time); // projected
    }
}

impl eframe::App for PrismApp {
    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        self.update_frame_time(ctx.input(|i| i.time), frame.info().cpu_usage);
        ui::draw(self, ctx, frame);
    }

    fn save(&mut self, storage: &mut dyn eframe::Storage) {
        eframe::set_value(storage, eframe::APP_KEY, self);
    }
}
