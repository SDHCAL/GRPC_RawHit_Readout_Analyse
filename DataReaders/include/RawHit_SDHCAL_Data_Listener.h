#ifndef RawHit_SDHCAL_Data_Listener_HH
#define RawHit_SDHCAL_Data_Listener_HH

class RawHit_SDHCAL_Data;

class RawHit_SDHCAL_Data_Listener
{
 public:
  virtual ~RawHit_SDHCAL_Data_Listener() {;}
  virtual void process(const RawHit_SDHCAL_Data&) =0;
};


#endif
