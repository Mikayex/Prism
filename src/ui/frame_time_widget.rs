use egui::util::History;

#[derive(Clone, Default, serde::Deserialize, serde::Serialize)]
enum FrameTimeMode {
    #[default]
    FrameTime,
    Fps,
}

impl FrameTimeMode {
    pub fn load(ctx: &egui::Context, id: egui::Id) -> Option<Self> {
        ctx.data_mut(|d| d.get_persisted(id))
    }

    pub fn store(self, ctx: &egui::Context, id: egui::Id) {
        ctx.data_mut(|d| d.insert_persisted(id, self));
    }
}

pub(super) struct FrameTimeWidget<'a> {
    id: egui::Id,
    frame_times: &'a History<f32>,
}

impl<'a> FrameTimeWidget<'a> {
    pub fn new(frame_times: &'a History<f32>, id: egui::Id) -> Self {
        Self { id, frame_times }
    }

    fn frame_time(&self) -> f32 {
        self.frame_times.average().unwrap_or(0.0)
    }

    fn fps(&self) -> f32 {
        match self.frame_times.average() {
            None => 0.0,
            Some(time) => 1.0 / time,
        }
    }
}

impl<'a> egui::Widget for FrameTimeWidget<'a> {
    fn ui(self, ui: &mut egui::Ui) -> egui::Response {
        let mut state = FrameTimeMode::load(ui.ctx(), self.id).unwrap_or_default();

        let text = match state {
            FrameTimeMode::FrameTime => format!("{:.2} ms", self.frame_time() * 1e3),
            FrameTimeMode::Fps => format!("{:.1} FPS", self.fps()),
        };
        let label = egui::Label::new(egui::RichText::new(text).monospace())
            .sense(egui::Sense::click())
            .selectable(false);

        let response = ui.add(label);
        if response.clicked() {
            state = match state {
                FrameTimeMode::FrameTime => FrameTimeMode::Fps,
                FrameTimeMode::Fps => FrameTimeMode::FrameTime,
            }
        }

        state.store(ui.ctx(), self.id);
        response
    }
}
