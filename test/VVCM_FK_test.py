import numpy as np
from VVCM import VVCM_FK


def test_VVCM_FK():
    N = 4  # number of robots
    zr = 1000.0  # height of the holding points (unit: mm)
    # robot formation (x, y) coordinates (unit: mm)
    Rn = np.array(
        [
            [213.7, 122.7],
            [804.6, 37.2],
            [904.0, 550.0],
            [439.3, 715.9],
        ]
    )
    # shape of the deformable sheet (x, y) coordinates (unit: mm)
    Vn = np.array(
        [
            [-316.1, -421.9],
            [803.4, -384.1],
            [746.1, 712.8],
            [-367.3, 664.2],
        ]
    )

    print("----------------------")

    a = VVCM_FK(N, zr, Vn)  # initialize the VVCM_FK object
    a.update_stable_solutions(Rn)  # compute the stable forward kinematics

    # M: the number of stable solutions found
    print(f"M: {a.M}")

    if a.M == 0:
        print("No stable solution found.")
        return

    # Po: object pose (x, y, z) coordinates for each stable solution (unit: mm)
    print("Po: ")
    for po in a.Po:
        print(po)

    # Vo: deformable sheet vertex (x, y) coordinates for each stable solution (unit: mm)
    print("Vo: ")
    for vo in a.Vo:
        print(vo)

    print("----------------------")


if __name__ == "__main__":
    test_VVCM_FK()
