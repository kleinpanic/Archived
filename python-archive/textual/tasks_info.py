# tasks_info.py

from textual.widget import Widget
from textual.reactive import Reactive


class TasksInfo(Widget):
    """Widget to display tasks."""

    data: Reactive[str] = Reactive("No tasks available.")

    def on_mount(self) -> None:
        self.set_interval(60, self.refresh_data)
        self.refresh_data()

    def render(self) -> str:
        return self.data

    def refresh_data(self) -> None:
        # Placeholder for future integration with task manager
        self.data = "No tasks available."
