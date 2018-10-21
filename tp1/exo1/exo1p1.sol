<?xml version = "1.0" encoding="UTF-8" standalone="yes"?>
<CPLEXSolution version="1.2">
 <header
   problemName="exo1p1.lp"
   objectiveValue="15"
   solutionTypeValue="1"
   solutionTypeString="basic"
   solutionStatusValue="1"
   solutionStatusString="optimal"
   solutionMethodString="dual"
   primalFeasible="1"
   dualFeasible="1"
   simplexIterations="0"
   writeLevel="1"/>
 <quality
   epRHS="1e-06"
   epOpt="1e-06"
   maxPrimalInfeas="0"
   maxDualInfeas="0"
   maxPrimalResidual="0"
   maxDualResidual="0"
   maxX="3"
   maxPi="2.5"
   maxSlack="13"
   maxRedCost="10.5"
   kappa="2.33333333333333"/>
 <linearConstraints>
  <constraint name="c1" index="0" status="BS" slack="-1" dual="0"/>
  <constraint name="c2" index="1" status="LL" slack="0" dual="2.5"/>
  <constraint name="c3" index="2" status="BS" slack="13" dual="-0"/>
 </linearConstraints>
 <variables>
  <variable name="x" index="0" status="BS" value="3" reducedCost="-0"/>
  <variable name="y" index="1" status="LL" value="0" reducedCost="-10.5"/>
 </variables>
</CPLEXSolution>
