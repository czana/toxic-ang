'use strict';

/**
 * @ngdoc function
 * @name toxicApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the toxicApp
 */
angular.module('toxicApp')
  .controller('MainCtrl', function ($scope, $timeout) {
  	var rootRef = new Firebase('https://brilliant-fire-9601.firebaseio.com/');
  	var roomsRef = rootRef.child('rooms');

  	$scope.rooms = [];

  	roomsRef.on('child_added', function(snapshot) {
  		$timeout(function() {
  			var snapshotVal = snapshot.val();

  			$scope.rooms.push({
  				name: snapshotVal.name,
  				empty: snapshotVal.empty,
  				key: snapshot.key()
  			});
  		});
  	});

  	roomsRef.on('child_changed', function(snapshot) {
  		$timeout(function() {
  			var snapshotVal = snapshot.val();
  			var room = $.grep($scope.rooms, function(room) { return room.key == snapshot.key(); })[0];
  			room.empty = snapshotVal.empty;
  		});
  	});

  	$scope.toggleRoom = function(room) {
  		roomsRef.child(room.key).update({
  			empty: !room.empty
  		});
  	};
  });
