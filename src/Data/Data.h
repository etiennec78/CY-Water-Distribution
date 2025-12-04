typedef enum {
    SOURCE,
    SPRING,
    POND,
    CATCHMENT,
    WELL,
    RESURGENCE,
    INTAKE,
    BOREHOLE,
    FOUNTAIN,
    RESERVOIR,
    FORAGE,
    WELL_FIELDS,
    TANK,
    UNIT,
    MODULE,
    FACILITY_COMPLEX,
    STORAGE,
    JUNCTION,
    CUST,
} FacilityType;

typedef struct {
    FacilityType facility;
    char* facilityID;
    FacilityType upstream;
    char* upstreamID;
    FacilityType downstream;
    char* downstreamID;
    float annualAmount;
    float leakPercentage;
} Water;
