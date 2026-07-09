import sys

import pandas as pd
from geant4_pybind import *


def main():
    ui = g4uiexecutive(len(sys.argv), sys.argv)
    ui.sessionstart()

    gamma_yields = pd.read_csv("data/srm_1633c_gamma_yields.csv")
    print(gamma_yields)


if __name__ == "__main__":
    main()
