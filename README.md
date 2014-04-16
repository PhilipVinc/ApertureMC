ApertureMC
==========

A monte carlo simulation to find the shape of the aperture, given some experimental diffraction data.

The program works via command line. It currently expects two file to work. those must be in the same directory:

  fondo.xls -> A file containing a noise measurement. 
  
  XXXXX.xls -> The file containing the experimental measurement you wish to simulate.
  
  
  
  
  
Usage: DataElaborator
[-x] se il nome del file contiene ".xls" \n

[-p] se si vuole la barra di avanzamento/progresso

[-n int] per il numero di Iterazioni per gruppo di fenditure. Default: 1000000

[-j int] numero di job paralleli (suggerito: threads -1). Default: 7

[-m int] numero di fenditure minime da simulare. Default: 3

[-f int] numero di fenditure massime da simulare. Default: 3

[-s int] Seed. Default: 5
