//
// Created by everlighting on 2020/8/5.
//

#pragma once

#include <brane/util/common-utils.hh>
#include <brane/actor/reference_base.hh>
#include <brane/actor/actor_implementation.hh>
#include "common/downstream_handlers.hh"

class i_q1_op_1x0_loopOut;

class ANNOTATION(actor:reference) i_q1_op_0_V_loopEnter : public brane::reference_base {
public:
    void trigger(StartVertex&& src);

    // Constructor.
    ACTOR_ITFC_CTOR(i_q1_op_0_V_loopEnter);
    // Destructor
    ACTOR_ITFC_DTOR(i_q1_op_0_V_loopEnter);
};

class ANNOTATION(actor:implement) q1_op_0_V_loopEnter : public brane::stateless_actor {
public:
    void trigger(StartVertex&& src);

    void initialize() override;
    void finalize() override;

    // Constructor.
    ACTOR_IMPL_CTOR(q1_op_0_V_loopEnter);
    // Destructor
    ACTOR_IMPL_DTOR(q1_op_0_V_loopEnter);
    // Do work
    ACTOR_DO_WORK();

private:
    using ds_group_t = downstream_group_handler<i_q1_op_1x0_loopOut, vertex_pathLen_Batch>;
    ds_group_t* _ds_group_hdl = nullptr;
};

