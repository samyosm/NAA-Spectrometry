import io

import pandas as pd
import requests
from curie import Element, Reaction

from constants import N_A, SRM_1633C_COMPOSITION, THERMAL_NEUTRON_ENERGY_MEV


def fetch_iaea(payload: dict) -> pd.DataFrame:
    BASE_URL = "https://nds.iaea.org/relnsd/v1/data"
    HEADERS = {"User-Agent": "Livechart/1.0"}

    response = requests.get(BASE_URL, headers=HEADERS, params=payload)
    response.raise_for_status()

    return pd.read_csv(io.StringIO(response.text))


def get_thermal_xs(
    target_symbol: str, target_mass: int, product_symbol: str, product_mass: int
):
    reaction = f"{target_mass}{target_symbol.upper()}(n,g){product_mass}{product_symbol.upper()}"

    xs, xs_lib = None, None
    unc, unc_lib = None, None

    for lib in ["endf", "tendl", "irdff"]:
        try:
            rx = Reaction(reaction, lib)
        except Exception:
            continue

        if xs is None:
            xs = float(rx.interpolate(THERMAL_NEUTRON_ENERGY_MEV))
            xs_lib = lib

        candidate_unc = float(rx.interpolate_unc(THERMAL_NEUTRON_ENERGY_MEV))
        if unc is None and candidate_unc > 0:
            unc = candidate_unc
            unc_lib = lib

        if xs is not None and unc is not None:
            break

    if xs is None:
        print(f"Reaction data not found for: {reaction}")
    elif unc is None:
        # Central value exists but no library carried non-zero covariance
        unc_lib = "MISSING_COVARIANCE"

    return xs, unc, xs_lib, unc_lib


def get_element_gamma_signature(element: str):
    all_isotopes = fetch_iaea({"fields": "ground_states", "nuclides": "all"})

    element_isotopes = all_isotopes.loc[all_isotopes["symbol"] == element]
    abundant_isotopes = element_isotopes.loc[element_isotopes["abundance"] > 0.0]

    all_gamma_lines = []

    for _, target in abundant_isotopes.iterrows():
        z = int(target["z"])
        n = int(target["n"])

        mass = z + n

        # Half life of the isotope after acquiring a neutron
        product_isotope = element_isotopes.loc[
            (element_isotopes["z"] == z) & (element_isotopes["n"] == (n + 1))
        ]

        if product_isotope.empty:
            continue

        if not (half_life := product_isotope["half_life_sec"].values[0]) > 0:
            continue

        product_n = int(product_isotope["n"].values[0])
        product_z = int(product_isotope["z"].values[0])
        product_mass = product_n + product_z

        product_symbol = product_isotope["symbol"].values[0]

        thermal_xs, thermal_unc_xs, thermal_xs_lib, thermal_xs_unc_lib = get_thermal_xs(
            target["symbol"], mass, product_symbol, product_mass
        )

        product_nuclide = f"{product_mass}{product_symbol}"

        gamma_lines = fetch_iaea(
            {"fields": "decay_rads", "rad_types": "g", "nuclides": product_nuclide}
        )

        if gamma_lines.empty:
            continue

        gamma_lines = gamma_lines[
            ["energy", "unc_en", "intensity", "unc_i"]
        ].drop_duplicates()

        gamma_lines = gamma_lines.assign(
            symbol=target["symbol"],
            target_abundance=target["abundance"],
            target_unc_a=target["unc_a"],
            target_n=n,
            target_z=z,
            product_half_life_sec=half_life,
            product_unc_hls=product_isotope["unc_hls"].values[0],
            product_n=product_n,
            product_z=product_z,
            thermal_xs_mb=thermal_xs,
            thermal_unc_xs_mb=thermal_unc_xs,
            thermal_xs_lib=thermal_xs_lib,
            thermal_xs_unc_lib=thermal_xs_unc_lib,
        )

        all_gamma_lines.append(gamma_lines)

    if not all_gamma_lines:
        return None

    return pd.concat(all_gamma_lines, ignore_index=True)


def get_element_molar_mass(element):
    el = Element(element)
    return el.mass


def get_compound_gamma_signature(composition: dict[str, float], sample_mass: float):
    compound_gamma_signatures = []

    for element, mass_pct in composition.items():
        element_gamma_lines = get_element_gamma_signature(element)

        if element_gamma_lines is None or element_gamma_lines.empty:
            continue

        molar_mass = get_element_molar_mass(element)
        element_mass = sample_mass * mass_pct
        element_moles = element_mass / molar_mass
        element_atoms = element_moles * N_A

        element_gamma_lines = element_gamma_lines.assign(
            atom_count=element_atoms * (element_gamma_lines["target_abundance"] / 100.0)
        )

        compound_gamma_signatures.append(element_gamma_lines)

    compound_gamma_signature = pd.concat(compound_gamma_signatures, ignore_index=True)
    return compound_gamma_signature


def main():
    cg = get_compound_gamma_signature(SRM_1633C_COMPOSITION, 75)
    cg.to_csv("signature.csv")


if __name__ == "__main__":
    main()
