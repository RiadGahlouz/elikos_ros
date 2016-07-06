Module de navigation fonctionnant à l'aide de moveit (http://moveit.ros.org/).
Description des packages:
 - elikos_nav : Package principal. Contient le launch file pour la navigation stack. Reçoit les cibles de l'AI et contient le move_group (elikos_moveit_move_group) qui les envoie dans le planning_pipeline de moveit. Contient le modèle du drone utilisé par moveit dans le dossier cfg/.
 - elikos_moveit_config : Package en partie auto-généré par le  MoveitSetupAssistant. Contient les configurations pour lier les senseurs 3D et les différents paramètres de moveit. Le launch file moveit_master.launch sert à lancer les différents packages de moveit.
 - elikos_moveit_controller_manager : Custom plugin pour supporter les multi_dof_follow_joint_trajectory.
 - elikos_action_controller : Reçoit les actions multi_dof_follow_joint_trajectory produites par moveit et broadcast les tf::Transforms représentant les points à franchir dans la trajectoire. La logique d'envoi des tf est dans src/elikos_actionController.cpp.

Plusieurs parties du code sont librement inspirées du repository suivant : https://github.com/AlessioTonioni/Autonomous-Flight-ROS