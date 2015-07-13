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
    var counter = null;

  	$scope.rooms = [];

    $scope.onCount = function() {
      for(var i = 0; i < $scope.rooms.length; i++) {
        $scope.rooms[i].time += 1000;
      }

      counter = $timeout($scope.onCount, 1000);
    }

  	roomsRef.on('child_added', function(snapshot) {
  		$timeout(function() {
  			var snapshotVal = snapshot.val();

  			$scope.rooms.push({
  				empty: snapshotVal.empty,
          time: 0,
  				key: snapshot.key()
  			});

        if(!counter)
          counter = $timeout($scope.onCount, 1000);
  		});
  	});

  	roomsRef.on('child_changed', function(snapshot) {
  		$timeout(function() {
  			var snapshotVal = snapshot.val();
  			var room = $.grep($scope.rooms, function(room) { return room.key == snapshot.key(); })[0];
  			room.empty = snapshotVal.empty;
        room.time = 0;
        $scope.updateFavicon();
  		});
  	});

    $scope.updateFavicon = function() {
      var left = ($scope.rooms[0].empty == true) ? 1 : 0;
      var right = ($scope.rooms[1].empty == true) ? 1 : 0;
      favicon.change('images/favicon/' + left + '' + right + '.ico');
    }

  	$scope.toggleRoom = function(room) {
  		roomsRef.child(room.key).update({
  			empty: !room.empty
  		});
  	};

    $scope.getClass = function(room) {
      return (room.empty == true) ? 'empty' : 'busy';
    };

    $scope.getRoomPosition = function(room) {
      return (room.key == 1) ? 'left' : 'right';
    };
  });
