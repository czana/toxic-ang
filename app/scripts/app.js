'use strict';

/**
 * @ngdoc overview
 * @name toxicApp
 * @description
 * # toxicApp
 *
 * Main module of the application.
 */
angular
  .module('toxicApp', [
    'ngAnimate',
    'ngCookies',
    'ngResource',
    'ngRoute',
    'ngSanitize',
    'ngTouch'
  ])
  .config(function ($routeProvider) {
    $routeProvider
      .when('/', {
        templateUrl: 'views/main.html',
        controller: 'MainCtrl'
      })
      .otherwise({
        redirectTo: '/'
      });
  });
