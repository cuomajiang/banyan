//
// Created by everlighting on 2020/9/28.
//
#pragma once

#include <brane/util/common-utils.hh>
#include <brane/actor/reference_base.hh>
#include <brane/actor/actor_implementation.hh>
#include "common/downstream_handlers.hh"

class i_q5_op_3_dedup_friendStore_in;
class i_q5_op_7_globalSync_order;

class ANNOTATION(actor:reference) i_q5_op_5_out : public brane::reference_base {
public:
    void process(vertex_forumId_Batch&& input);
    void receive_eos(Eos&& eos);

    // Constructor.
    ACTOR_ITFC_CTOR(i_q5_op_5_out);
    // Destructor
    ACTOR_ITFC_DTOR(i_q5_op_5_out);
};

class ANNOTATION(actor:implement) q5_op_5_out : public brane::stateful_actor {
public:
    void process(vertex_forumId_Batch&& input);
    void receive_eos(Eos&& eos);

    void initialize() override;
    void finalize() override;

    // Constructor.
    ACTOR_IMPL_CTOR(q5_op_5_out);
    // Destructor
    ACTOR_IMPL_DTOR(q5_op_5_out);
    // Do work
    ACTOR_DO_WORK();

private:
    unsigned _eos_rcv_num = 0;
    const unsigned _expect_eos_num = brane::global_shard_count();
    bool _notify_ds = false;

    using ds_group_t = downstream_group_handler<i_q5_op_3_dedup_friendStore_in, vertex_forumId_Batch>;
    const unsigned _ds_op_id = 3;
    ds_group_t* _ds_group_hdl = nullptr;
    using sync_t = downstream_handler<i_q5_op_7_globalSync_order, true>;
    sync_t* _sync_hdl = nullptr;

    const std::string _out_label = "hasCreator";
};

