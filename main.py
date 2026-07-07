from constants import SRM_1633C_COMPOSITION


def main():
    total = 0

    for _, percent in SRM_1633C_COMPOSITION.items():
        total += percent

    print(total)


if __name__ == "__main__":
    main()
