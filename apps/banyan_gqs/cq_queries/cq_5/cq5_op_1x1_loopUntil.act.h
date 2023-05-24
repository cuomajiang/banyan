//
// Created by everlighting on 2021/4/9.
//
#pragma once

#include <brane/util/common-utils.hh>
#include <brane/actor/reference_base.hh>
#include <brane/actor/actor_implementation.hh>
#include "common/downstream_handlers.hh"

class i_cq5_op_1x0_loopOut;
class i_cq5_op_2_loopLeave_dedup;

class ANNOTATION(actor:reference) i_cq5_op_1x1_loopUntil : public brane::reference_base {
public:
    void process(VertexBatch&& input);
    void receive_eos(Eos&& eos);

    // Constructor.
    ACTOR_ITFC_CTOR(i_cq5_op_1x1_loopUntil);
    // Destructor
    ACTOR_ITFC_DTOR(i_cq5_op_1x1_loopUntil);
};

class ANNOTATION(actor:implement) cq5_op_1x1_loopUntil : public brane::stateless_actor {
public:
    void process(VertexBatch&& input);
    void receive_eos(Eos&& eos);

    void initialize() override;
    void finalize() override;

    // Constructor.
    ACTOR_IMPL_CTOR(cq5_op_1x1_loopUntil);
    // Destructor
    ACTOR_IMPL_DTOR(cq5_op_1x1_loopUntil);
    // Do work
    ACTOR_DO_WORK();

public:
    thread_local static std::unordered_set<int64_t> _successful_vertices;
    thread_local static std::unordered_set<int64_t> _failed_vertices;

private:
    unsigned _eos_rcv_num = 0;
    const unsigned _expect_eos_num = brane::global_shard_count();
    bool _notify_ds = false;

    const unsigned _max_loop_times = 5;
    unsigned _cur_loop_times = 0;

    std::unordered_set<int64_t> _vertices_have_seen{};

    std::unordered_set<int64_t> _expect_universities{};
    const std::string _workAt_label = "workAt";

    using next_loop_t = downstream_handler<i_cq5_op_1x0_loopOut, false, VertexBatch>;
    using loop_sync_t = downstream_handler<i_cq5_op_2_loopLeave_dedup, true, VertexBatch>;
    next_loop_t* _next_loop_hdl = nullptr;
    loop_sync_t* _loop_sync_hdl = nullptr;
};
