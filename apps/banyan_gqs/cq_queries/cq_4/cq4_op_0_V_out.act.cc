//
// Created by everlighting on 2021/4/15.
//

#include "berkeley_db/storage/bdb_graph_handler.hh"
#include "common/query_helpers.hh"
#include "common/schedule_helpers.hh"
#include "cq4_op_0_V_out.act.h"
#include "cq4_op_2_whereEnter.act.h"

void cq4_op_0_V_out::trigger(StartVertex&& src) {
    _src_vertex = src.val;

    std::vector<int64_t> organisations{};
    int64_t out_neighbour;
    auto iter = storage::bdb_graph_handler::_local_graph->get_out_v(_src_vertex, _side_out_label);
    while (iter.next(out_neighbour)) {
        organisations.push_back(out_neighbour);
    }
    cq4_helper.set_organisation_ids(organisations);

    if (_machine_num == 1) {
        forward();
        return;
    }

    std::vector<seastar::future<TrivialStruct<bool> > > futs;
    futs.reserve(_machine_num - 1);
    auto builder = get_scope();
    for (unsigned anchor = 0; anchor < brane::global_shard_count(); anchor += brane::local_shard_count()) {
        if ((brane::global_shard_id() >= anchor) && (brane::global_shard_id() < anchor + brane::local_shard_count())) {
            continue;
        } else {
            VertexBatch output{static_cast<uint32_t>(organisations.size())};
            for (auto& n : organisations) {
                output.push_back(n);
            }
            builder.reset_shard(anchor);
            futs.push_back(builder.build_ref<param_setter_t>(0).set_params(std::move(output)));
        }
    }
    seastar::when_all_succeed(futs.begin(), futs.end()).then([this] (auto) {
        forward();
    }, this);
}

seastar::future<TrivialStruct<bool> > cq4_op_0_V_out::set_params(VertexBatch &&input) {
    cq4_helper.set_organisation_ids(std::move(input));
    return seastar::make_ready_future<TrivialStruct<bool> >(TrivialStruct<bool>{true});
}

void cq4_op_0_V_out::forward() {
    int64_t out_neighbour;
    auto iter = storage::bdb_graph_handler::_local_graph->get_out_v(_src_vertex, _out_label);
    while (iter.next(out_neighbour)) {
        _ds_group_hdl->emplace_data(locate_helper::locate(out_neighbour), out_neighbour);
    }
    _ds_group_hdl->flush();
    _ds_group_hdl->receive_eos();
}

void cq4_op_0_V_out::initialize() {
    auto builder = get_scope();
    _ds_group_hdl = new ds_group_t(builder, _ds_op_id);
}

void cq4_op_0_V_out::finalize() {
    delete _ds_group_hdl;
}
