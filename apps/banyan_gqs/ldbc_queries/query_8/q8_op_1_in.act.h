//
// Created by everlighting on 2020/10/9.
//
#pragma once

#include <brane/util/common-utils.hh>
#include <brane/actor/reference_base.hh>
#include <brane/actor/actor_implementation.hh>
#include "common/downstream_handlers.hh"

class i_q8_op_2_in;
class i_q8_op_4_globalSync_order_limit;

class ANNOTATION(actor:reference) i_q8_op_1_in : public brane::reference_base {
public:
    void process(StartVertex&& input);
    void receive_eos(Eos&& eos);

    // Constructor.
    ACTOR_ITFC_CTOR(i_q8_op_1_in);
    // Destructor
    ACTOR_ITFC_DTOR(i_q8_op_1_in);
};

class ANNOTATION(actor:implement) q8_op_1_in : public brane::stateful_actor {
public:
    void process(StartVertex&& input);
    void receive_eos(Eos&& eos);

    void initialize() override;
    void finalize() override;

    // Constructor.
    ACTOR_IMPL_CTOR(q8_op_1_in);
    // Destructor
    ACTOR_IMPL_DTOR(q8_op_1_in);
    // Do work
    ACTOR_DO_WORK();

private:
    using ds_group_t = downstream_group_handler<i_q8_op_2_in, VertexBatch>;
    ds_group_t* _ds_group_hdl = nullptr;

    using sync_t = downstream_handler<i_q8_op_4_globalSync_order_limit, true>;
    sync_t* _sync_hdl = nullptr;

    const std::string _in_label = "hasCreator";
};
