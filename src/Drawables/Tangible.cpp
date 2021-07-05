#include "Tangible.hpp"

Tangible::Tangible()
{

}

void Tangible::update()
{
    if(!mInited) {
        init();
        mInited = true;
    }
    if(mGPU_outOfSync)
        sync_GPU();
    update_self();
}
