// from ODE 0.035 documentation (pre release)

// if (k > 0 || c > 0) then no division by zero (dt > 0 always)
#define KC2ERP(SPRING, DAMP, DT) ( (DT) * (SPRING) / ( (DT) * (SPRING) + (DAMP) ) )
#define KC2CFM(SPRING, DAMP, DT) ( 1 / ( (DT) * (SPRING) + (DAMP) ) )
// if (cfm > 0) no division by zero (dt > 0 always)
#define EC2K(ERP, CFM, DT) ( (ERP) / (DT) / (CFM) )
#define EC2C(ERP, CFM, DT) ( ( (CFM) - (ERP) ) / (DT) / (CFM) )


