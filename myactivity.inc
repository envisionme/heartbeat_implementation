<?

/**
 * @file
 *   Relational heartbeat activity access object
 */

heartbeat_include('HeartbeatAccess');

/**
 * Class ConnectedHeartbeat
 * Concrete class to prepare messages for the current user
 * and all of his/her relations.
 *
 */
class MyActivity extends HeartbeatAccess {

  /**
   * proctected fuinction whoIsActor
   *   Add the heartbeat relations for the stream's active user.
   */
  protected function whoIsActor($account = NULL) {
    parent::whoIsActor($account);
    $this->stream->heartbeat_relations = heartbeat_get_related_uids($this->stream->uid);
  }

  /**
   * Override dressupMessages to change the query.
   * @see HeartbeatAccess::dressUpMessages().
   */
  public function dressUpMessages(HeartbeatParser $heartbeat) {

    if (!empty($this->stream->heartbeat_relations)) {
	    $viewed_user_uid = $this->_uid;
      // filter activity for related users and activity that was addressed to the current user.
      //$sql = " AND ((ua.uid in( %s ) AND ua.access >= 0) OR (ua.uid_target = %d AND ua.nid <> 0))";
	    $sql = '';
      $heartbeat->raw_messages = $this->resultSql($sql, array(implode(',', $this->stream->heartbeat_relations), $this->stream->uid));
	    $msg_array = array();

	    //$flag = flag_get_flag('follow_company') or die('no "follow_company" flag defined');

      //dpm($heartbeat->raw_messages);

	    foreach ($heartbeat->raw_messages as $message) {
        if (isset($message->nid_info['uid'])) {
  		    $msg_uid = $message->nid_info['uid'];

  		    $result = db_query('SELECT * FROM {flag_content} WHERE (content_id = "'. $msg_uid .'") AND (uid = "'. $viewed_user_uid .'") AND (fid = "16")');

  		    while ($new_object = db_fetch_object($result)) {
  			    $msg_array[] = $message;
  		    }

  		    if (sizeof(user_relationships_load($param = array("approved" => 1, "between" => array($viewed_user_uid, $msg_uid)))) > 0)
  			    $msg_array[] = $message;

          // Add status updates to heartbeat stream.
          if ((sizeof(user_relationships_load($param = array("approved" => 1, "between" => array($viewed_user_uid, $message->uid)))) > 0) && ($message->message_id == 'fb_status_update')) {
            $msg_array[] = $message;
          }
        }
	    }

	    $heartbeat->raw_messages = $msg_array;
    }
    else {
      $heartbeat->raw_messages = array();
    }

    return $heartbeat;
  }

  /**
   * Function to add a part of a sql to a query built by views UI
   *
   * @param object $view The view handler object by reference to add our part to the query
   */
  public function addViewQuery(&$view) {

    // Make the sql limited to the access
    $field = "$view->table_alias.$view->real_field";
    if (!empty($this->stream->heartbeat_relations)) {

      $view->query->set_where_group('OR', 'orgroup');
      $sql = "$field in( ". implode(',', $this->stream->heartbeat_relations) ." ) AND $view->table_alias.access >= 0 ";
      $view->query->add_where('orgroup', $sql);

      $sql = "$view->table_alias.uid_target = %d AND $view->table_alias.nid <> 0";
      $view->query->add_where('orgroup', $sql, $this->_uid);

    }
    else {
      $view->query->add_where('deny', "$view->table_alias.access < 0");
    }
  }
}
