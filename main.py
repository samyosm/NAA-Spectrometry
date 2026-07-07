def main():
    from curie import Reaction  # or npat

    rx = Reaction("54FE(n,g)55FE", "tendl")
    print(rx.eng[:10])
    print(rx.xs[:10])
    print(rx.unc_xs[:10])


if __name__ == "__main__":
    main()
