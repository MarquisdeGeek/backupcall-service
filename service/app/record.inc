<?php

function processRecordMessage($v) {
	global $EmergencySettings;

	$db = new VDB('calls');
	$record = $db->getRecord($v->getCallID());

	$udb = new VDB('users');
	$user = $udb->getRecord(trim($record->user));


	$url = $EmergencySettings['recording_callback_url'];
	$url .= "?user_id=" . $user->id;


	$v->tropo->record(array (
		'terminator' => '*',
    	'say' => 'Please record a message',
    	'url' => $url
       )
	);

	$v->tropo->RenderJson();
}

?>
