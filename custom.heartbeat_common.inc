<?php

/**
 * Helper function to theme user pictures
 */
function _theme_heartbeat_user_picture($user) {
  if (is_numeric($user) && $user) {
    $user = user_load($user);
  }
  $image = get_user_image($user->uid);
  
  /*if ($user && in_array('applicant', $user->roles)) {
    $image = l(theme('imagecache', 'thumb-mini', $user->picture), 'user/' . $user->uid, array('html' => TRUE));
  }
  else {
    $image = '<span class="user-default"></span>';
  }/**/
  // Provide a default image for anonymous users
	$output = l($image, 'user/'.$user->uid, array('html' => true, 'attributes' => array('class' => 'avatar'/*, 'title' => 'Go to '.$user->realname.'\'s profile'/**/)));
	return $output;
}

/**
 * Theme helper function for comment button
 */
function _theme_heartbeat_comment_button($object = NULL, $link_text = '') {

  $comment_count = isset($object->additions->comment_count) ? $object->additions->comment_count : 0;
  if ($comment_count <= 0)
    $link_title = 'comment';
  elseif ($comment_count == 1)
    $link_title = $comment_count . ' comment';
  elseif ($comment_count > 1)
    $link_title = $comment_count . ' comments';

  $arguments = array('attributes' => array());

  if ($comment_count > 0) {
    $class = 'heartbeat-comment-count commented';
  }
  else {
    $class = 'heartbeat-comment-count uncommented';
  }

  if ($link_text != '') {
    $class .= ' heartbeat-comment-button';
    $arguments['attributes']['title'] = $link_text;
  }
  else {
    $class .= ' heartbeat-comment-counter';
  }

  $arguments['attributes']['onclick'] = 'javascript:Drupal.heartbeat.toggleComments(this); return false;';

  return '<span class="' . $class . '">' . l($link_title, $_GET['q'], $arguments) . '</span>';
}

/**
 * Theme helper function for comment button
 */
function _theme_heartbeat_action_buttons($message, $args = array(), $node = NULL) {

  global $user;

  static $relationship_type = NULL;

  if (!isset($relationship_type)) {
    $all_relationships = user_relationships_types_load();
    // 1 is the friend relation type
    $relationship_type = $all_relationships[1];
  }

  $links = array();

  if (!empty($args['wrap'])) {
    $links[] = '<div class="heartbeat-action-buttons">';
  }

  $links[] = '<span class="heartbeat_times">' . theme('heartbeat_time_ago', $message) . '</span>';
  //$links[] = '<span class="heartbeat_intersection">  </span>';

  if (!empty($args['comment'])) {
    $links[] = _theme_heartbeat_comment_button($message, t('comment'));
  }

  if (!empty($args['member']) || !empty($args['group'])) {
    $links[] = '<span class="og">' . og_subscribe_link(node_load($message->nid_target)) . '</span>';
  }

  if (!empty($args['friend']) && $message->uid != $user->uid) {
    // For friendlist module
    //$friend = friendlist_ui_get_link('add', $message->uid, 1);
    //if (!empty($friend)) $links[] = '<span class="friend">' . $friend . '</span>';

    $links[] = '<span class="friend">' . theme('user_relationships_request_relationship_direct_link', user_load($message->uid), $relationship_type) . '</span>';
    //~ $links[] = '<span class="friend">' . l('Add as Contact', 'relationship/'.$message->uid.'/request') . '</span>';

  }

  if (!empty($args['wrap'])) {
    $links[] = '</div>';
  }

  return implode('', $links);
}

/**
 * Helper function to theme the icon left
 */
function _theme_heartbeat_icon_left($type, $message, $node = NULL) {

  $output = '';

  $output .= '<div class="icon-left-margin">';
  $output .= '<span class="icon-left ' . $type. '-icon">';
  $output .= theme('imagecache', 'thumb-mini', $type . '.png');
  $output .= '</span>';
  $output .= '</div>';

  return $output;
}
