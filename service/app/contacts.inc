<?php

function generateEmergencyForwardList($v, $emergencyOnly) {
	$db = new VDB('calls');
	$record = $db->getRecord($v->getCallID());

	$udb = new VDB('users');
	$user = $udb->getRecord(trim($record->user));

	$idx = 1;
	$results = array();
	foreach($user->contacts as $contact) {
		if ($contact->primary) {

			if (isset($contact->sip_phone)) {
				$primary = "sip:" . $contact->sip_phone;
			} else if (isset($contact->phone)) {
				$primary= "tel:" . $contact->phone;
			}

			@array_push($results, array('idx' => $idx, 'name' => $contact->name, 'phone' => $contact->phone, 'sip_phone' => $contact->sip_phone, 'primary' => $primary));
			
			++$idx;
		}
	}

	return $results;
}


function processEmergencyContactList($v) {
	$results = generateEmergencyForwardList($v, true);

	$msg = "";
	foreach($results as $contact) {
		$msg .= $contact['name'] . " on " . $contact['primary'] . ". ";
	}

	$v->tropo->say($msg);
}


function processEmergencyForwardList($v) {
	$results = generateEmergencyForwardList($v, true);

	$msg = "";
	foreach($results as $contact) {
		$msg .= $contact['idx'] . " is " . $contact['name'] . ". ";
	}

	// Set up options form zip code input
	$options = array("attempts" => 3, "bargein" => true, "choices" => "[1 DIGIT]", "name" => "contact", "timeout" => 5);
		
	// Ask the user for input, pass in options.
	$v->tropo->ask("To whom shall I forward you?  $msg", $options);
		
	// Tell Tropo what to do when the user has entered input, or if there is an error.
	$v->tropo->on(array("event" => "continue", "next" => $v->getStateURL("forward_emergency")));

}

			
$v->registerState('forward_emergency', function() {
global $v;

	$option = $v->getLastInput();

	$results = generateEmergencyForwardList($v, true);

	$msg = "";
	foreach($results as $contact) {
		if ($option == $contact['idx']) {
			$v->tropo->say("Forwarding call to " . $contact['name']);
			$new_number = $contact['primary'];
		}
	}

	if (isset($new_number)) {
		vlog("attempt to transfer to $new_number");
		$v->tropo->transfer(null, array("to" => $new_number, 'name' => 'Name', 'timeout' => 1000) );
	} else {
		$v->tropo->say("I can not find contact $option");
		generateMainMenu($v);
	}

	return $v->tropo->RenderJson();
});

?>
