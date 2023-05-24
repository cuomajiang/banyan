//
// Created by everlighting on 2020/9/29.
//

#pragma once

#include <brane/util/common-utils.hh>
#include <brane/actor/reference_base.hh>
#include <brane/actor/actor_implementation.hh>
#include "common/downstream_handlers.hh"

class i_q6_op_1x1_loopUntil;

class ANNOTATION(actor:reference) i_q6_op_1x0_loopOut : public brane::reference_base {
public:
    void process(VertexBatch&& input);
    void receive_eos(Eos&& eos);

    // Constructor.
    ACTOR_ITFC_CTOR(i_q6_op_1x0_loopOut);
    // Destructor
    ACTOR_ITFC_DTOR(i_q6_op_1x0_loopOut);
};

class ANNOTATION(actor:implement) q6_op_1x0_loopOut : public brane::stateful_actor {
public:
    void process(VertexBatch&& input);
    void receive_eos(Eos&& eos);

    void initialize() override;
    void finalize() override;

    // Constructor.
    ACTOR_IMPL_CTOR(q6_op_1x0_loopOut);
    // Destructor
    ACTOR_IMPL_DTOR(q6_op_1x0_loopOut);
    // Do work
    ACTOR_DO_WORK();

private:
    using ds_group_t = downstream_group_handler<i_q6_op_1x1_loopUntil, VertexBatch>;
    ds_group_t* _ds_group_hdl = nullptr;

    const std::string _out_label = "knows";
};

