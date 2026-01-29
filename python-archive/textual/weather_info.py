# weather_info.py

from textual.widget import Widget
from textual.reactive import Reactive
import requests


class WeatherInfo(Widget):
    """Widget to display weather information."""

    data: Reactive[str] = Reactive("Loading weather info...")

    def on_mount(self) -> None:
        self.set_interval(600, self.refresh_data)
        self.refresh_data()

    def render(self) -> str:
        return self.data

    def refresh_data(self) -> None:
        location = self.get_location()
        if location:
            weather = self.get_weather(location)
            self.data = weather
        else:
            self.data = "Unable to get location."

    def get_location(self):
        try:
            res = requests.get('http://ip-api.com/json/')
            data = res.json()
            city = data.get('city')
            return city
        except:
            return None

    def get_weather(self, location):
        try:
            url = f'http://wttr.in/{location}?format=3'
            res = requests.get(url)
            if res.status_code == 200:
                return res.text.strip()
            else:
                return "Unable to fetch weather data."
        except:
            return "Unable to fetch weather data."
