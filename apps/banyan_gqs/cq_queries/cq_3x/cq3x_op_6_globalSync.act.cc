#include "common/query_helpers.hh"
#include "cq3x_executor.hh"
#include "cq3x_op_6_globalSync.act.h"

void cq3x_op_6_globalSync::process(VertexBatch &&input) {
    for (unsigned i = 0; i < input.size(); i++) {
        _order_heap.push(input[i]);
        if (_order_heap.size() > _expect_num) {
            _order_heap.pop();
        }
    }
}

void cq3x_op_6_globalSync::receive_eos(Eos&& eos) {
    if (++_eos_rcv_num == _expect_eos_num) {
        if (at_cq) {
            cq_end_action();
        }
    }
}

void cq3x_op_6_globalSync::log_results() {
    std::vector<int64_t> results{};
    results.reserve(_order_heap.size());
    while (!_order_heap.empty()) {
        results.push_back(_order_heap.top());
        _order_heap.pop();
    }
    write_log("[CQ 3x finished]\n");
    for (auto i = results.rbegin(); i != results.rend(); i++) {
        write_log(seastar::format("  FriendId: {}\n", *i));
    }
}

void cq3x_op_6_globalSync::cq_end_action() {
    cq3_helper.record_end_time();
//    log_results();

    auto query_id = get_scope().get_root_scope_id();
    brane::actor_engine().cancel_query_request(query_id, get_query_actor_group_typeId()).then([] {
        cq3_helper.next_round();
        if (cq3_helper.current_round() < (exec_epoch * param_num)) {
            exec_cq3x(cq3_helper.get_start_person(cq3_helper.current_query_id()));
        } else {
            cq3_helper.log_cq_exec_times();
            brane::actor_engine().exit();
        }
    }, nullptr);
}
