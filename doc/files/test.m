% example source code
% This function reads the current state of the robot 
% as well as ...

function robotData = controlJoints(robotData)

  %% Set variables:

  global PWD;
  
  persistent controlErrorInt;
  if robotData.initControlLoop
    controlErrorInt = [0; 0; 0];

  end
end