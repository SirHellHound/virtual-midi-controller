
#pragma once

#include "JuceHeader.h"

namespace vmc {

class Device : public ReferenceCountedObject
{
public:
    using Ptr = ReferenceCountedObjectPtr<Device>;
    Device() = default;
    virtual ~Device() = default;
};

}
