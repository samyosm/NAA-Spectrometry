import marimo

__generated_with = "0.23.13"
app = marimo.App(width="medium")


@app.cell
def _():
    import marimo as mo
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd
    import uproot

    root_file = uproot.open("data/srm_1633c_HPGe_detector_simulation.root")
    tree = root_file["Spectrum"]
    df = tree.arrays(library="pd")
    df
    return df, pd, plt


@app.cell
def _(pd):
    gamma_lines = pd.read_csv("data/srm_1633c_gamma_yields.csv")

    # For each element, keep only its single highest-yield gamma line
    strongest_line_per_element = (
        gamma_lines
        .sort_values("n_gamma", ascending=False)
        .drop_duplicates(subset="symbol", keep="first")
    )

    # Now take the top 15 elements by that strongest line's yield
    top_lines = strongest_line_per_element.nlargest(15, "n_gamma")
    return (top_lines,)


@app.cell
def _(df, plt, top_lines):
    from adjustText import adjust_text

    fig, ax = plt.subplots(figsize=(16, 5))
    ax.hist(df["EdepKeV"], bins=2000, histtype="bar")
    ax.set_yscale("log")

    ax.set_xlabel("Energy (keV)")
    ax.set_ylabel("Counts")
    ax.set_title("HPGe Detector Simulation of NAA Gamma Spectrum for SRM 1633c")

    texts = []
    for i, row in top_lines.iterrows():
        energy = row["energy"]
        label = f"{int(row['target_z'])}{row['symbol']}\n{energy:.0f}"
    
        ax.axvline(x=energy, color='black', linestyle='--', alpha=0.5, linewidth=0.5)
        t = ax.text(energy, 100000, label, color='black', ha='center', fontsize=8)
        texts.append(t)

    adjust_text(
        texts, 
        ax=ax,
        expand=(1.5, 1.8),         
        arrowprops=dict(arrowstyle="->", color="black", lw=0.7, alpha=1),
        only_move={'text': 'xy'} 
    )
    plt.tight_layout()
    plt.savefig(
        "assets/HPGe_detector_simulation_of_naa_gamma_spectrum_for_srm_1633c.png",
        dpi=600,
    )
    plt.show()
    return


if __name__ == "__main__":
    app.run()
