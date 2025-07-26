"""
VVCM Simulation module for simulating multi-robot deformable sheet transport system
"""
from __future__ import annotations
import numpy
import typing
__all__ = ['VVCM_FK', 'VVCM_FK_Error', 'VVCM_ManualSimulation', 'VVCM_Simulation']
class VVCM_FK:
    """
    Get Stable Solutions of Forward Kinematics for Multi-Robot Deformable Sheet Transport System
    """
    def __init__(self, N: int, zr: float, Vn: numpy.ndarray[numpy.float32[m, n]]) -> None:
        ...
    def update_stable_solutions(self, Rn: numpy.ndarray[numpy.float32[m, n]]) -> VVCM_FK_Error:
        ...
    @property
    def ITn(self) -> list[int]:
        """
        Number of non-taut cables in each solution
        """
    @property
    def It(self) -> list[list[int]]:
        """
        Taut cable set in all stable solutions
        """
    @property
    def M(self) -> int:
        """
        Number of stable solutions
        """
    @property
    def N(self) -> int:
        """
        Number of robots
        """
    @property
    def Po(self) -> list[numpy.ndarray[numpy.float32[3, 1]]]:
        """
        Object positions in world frame in all stable solutions
        """
    @property
    def Rn(self) -> numpy.ndarray[numpy.float32[m, n]]:
        """
        Current robot formation
        """
    @property
    def Tn(self) -> list[int]:
        """
        Number of taut cables in each solution
        """
    @property
    def Vn(self) -> numpy.ndarray[numpy.float32[m, n]]:
        """
        Sheet shape
        """
    @property
    def Vo(self) -> list[numpy.ndarray[numpy.float32[2, 1]]]:
        """
        Object positions in sheet frame in all stable solutions
        """
    @property
    def zr(self) -> float:
        """
        Height of holding point
        """
class VVCM_FK_Error:
    """
    Error Type for VVCM_FK
    
    Members:
    
      NoError : No Error
    
      NoSolution : No Solution
    
      NoStableSolution : No Stable Solution
    
      InFeasibleFormation : Rn is not inside Vn
    """
    InFeasibleFormation: typing.ClassVar[VVCM_FK_Error]  # value = <VVCM_FK_Error.InFeasibleFormation: 3>
    NoError: typing.ClassVar[VVCM_FK_Error]  # value = <VVCM_FK_Error.NoError: 0>
    NoSolution: typing.ClassVar[VVCM_FK_Error]  # value = <VVCM_FK_Error.NoSolution: 1>
    NoStableSolution: typing.ClassVar[VVCM_FK_Error]  # value = <VVCM_FK_Error.NoStableSolution: 2>
    __members__: typing.ClassVar[dict[str, VVCM_FK_Error]]  # value = {'NoError': <VVCM_FK_Error.NoError: 0>, 'NoSolution': <VVCM_FK_Error.NoSolution: 1>, 'NoStableSolution': <VVCM_FK_Error.NoStableSolution: 2>, 'InFeasibleFormation': <VVCM_FK_Error.InFeasibleFormation: 3>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class VVCM_ManualSimulation:
    """
    Simulation Engine for Multi-Robot Deformable Sheet Transport System.
    It does not simulate the motion of the robots, but give the stable solution
    when given the formation.
    """
    def __init__(self, N: int, zr: float, Vn: numpy.ndarray[numpy.float32[m, n]]) -> None:
        ...
    def get_new_stable_solution(self, Rn: numpy.ndarray[numpy.float32[m, n]]) -> tuple[VVCM_FK_Error, numpy.ndarray[numpy.float32[3, 1]]]:
        """
        Get new stable solution with changed formation.
        
        Args:
            Rn: current robot formation
        
        Returns:
            Po
        """
    def init(self, Rn_initial: numpy.ndarray[numpy.float32[m, n]], Po_initial: numpy.ndarray[numpy.float32[3, 1]] = ...) -> tuple[VVCM_FK_Error, numpy.ndarray[numpy.float32[3, 1]]]:
        """
        init the engine, all the unit of length is mm or s.
        
        Args:
            Rn_initial: current robot formation
            Po_initial: current Po (unimportant, it affets the solution choosen)
        
        Returns:
            Po
        """
    @property
    def It(self) -> list[int]:
        """
        The taut cable set
        """
    @property
    def Po(self) -> numpy.ndarray[numpy.float32[3, 1]]:
        """
        Current object position (the true position of the object should be Po + global_pos)
        """
    @property
    def Rn(self) -> numpy.ndarray[numpy.float32[m, n]]:
        """
        Current robot formation (the true position of all robots should be Rn + global_pos)
        """
    @property
    def fk_engine(self) -> VVCM_FK:
        """
        Forward Kinematics Engine
        """
    @property
    def global_pos(self) -> numpy.ndarray[numpy.float32[2, 1]]:
        """
        Global position of the formation
        """
    @property
    def solution_idx(self) -> int:
        """
        Index of the solution in the fk_engine
        """
class VVCM_Simulation:
    """
    Simulation Engine for Multi-Robot Deformable Sheet Transport System
    """
    def __init__(self, N: int, zr: float, Vn: numpy.ndarray[numpy.float32[m, n]], Rn_initial: numpy.ndarray[numpy.float32[m, n]], Po_initial: numpy.ndarray[numpy.float32[3, 1]] = ..., dt: float = 0.03333333333333333) -> None:
        """
        init the engine, all the unit of length is mm or s.
        
        Args:
            N: robot number
            zr: the height of holding point
            Vn: sheet shape
            Rn_initial: current robot formation
            Po_initial: current Po (unimportant, it affets the solution choosen)
            dt: time step for the simulation
        """
    def get_absolute_Rn(self) -> numpy.ndarray[numpy.float32[m, 2]]:
        """
        Get the Absolute Rn object
        Returns:
            true position of all robots
        """
    def set_velocity(self, Rn_velocity: numpy.ndarray[numpy.float32[m, n]]) -> None:
        """
        Set velocity for the robot formation
        
        Args:
            Rn_velocity: N x 2 velocity vector
        """
    def step(self) -> None:
        """
        Simulation step
        """
    @property
    def It(self) -> list[int]:
        """
        The taut cable set
        """
    @property
    def Po(self) -> numpy.ndarray[numpy.float32[3, 1]]:
        """
        Current object position (the true position of the object should be Po + global_pos)
        """
    @property
    def Rn(self) -> numpy.ndarray[numpy.float32[m, n]]:
        """
        Current robot formation (the true position of all robots should be Rn + global_pos)
        """
    @property
    def Rn_vel(self) -> numpy.ndarray[numpy.float32[m, n]]:
        """
        Velocity of the robots (N x 2)
        """
    @property
    def dt(self) -> float:
        """
        Time step for the simulation
        """
    @property
    def fk_engine(self) -> VVCM_FK:
        """
        Forward Kinematics Engine
        """
    @property
    def global_pos(self) -> numpy.ndarray[numpy.float32[2, 1]]:
        """
        Global position of the formation
        """
    @property
    def solution_idx(self) -> int:
        """
        Index of the solution in the fk_engine
        """
