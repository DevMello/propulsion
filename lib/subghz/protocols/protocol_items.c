#include "protocol_items.h"

const SubGhzProtocol* subghz_protocol_registry_items[] = {
    &subghz_protocol_gate_tx,
    &subghz_protocol_keeloq,
    &subghz_protocol_star_line,
    &subghz_protocol_nice_flo,
    &subghz_protocol_came,
    &subghz_protocol_faac_slh,
    &subghz_protocol_nice_flor_s,
    &subghz_protocol_came_twee,
    &subghz_protocol_came_atomo,
    &subghz_protocol_nero_sketch,
    &subghz_protocol_ido,
    &subghz_protocol_kia,
    &subghz_protocol_hormann,
    &subghz_protocol_nero_radio,
    &subghz_protocol_somfy_telis,
    &subghz_protocol_somfy_keytis,
    &subghz_protocol_scher_khan,
    &subghz_protocol_princeton,
    &subghz_protocol_raw,
    &subghz_protocol_linear,
    &subghz_protocol_secplus_v2,
    &subghz_protocol_secplus_v1,
    &subghz_protocol_megacode,
    &subghz_protocol_holtek,
    &subghz_protocol_chamb_code,
    &subghz_protocol_power_smart,
    &subghz_protocol_marantec,
    &subghz_protocol_bett,
    &subghz_protocol_doitrand,
    &subghz_protocol_phoenix_v2,
    &subghz_protocol_honeywell,
    &subghz_protocol_honeywell_wdb,
    &subghz_protocol_magellan,
    &subghz_protocol_intertechno_v3,
    &subghz_protocol_clemsa,
    &subghz_protocol_ansonic,
    &subghz_protocol_smc5326,
    &subghz_protocol_holtek_th12x,
    &subghz_protocol_linear_delta3,
    &subghz_protocol_dooya,
    &subghz_protocol_alutech_at_4n,
    &subghz_protocol_kinggates_stylo_4k,
    &ws_protocol_infactory,
    &ws_protocol_thermopro_tx4,
    &ws_protocol_nexus_th,
    &ws_protocol_gt_wt_02,
    &ws_protocol_gt_wt_03,
    &ws_protocol_acurite_606tx,
    &ws_protocol_acurite_609txc,
    &ws_protocol_acurite_986,
    &ws_protocol_lacrosse_tx,
    &ws_protocol_lacrosse_tx141thbv2,
    &ws_protocol_oregon2,
    &ws_protocol_oregon3,
    &ws_protocol_acurite_592txr,
    &ws_protocol_ambient_weather,
    &ws_protocol_auriol_th,
    &ws_protocol_oregon_v1,
    &ws_protocol_tx_8300,
    &ws_protocol_wendox_w6726,
    &ws_protocol_auriol_ahfl,
    &ws_protocol_kedsum_th,
    &subghz_protocol_pocsag,
    &tpms_protocol_schrader_gg4,
    &subghz_protocol_bin_raw,
    &subghz_protocol_mastercode,
    &subghz_protocol_x10,
};

const SubGhzProtocolRegistry subghz_protocol_registry = {
    .items = subghz_protocol_registry_items,
    .size = COUNT_OF(subghz_protocol_registry_items)};