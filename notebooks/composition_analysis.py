import marimo

__generated_with = "0.23.13"
app = marimo.App()


@app.cell
def _():
    import matplotlib.pyplot as plt
    import pandas as pd

    SRM_1633C_COMPOSITION = {
        "Si": 0.213000,
        "Al": 0.132800,
        "Fe": 0.104900,
        "K": 0.017730,
        "Ca": 0.013650,
        "Ti": 0.007240,
        "Mg": 0.004980,
        "P": 0.001920,
        "Na": 0.001707,
        "Ba": 0.001126,
        "S": 0.001100,
        "Sr": 0.000901,
        "Cr": 0.000258,
        "V": 0.000286,
        "Mn": 0.000240,
        "Zn": 0.000235,
        "As": 0.000186,
        "Cu": 0.000174,
        "Ni": 0.000132,
        "Rb": 0.000117,
        "Pb": 0.000095,
        "La": 0.000087,
        "Co": 0.000043,
        "Sc": 0.000038,
        "Th": 0.000023,
        "Dy": 0.000019,
        "Se": 0.000014,
        "Cs": 0.000009,
        "U": 0.000009,
        "Sb": 0.000009,
        "Eu": 0.000005,
        "Tb": 0.000003,
        "Ta": 0.000002,
        "Lu": 0.000001,
        "Hg": 0.000001,
        "Cd": 0.000001,
        "O": 0.496799,
    }

    df = pd.DataFrame(SRM_1633C_COMPOSITION.items(), columns=['symbol', 'percent'])
    df = df.sort_values(by='percent', ascending=True)
    df
    return df, plt


@app.cell
def _(df, plt):
    import matplotlib.ticker as mticker

    fig, ax = plt.subplots(figsize=(8, 10))

    bars = ax.barh(df["symbol"], df["percent"])

    ax.set_xscale('log')
    ax.xaxis.set_major_formatter(mticker.PercentFormatter(xmax=1.0))

    ax.grid(True, which="both", axis="x", linestyle="--", alpha=0.5)
    ax.set_axisbelow(True)

    labels = [f"{val * 100:.6g}%" for val in df["percent"]]
    text_labels = ax.bar_label(
        bars, 
        labels=labels, 
        label_type='edge',
        padding=6,
    )

    xmin, xmax = ax.get_xlim()
    ax.set_xlim(xmin, xmax * 3.5)

    ax.set_title("Elemental Composition of SRM 1633C (Fly Ash)",)
    ax.set_xlabel("Mass Fraction (%)")
    ax.set_ylabel("Element Symbol")

    fig.tight_layout()

    plt.savefig("assets/elemental_composition_of_srm_1633c.png", dpi=600)
    plt.show()
    return


if __name__ == "__main__":
    app.run()
