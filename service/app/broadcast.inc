<?php

function processBroadcastMessage($v) {
	global $EmergencySettings;
	
	$db = new VDB('calls');
	$record = $db->getRecord($v->getCallID());

	$udb = new VDB('users');
	$user = $udb->getRecord(trim($record->user));


	$url = $EmergencySettings['media_path'];
	$url .= $user->id . '.wav';

	$results = generateEmergencyForwardList($v, true);

	$msg = "";
	foreach($results as $contact) {
		$v->tropo->message($url, array('to' => $contact['primary']));
	}

	// BUG: Doesn't hangup
	$v->tropo->hangup();
	$v->tropo->RenderJson();
}

?>
