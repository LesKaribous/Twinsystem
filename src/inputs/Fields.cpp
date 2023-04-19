#include "inputs/Fields.h"
namespace TwinSystem{
    Fields::Fields(){};

    void Fields::Initialize(){
        x.Init();
        y.Init();
        z.Init();
        probed.Init();
        probing.Init();
        started.Init();
        armed.Init();
        intercom.Init();
        score.Init();
        time.Init();
    }

    void Fields::Read(){
        x.Read();
        y.Read();
        z.Read();
        probed.Read();
        probing.Read();
        started.Read();
        armed.Read();
        intercom.Read();
        score.Read();
        time.Read();
    }

    bool Fields::HasChanged(){ //TODO Add the rest
        return  x.HasChanged()          ||
                y.HasChanged()          ||
                z.HasChanged()          ||
                probed.HasChanged()     ||
                probing.HasChanged()    ||
                started.HasChanged()    ||
                armed.HasChanged()      ||
                intercom.HasChanged()   ||
                score.HasChanged()      ||
                time.HasChanged();
    }



} // namespace TwinSystem


