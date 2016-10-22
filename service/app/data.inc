<?php

function getCountryCode($v, $name) {
	$result = file_get_contents("http://marquisdegeek.com/api/country/?name=$name&closest");

	$obj = json_decode($result);

	$best = PHP_INT_MAX;
	$code = $bestname = 'UK';
	foreach($obj as $c) {
		if ($c->accuracy < $best) {
			$code = $c->id;
			$best = $c->accuracy;
			$bestname = $c->name;
		}
	}

	if ($best != 0) {
		$v->tropo->say("I couldn't find a country called $name so have assumed you meant $bestname");
	}

	return $code;
}

function checkNumber($v, $type, $d) {
	if (!empty($d->all[0])) {
    	$v->tropo->say("$type on " . $d->all[0]);
	}
	if (!empty($d->gsm[0])) {
    	$v->tropo->say("$type with mobile on " . $d->gsm[0]);
	}
	if (!empty($d->fixed[0])) {
    	$v->tropo->say("$type on landline on " . $d->fixed[0]);
	}
}


function getEmergencyData($v) {
	$db = new VDB('calls');
	$record = $db->getRecord($v->getCallID());

	$udb = new VDB('users');
	$user = $udb->getRecord(trim($record->user));

	$location = getLocationFromCalendar($v);
	$location = str_replace('\\,', ',', $location);
	$address = split(',', $location);
	$country = $address[count($address) - 1];
	$country = trim($country);

	$country = getCountryCode($v, $country);

	$result = file_get_contents("http://emergencynumberapi.com/api/country/" . $country);
	$obj = json_decode($result);

	if ($obj->data->member_112) {
		$v->tropo->say("For emergency phone calls use 112");
	}

	checkNumber($v, "dispatch", @$obj->data->dispatch);
	checkNumber($v, "ambulance", @$obj->data->ambulance);
	checkNumber($v, "fire", @$obj->data->fire);
	checkNumber($v, "police", @$obj->data->police);

	if (isset($user->personal_details->passport_number)) {
		$v->tropo->say("Passport number is "  . $user->personal_details->passport_number);
	}

	if (isset($user->personal_details->bank)) {
		$v->tropo->say("Your bank is "  . $user->personal_details->bank);
	}

	if (isset($user->travel->embassy)) {
		$v->tropo->say("The nearest embassy is "  . $user->travel->embassy->address);
	}
}


function getLocationFromCalendar($v) {

	$db = new VDB('calls');
	$record = $db->getRecord($v->getCallID());

	$udb = new VDB('users');
	$user = $udb->getRecord(trim($record->user));

	$url = $user->calendar_uri;

	$ical   = new ICal($url);
	$events = $ical->events();

	$nowtime = time();
	foreach ($events as $event) {
		$startTime = $ical->iCalDateToUnixTimestamp($event['DTSTART']);
		$endTime = $ical->iCalDateToUnixTimestamp($event['DTEND']);

		if ($nowtime >= $startTime && $nowtime < $endTime) {
			$info = @$event['SUMMARY'];
			if (isset($event['LOCATION'])) {
				return @$event['LOCATION'];
			}
		}
	}

	return "";
}


function getNextCalendar($v) {
	$v->tropo->say("Your calendar ");

	$db = new VDB('calls');
	$record = $db->getRecord($v->getCallID());

	$udb = new VDB('users');
	$user = $udb->getRecord(trim($record->user));

	$url = $user->calendar_uri;

	$ical   = new ICal($url);
	$events = $ical->events();
	$end_message = "You have no events today.";

	$nowtime = time();
	foreach ($events as $event) {
		$startTime = $ical->iCalDateToUnixTimestamp($event['DTSTART']);
		$endTime = $ical->iCalDateToUnixTimestamp($event['DTEND']);

		if ($nowtime >= $startTime && $nowtime < $endTime) {
			$info = @$event['SUMMARY'];
			if (isset($event['LOCATION'])) {
				$info .= "at " . @$event['LOCATION'];
			}

			if (isset($event['DESCRIPTION'])) {
				$info .= @$event['DESCRIPTION'];
			}
			$info = str_replace('\\', '', $info);

			$v->tropo->say("Todays event: $info");
			$end_message = "";
		}
	}

	$v->tropo->say($end_message);
}

?>
