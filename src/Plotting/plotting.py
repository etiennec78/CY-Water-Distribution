import numpy as np
import csv
import sys
from os import path

import matplotlib.pyplot as plt


def check_args(args: list[str]) -> bool:
    """Check that given arguments are valid.

    Returns True if they are, False if not."""

    help_message = """
plotting.py: Un script pour générer des histogrammes
des usines de traitement d'eau à partir des données
traitées par C-WildWater.

Libs nécessaires: matplotlib

Utilisation: python <data_file> [args]

Arguments:
    help: Afficher ce message
"""
    for arg in args:
        if arg in ["help", "--help", "-h"]:
            print(help_message)
            return True
    if len(args) < 2:
        error_msg = "Pas assez d'arguments"
    elif len(args) > 2:
        error_msg = "Trop d'arguments"
    elif not path.isfile(args[1]):
        error_msg = f"Le fichier {args[1]} n'existe pas'"
    else:
        return True

    print(f"{help_message}\n\n")
    print(f"Erreur: {error_msg} !")

    return False


class Plotter:
    """A class to store plotting related functions."""

    data_path: str
    output_path: str
    data: dict[str:list[float]] | None

    def __init__(self, data_path: str) -> None:
        """Init the plotter."""
        self.data_path = data_path
        self.data = None
        self.output_path = self.get_output_path()

    def get_output_path(self) -> str:
        """Return the data path but change the extension to .png"""
        if "." not in self.data_path:
            print("Erreur: Le nom du fichier de données ne contient pas d'extension")
            sys.exit(3)

        splitted = self.data_path.split(".")
        return ".".join(splitted[:-1]) + ".png"

    def parse_data(self) -> dict[str:list[float]]:
        """Parse the data from the file given as an argument.

        Returns a dictionary with the factory id as the key
        and a list of floats as value."""

        result = {}
        with open(self.data_path, newline="", encoding="utf-8") as data_file:
            next(data_file)  # Skip header

            spamreader = csv.reader(data_file, delimiter=";")
            for row in spamreader:
                if len(row) < 2:
                    print(
                        "Erreur: Le fichier de données doit contenir au moins 2 colonnes !"
                    )
                    sys.exit(2)
                result.update({row[0]: [float(val) for val in row[1:]]})

        return result

    def get_filtered_data(self) -> dict[str:list[float]]:
        """Summs up the float values of each factory id, then order them by asc,
        and return a list of the 5 lowest and 5 highest factory values."""
        sorted_data = sorted(self.data.items(), key=lambda item: sum(item[1]))
        if len(sorted_data) > 10:
            filtered_data = sorted_data[:5] + sorted_data[-5:]
        else:
            filtered_data = sorted_data
        return filtered_data

    def plot(self) -> None:
        """Save the data parsed to a bar graph as a png."""
        if self.data is None:
            self.data = self.parse_data()

        filtered_data = self.get_filtered_data()

        xpoints = np.array([item[0] for item in filtered_data])
        ypoints = np.array([item[1] for item in filtered_data])

        plt.figure(figsize=(12, 6))

        # Initialize the bottom of the bars to zero
        bottom = np.zeros(len(xpoints))

        # Plot each layer of the stacked bar
        for i in range(ypoints.shape[1]):
            plt.bar(xpoints, ypoints[:, i], bottom=bottom)
            bottom += ypoints[:, i]

        # Add value labels on top of bars
        for i, total in enumerate(bottom):
            plt.text(i, total, round(total, 2), va="bottom", ha="center")

        plt.xticks(rotation=45, ha="right")
        plt.tight_layout()
        plt.savefig(self.output_path)


if __name__ == "__main__":
    if not check_args(sys.argv):
        sys.exit(1)

    data_path = sys.argv[1]
    plotter = Plotter(data_path)
    plotter.plot()
