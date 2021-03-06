<?php
// $Id$

/**
 * @file
 *   Rules pre-configured rules for default drupal core behaviour.
 */

/**
 * Implementation of hook_rules_defaults().
 *
 * @return configured rules array
 */
function hbi_rules_defaults() {
  return array(
    'rules' =>
    array(

      'rules_30' =>
      array (
        '#type' => 'rule',
        '#set' => 'event_flag_flagged_fan',
        '#label' => 'User becomes a fan of',
        '#active' => 1,
        '#weight' => '0',
        '#categories' =>
        array (
          0 => 'heartbeat',
        ),
        '#status' => 'custom',
        '#conditions' =>
        array (
        ),
        '#actions' =>
        array (
          0 =>
          array (
            '#weight' => 0,
            '#info' =>
            array (
              'label' => 'Logs user activity for single users',
              'eval input' =>
              array (
                0 => 'uid_param',
                1 => 'uid_target_param',
                2 => 'nid_param',
                3 => 'nid_target_param',
                4 => 'message_id_param',
                5 => 'variables_param',
              ),
              'module' => 'heartbeat',
            ),
            '#name' => 'heartbeat_rules_default_action',
            '#settings' =>
            array (
              'uid_param' => '[flagging_user:uid]',
              'uid_target_param' => '',
              'nid_param' => '',
              'nid_target_param' => '[node:nid]',
              'message_id_param' => 'heartbeat_flag_fan_add',
              'variables_param' => '@user=|=[flagging_user:user-name-url]-|-@node_title=|=[node:title-link]-|-',
              '#eval input' =>
              array (
                'token_rules_input_evaluator' =>
                array (
                  'uid_param' =>
                  array (
                    0 => 'flagging_user',
                  ),
                  'uid_target_param' =>
                  array (
                    0 => 'node',
                  ),
                  'nid_param' =>
                  array (
                    0 => 'node',
                  ),
                  'nid_target_param' =>
                  array (
                    0 => 'node',
                  ),
                  'variables_param' =>
                  array (
                    0 => 'flagging_user',
                    1 => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),

    ),
  );
}