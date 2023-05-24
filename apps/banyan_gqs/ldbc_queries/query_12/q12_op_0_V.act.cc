//
// Created by everlighting on 2020/10/10.
//

#include "q12_op_0_V.act.h"
#include "q12_op_1_out.act.h"

void q12_op_0_V::trigger(StartVertex&& src) {
    _ds_group_hdl->emplace_data(brane::global_shard_id(), src.val);
    _ds_group_hdl->flush();
    _ds_group_hdl->receive_eos();
}

void q12_op_0_V::initialize() {
    auto builder = get_scope();
    _ds_group_hdl = new ds_group_t(builder, 1, 1);
}

void q12_op_0_V::finalize() {
    delete _ds_group_hdl;
}
