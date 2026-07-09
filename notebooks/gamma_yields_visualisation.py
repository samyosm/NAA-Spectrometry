# /// script
# dependencies = [
#     "altair==6.2.2",
#     "marimo",
#     "matplotlib==3.11.0",
#     "numpy==2.5.1",
#     "pandas==3.0.3",
#     "plotly==6.8.0",
#     "pyarrow==24.0.0",
#     "vegafusion==2.0.3",
#     "vl-convert-python==1.9.0.post1",
# ]
# requires-python = ">=3.13"
# ///

import marimo

__generated_with = "0.23.13"
app = marimo.App(width="medium")


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    # Predicted SRM 1633C Gamma Spectrum Analysis

    The following dataframe was generated from the `compute_gamma_yields_srm_1633c` in the gamma_yields module.

    ## Data Sources
    The data was gathered from the following datasets:
    - Ground states, isotopic abundances, half-lives, and gamma-ray intensities are fetched directly via the **IAEA Nuclear Data Section (NDS) API**.
    - Evaluated thermal neutron cross-sections ($\sigma_{(n,\gamma)}$) and uncertainties are parsed via the **curie library, querying the ENDF, TENDL, and IRDFF nuclear data libraries**.
    - Atomic masses and basic elemental properties are supplied by the internal dataset of the **curie library**.
    - The elemental makeup for the calculation is based on the standard reference material composition for **NIST SRM 1633c** (Fly Ash).

    ## DATAFRAME: SRM 1633C Gamma Yields
    """)
    return


@app.cell
def _():
    import altair as alt
    import marimo as mo
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd

    df = pd.read_csv("data/srm_1633c_gamma_yields.csv")
    df
    return alt, df, mo


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## GRAPH: Predicted NAA Gamma Spectrum

    The (interactive) graph below shows the predicted gamma specra lines we expect to find in SRM 1633C.
    """)
    return


@app.cell
def _(alt, df, mo):
    df_visible = df[df["n_gamma"] > 1e-1]

    selection = alt.selection_point(fields=["symbol"], bind="legend")

    chart = (
        alt.Chart(df_visible)
        .mark_rule(strokeWidth=2)
        .encode(
            x=alt.X("energy:Q", title="Gamma Energy (keV)"),
            y=alt.Y(
                "n_gamma:Q",
                title="Expected Gamma Yield (Counts)",
                scale=alt.Scale(type="log"),
            ),
            color=alt.Color("symbol:N", title="Element"),
            # Smooth UX: If selected, opacity is 1. If not, it drops to 0.05 (semi-invisible)
            opacity=alt.condition(selection, alt.value(1.0), alt.value(0)),
            tooltip=["symbol", "energy", "n_gamma", "product_half_life_sec"],
        )
        .add_params(selection)
        .properties(
            width=700, height=400, title="Predicted NAA Gamma Spectrum (SRM 1633c)"
        )
        .interactive()
    )

    mo.ui.altair_chart(chart, legend_selection=False, chart_selection=False)
    return (df_visible,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## GRAPH: Total Predicted Gamma Yield Contribution by Element

    The (interactive) graph below showcases the gamma yield contribution for each element.
    """)
    return


@app.cell
def _(alt, df_visible, mo):
    df_bar = df_visible.groupby(["symbol"])["n_gamma"].sum().reset_index()

    bar_chart = (
        alt.Chart(df_bar)
        .mark_bar()
        .encode(
            x=alt.X(
                "symbol:N",
                title="Element",
                sort=alt.EncodingSortField(field="n_gamma", order="descending"),
            ),
            y=alt.Y(
                "n_gamma:Q",
                title="Total Cumulative Gamma Yield (Counts)",
                scale=alt.Scale(type="log", domainMin=0.1),
            ),
            y2=alt.datum(0.1),
            tooltip=["symbol", "n_gamma"],
        )
        .properties(
            width=750,
            height=400,
            title="Total Predicted Gamma Yield Contribution by Element (SRM 1633c)",
        )
        .interactive()
    )

    mo.ui.altair_chart(bar_chart)
    return


if __name__ == "__main__":
    app.run()
