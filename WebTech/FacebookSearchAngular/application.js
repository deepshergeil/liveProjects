var app = angular.module('fbSearch', ['ngAnimate']);

app.controller('myCtrl',function($scope, $http, $window) {

    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(function(position){
            $scope.lat = position.coords.latitude;
            $scope.long = position.coords.longitude;
        });
    }

     $scope.userAppend={};
     $scope.pageAppend={};
     $scope.eventAppend={};
     $scope.placeAppend={};
     $scope.groupAppend={};
    $scope.favTab=false;
    $scope.message=false;
    $scope.story=false;
    /*
     if (navigator.geolocation) {
     navigator.geolocation.getCurrentPosition($scope.showPosition,$scope.showError);
     }
     else {
     $scope.error = "Geolocation is not supported by this browser.";
     }
     $scope.showPosition = function (position) {
     $scope.lat = position.coords.latitude;
     $scope.lng = position.coords.longitude;
     }
     */
    $scope.id=1; //By default user is true
    $scope.tableShow=true;

    //  $scope.activeProgressbar=true;

    $scope.isSet = function(id) {       //true or false
        if($scope.id == id)
        {
            return true;
        }
        else
            return false;
    }

    $scope.clickSetTab=function(id)  //set tab on click
    {
        $scope.id=id;
        if(id==6)
        {
            $scope.favTab=true;
        }
        else
            $scope.favTab=false;

        $scope.setData($scope.id);
        if($scope.tableDetail==true)
        {
            $scope.tableDetail=false;
            $scope.tableShow=true;
        }

    }

    $scope.backButton=function()
    {
        console.log("inside back");
        $scope.tableDetail=false;
        $scope.setData($scope.id);
    }

    $scope.submit = function () {
        var keyword = $("#searchText").val();
        console.log("valueis"+keyword);
        if(!keyword) {
            $('#searchText').tooltip('enable');
            $('#searchText').tooltip('show');
            return;
        }
        else {
            $('#searchText').tooltip('disable');
        }
        $scope.showTableWait();
        $scope.activeProgressbar=true;
        var lat= $scope.lat;
        var long=$scope.long;
        var url = 'http://www.sodium-task-95910.appspot.com/?keyword';
        console.log(keyword);
        $http({
            method:'GET',
            url:"http://www.sodium-task-95910.appspot.com/?",
            params:{"data": keyword, "type": "user"}
        }).then(function successCallback(response) {
            $scope.userData = response.data.data;
            $scope.userPage= response.data.paging;
            console.log(response.data.data);
            console.log(response.data.paging);
            if( !$scope.userPage)
            {
                $scope.userAppend['hasNext']=false;
                $scope.userAppend['hasPrev']=false;
            }
            else {
                $scope.userAppend = $scope.append($scope.userPage, "user");
                console.log(response.data.data);
            }
            angular.forEach($scope.userData, function(entry) {
                entry.isFav = $scope.isStarred(entry.id);
            });
            $http({
                method:'GET',
                url:"http://www.sodium-task-95910.appspot.com/?",
                params:{"data": keyword, "type": "page"}
            }).then(function successCallback(response) {
                $scope.pageData = response.data.data;
                console.log(response.data.data);
                $scope.pagePage=response.data.paging;
                if( !$scope.pagePage)
                {
                    $scope.pageAppend['hasNext']=false;
                    $scope.pageAppend['hasPrev']=false;
                }
                else {
                    $scope.pageAppend=$scope.append($scope.pagePage,"page");
                    console.log(response.data.paging);
                }
                angular.forEach($scope.pageData, function(entry) {
                    entry.isFav = $scope.isStarred(entry.id);
                });
                $http({
                    method:'GET',
                    url:"http://www.sodium-task-95910.appspot.com/?",
                    params:{"data": keyword, "type": "event"}
                }).then(function successCallback(response) {
                    $scope.eventData = response.data.data;
                    console.log(  response.data.paging );
                    $scope.eventPage=response.data.paging;
                    if( !$scope.eventPage)
                    {
                        $scope.eventAppend['hasNext']=false;
                        $scope.eventAppend['hasPrev']=false;
                    }
                    else {
                        $scope.eventAppend=$scope.append($scope.eventPage,"event");
                        console.log(response.data.paging);
                    }
                    angular.forEach($scope.eventData, function(entry) {
                        entry.isFav = $scope.isStarred(entry.id);
                    });
                    //    console.log( response.data );
                    $http({
                        method:'GET',
                        url:"http://www.sodium-task-95910.appspot.com/?",
                        params:{"data": keyword, "type": "place","lat": lat,"long":long}
                    }).then(function successCallback(response) {
                        $scope.placeData = response.data.data;
                        console.log( response.data.paging );
                        $scope.placePage=response.data.paging;
                        if( !$scope.placePage)
                        {
                            $scope.placeAppend['hasNext']=false;
                            $scope.placeAppend['hasPrev']=false;
                        }
                        else {
                            $scope.placeAppend=$scope.append($scope.placePage,"place");
                            console.log(response.data.paging);
                        }
                        angular.forEach($scope.placeData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        //   console.log( response.data );
                        $http({
                            method:'GET',
                            url:"http://www.sodium-task-95910.appspot.com/?",
                            params:{"data": keyword, "type": "group"}
                        }).then(function successCallback(response) {
                            $scope.groupData = response.data.data;
                            console.log(response.data.paging);
                            $scope.groupPage=response.data.paging;
                            if( !$scope.groupPage)
                            {
                                $scope.groupAppend['hasNext']=false;
                                $scope.groupAppend['hasPrev']=false;
                            }
                            else {
                                $scope.groupAppend=$scope.append($scope.groupPage,"group");
                                console.log(response.data.paging);
                            }
                            angular.forEach($scope.groupData, function(entry) {
                                entry.isFav = $scope.isStarred(entry.id);
                            });
                            /*Rendering Starts*/
                            $scope.setData($scope.id);
                        });
                    });
                });
            });
        });
    }

    $scope.showTableWait=function()
    {
        $scope.tableDetail=false;
        $scope.noShow=false;
        $scope.tableShow=false;
    }

    $scope.fbClick=function(showData)
    {
        console.log(showData);
        FB.getLoginStatus(function (response) {
            if (response.status === 'connected') {
                console.log('connected');
                $scope.postFB(showData);
            } else {
                // the user isn't logged in to Facebook.
                FB.login(function (response) {
                    if (response.status === 'connected') {
                        $scope.postFB(showData);
                    } else {
                        alert('Not Posted');
                    }
                });
            }
        });
    }

    $scope.postFB=function(showData){
        FB.ui({
            app_id:'1821503231447458',
            method: 'feed',
            name: showData.passData.name,
            link: window.location.href,
            picture: showData.passData.picture.data.url,
            caption: "FB SEARCH FROM USC CSCI571",
            display: 'popup'
        }, function (response) {
            if (response && !response.error_message) {
                alert('Posted Successfully');
            } else {
                alert('Not Posted');
            }
        });
    }


    $scope.append=function(pagingData,type) {
        var append = {};
        if (type == "user") {
            var nextLink = pagingData.next;
            var prevLink = pagingData.previous;
            if (nextLink) {
                nextLink = nextLink.split('&');
                angular.forEach(nextLink, function (obj) {
                    if (obj.indexOf("offset") != -1) { //if present
                        append['nextOffset'] = obj.split('=')[1];
                    }
                    if (obj.indexOf("__after_id") != -1) {
                        append['afterId'] = obj.split('=')[1];
                    }
                });
                if (prevLink) {
                    console.log(prevLink);
                    prevLink = prevLink.split('&');
                    angular.forEach(prevLink, function (obj) {
                        if (obj.indexOf("offset") != -1) { //if present
                            append['prevOffset'] = obj.split('=')[1];
                        }
                        if (obj.indexOf("__before_id") != -1) {
                            append['beforeId'] = obj.split('=')[1];
                        }
                    });

                }

            }

        }
        else  //other Tabs
        {
            if (pagingData.cursors) {
                append['after'] = pagingData.cursors.after;
                append['before'] = pagingData.cursors.before;

            }
        }
        append['hasNext'] = pagingData.hasOwnProperty('next');
        append['hasPrev'] = pagingData.hasOwnProperty('previous');

        return append;

    }
    $scope.setData = function (id) {

        $scope.activeProgressbar=false;
        $scope.tableShow=true;
        console.log("inside setData");
        if (id == 1) {
            $scope.items = $scope.userData;
            $scope.pagination = $scope.userAppend;
        }
        else {
            if (id == 2) {
                $scope.items = $scope.pageData;
                $scope.pagination = $scope.pageAppend;
            }
            else if (id == 3) {
                $scope.items = $scope.eventData;
                $scope.pagination = $scope.eventAppend;
            }
            else if (id == 4) {
                $scope.items = $scope.placeData;
                $scope.pagination = $scope.placeAppend;
            }
            else if (id == 5) {
                $scope.items = $scope.groupData;
                $scope.pagination = $scope.groupAppend;
            }
            else if(id==6)
            {
                $scope.items = angular.fromJson(localStorage.getItem("favouriteList"));
            }
        }

        if($scope.items && $scope.items.length == 0) {
            $scope.noShow = true;
        }
        else {
            $scope.noShow = false;
        }

    }

    $scope.showDetail = function (item) {

        $scope.showLoaderDetails();
        var detailData = {};
        console.log(item.id);
        $http({
            method: 'GET',
            url: "http://www.sodium-task-95910.appspot.com/?",
            params: {"id": item.id,}
        }).then(function successCallback(response) {
            console.log(response.data);
            if (response.data.albums)
            {
                detailData.hasAlbum=true;
            }
            else
            {
                detailData.hasAlbum=false;
            }
            if(detailData.hasAlbum)
            {
                console.log("albums present");
                detailData.albums = response.data.albums.data;
                angular.forEach(detailData.albums, function (values) {
                    if (values.photos) { //photos present for particular album
                        console.log("photos present");
                        angular.forEach(values.photos.data, function (photo) { //extracting photos of albums
                            console.log(photo.images[0].source);
                            photo.url= photo.images[0].source;

                        });
                    }
                    else {

                        //No photos Present in album
                    }
                });

            }
            if(response.data.posts)
            {
                if(response.data.posts.data.length!=0) {
                //    console.log("post present");
                    detailData.hasPost = true;
                    angular.forEach(response.data.posts.data, function (message) { //extracting photos of albums
                       if(message.message)
                           message.message=message.message;
                       else
                           if(message.story)
                               message.message=message.story;
                       else
                           message.message="";

                    });
                    detailData.posts=response.data.posts.data;
                }
            }
            else
            {
                detailData.hasPost=false;
            }
            detailData.passData=item;
            $scope.showData=detailData;
            $scope.removeLoaderDetails();

        });//success callback ends

    }

    $scope.showLoaderDetails=function(){
        $scope.tableShow=false;
        $scope.tableDetail=true;
        $scope.albumProgressbar=true;
        $scope.postProgressbar=true;
    }

    $scope.removeLoaderDetails=function(){
        $scope.albumProgressbar=false;
        $scope.postProgressbar=false;
    }

    $scope.nextPage = function () {
        console.log("inside nextbutton");
        var appendString = "";
        switch ($scope.id) {
            case 1:
                $scope.keyType = 'user';
                break;
            case 2:
                $scope.keyType = 'page';
                break;
            case 3:
                $scope.keyType = 'event';
                break;
            case 4:
                $scope.keyType = 'place';
                break;
            case 5:
                $scope.keyType = 'group';
                break;
        }
        if ($scope.id === 1) {
            appendString += "&offset=" + $scope.pagination.nextOffset + "&__after_id=" + $scope.pagination.afterId;
        }
        else {
            appendString += "&after=" + $scope.pagination.after;
        }
        console.log(appendString);
        var keyword1 = $("#searchText").val();
        console.log(keyword1+"&&&&&&"+$scope.keyType);

        url1='https://graph.facebook.com/v2.8/search?q='+keyword1+'&type='+$scope.keyType+'&fields=id,name,picture.width(700).height(700)&access_token=EAAZA4pcURUaIBAEVN4YdZCpnhQGuu6Uql7yDdaEkaUD0Hd8Mll3XRMAS61DQpRWSD1akeNMU3z4Ejqm4XHZCCyDp4IZBdfzYXW6SHEZBj6KF3PthiUC2m9rfJGPVecCSTGYs08JJ1NbbhPx1zQUAw2fzSQksGkrgZD'+appendString;
        $http.get(url1)
            .then(function (response) {
                console.log(response.data.data);
                data=response.data;
                switch ($scope.id) {
                    case 1:
                        $scope.userData = data.data;
                        $scope.userPage = data.paging;
                        if (!$scope.userPage) {
                            $scope.userAppend['hasNext'] = false;
                            $scope.userAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.userAppend = $scope.append($scope.userPage, "user");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.userData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });

                        break;
                    case 2:
                        $scope.pageData = data.data;
                        $scope.pagePage = data.paging;
                        if (!$scope.pagePage) {
                            $scope.pageAppend['hasNext'] = false;
                            $scope.pageAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.pageAppend = $scope.append($scope.pagePage, "page");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.pageData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });

                        break;
                    case 3:
                        $scope.eventData = data.data;
                        console.log(data.paging);
                        $scope.eventPage = data.paging;
                        if (!$scope.eventPage) {
                            $scope.eventAppend['hasNext'] = false;
                            $scope.eventAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.eventAppend = $scope.append($scope.eventPage, "event");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.eventData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });

                        break;
                    case 4:
                        $scope.placeData = data.data;
                        console.log(data.paging);
                        $scope.placePage = data.paging;
                        if (!$scope.placePage) {
                            $scope.placeAppend['hasNext'] = false;
                            $scope.placeAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.placeAppend = $scope.append($scope.placePage, "place");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.placeData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                    case 5:
                        $scope.groupData = data.data;
                        console.log(data.paging);
                        $scope.groupPage = data.paging;
                        if (!$scope.groupPage) {
                            $scope.groupAppend['hasNext'] = false;
                            $scope.groupAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.groupAppend = $scope.append($scope.groupPage, "group");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.groupData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                }
                $scope.setData($scope.id);

            });

    }


    $scope.previousPage = function () {
        var appendString = "";
        switch ($scope.id) {
            case 1:
                $scope.keyType = 'user';
                break;
            case 2:
                $scope.keyType = 'page';
                break;
            case 3:
                $scope.keyType = 'event';
                break;
            case 4:
                $scope.keyType = 'place';
                break;
            case 5:
                $scope.keyType = 'group';
                break;
        }
        if ($scope.id === 1) {
            appendString += "&offset=" + $scope.pagination.prevOffset + "&__before_id=" + $scope.pagination.beforeId;
        }
        else {
            appendString += "&before=" + $scope.pagination.before;
        }
        console.log(appendString);
        var keyword1 = $("#searchText").val();
        url1='https://graph.facebook.com/v2.8/search?q='+keyword1+'&type='+$scope.keyType+'&fields=id,name,picture.width(700).height(700)&access_token=EAAZA4pcURUaIBAEVN4YdZCpnhQGuu6Uql7yDdaEkaUD0Hd8Mll3XRMAS61DQpRWSD1akeNMU3z4Ejqm4XHZCCyDp4IZBdfzYXW6SHEZBj6KF3PthiUC2m9rfJGPVecCSTGYs08JJ1NbbhPx1zQUAw2fzSQksGkrgZD'+appendString;
        $http.get(url1)
            .then(function (response) {
                console.log(response.data.data);
                data=response.data;
                switch ($scope.id) {
                    case 1:
                        $scope.userData = data.data;
                        $scope.userPage = data.paging;
                        if (!$scope.userPage) {
                            $scope.userAppend['hasNext'] = false;
                            $scope.userAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.userAppend = $scope.append($scope.userPage, "user");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.userData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                    case 2:
                        $scope.pageData = data.data;
                        $scope.pagePage = data.paging;
                        if (!$scope.pagePage) {
                            $scope.pageAppend['hasNext'] = false;
                            $scope.pageAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.pageAppend = $scope.append($scope.pagePage, "page");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.pageData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                    case 3:
                        $scope.eventData = data.data;
                        console.log(data.paging);
                        $scope.eventPage = data.paging;
                        if (!$scope.eventPage) {
                            $scope.eventAppend['hasNext'] = false;
                            $scope.eventAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.eventAppend = $scope.append($scope.eventPage, "event");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.eventData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                    case 4:
                        $scope.placeData = data.data;
                        console.log(data.paging);
                        $scope.placePage = data.paging;
                        if (!$scope.placePage) {
                            $scope.placeAppend['hasNext'] = false;
                            $scope.placeAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.placeAppend = $scope.append($scope.placePage, "place");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.placeData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                    case 5:
                        $scope.groupData = data.data;
                        console.log(data.paging);
                        $scope.groupPage = data.paging;
                        if (!$scope.groupPage) {
                            $scope.groupAppend['hasNext'] = false;
                            $scope.groupAppend['hasPrev'] = false;
                        }
                        else {
                            $scope.groupAppend = $scope.append($scope.groupPage, "group");
                            console.log(data.paging);
                        }
                        angular.forEach($scope.groupData, function(entry) {
                            entry.isFav = $scope.isStarred(entry.id);
                        });
                        break;
                }
                $scope.setData($scope.id);

            });

    }


    $scope.addFav = function( item ) {
        var list = [];
        item.isFav = true;
        switch ($scope.id) {
            case 1:
                $scope.keyType = 'user';
                break;
            case 2:
                $scope.keyType = 'page';
                break;
            case 3:
                $scope.keyType = 'event';
                break;
            case 4:
                $scope.keyType = 'place';
                break;
            case 5:
                $scope.keyType = 'group';
                break;
        }
        var addItem = {'id': item.id, 'name': item.name, 'type': $scope.keyType, 'url': item.picture.data.url, 'isFav': true};
        var favouriteList = localStorage.getItem("favouriteList");

        if(favouriteList) {
            list = angular.fromJson(favouriteList); //parse
        }
        list.push(addItem);
        localStorage.setItem("favouriteList", JSON.stringify(list));
    }
    $scope.deleteFav = function( item ) {
        item.isFav= false;
        switch ($scope.id) {
            case 1:
                $scope.keyType = 'user';
                break;
            case 2:
                $scope.keyType = 'page';
                break;
            case 3:
                $scope.keyType = 'event';
                break;
            case 4:
                $scope.keyType = 'place';
                break;
            case 5:
                $scope.keyType = 'group';
                break;
            case 6:
                $scope.keyType = 'favourite';
        }
        var list = angular.fromJson(localStorage.getItem("favouriteList")); //parsing list
        angular.forEach(list, function(additem, index) {
            if(additem.id == item.id) {
                list.splice(index, 1);
                if($scope.keyType === "favourite") {
                    switch (additem.type) {
                        case "user" :
                            angular.forEach($scope.userData, function(entry, index) {
                                if(entry.id === item.id) {
                                    entry.isFav = false;
                                }
                            });
                            break;
                        case "page" :
                            angular.forEach($scope.pageData, function(entry, index) {
                                if(entry.id === item.id) {
                                    entry.isFav = false;
                                }
                            });
                            break;
                        case "event" :
                            angular.forEach($scope.eventData, function(entry, index) {
                                if(entry.id === item.id) {
                                    entry.isFav = false;
                                }
                            });
                            break;
                        case "place" :
                            angular.forEach($scope.placeData, function(entry, index) {
                                if(entry.id === item.id) {
                                    entry.isFav = false;
                                }
                            });
                            break;
                        case "group" :
                            angular.forEach($scope.groupData, function(entry, index) {
                                if(entry.id === item.id) {
                                    entry.isFav = false;
                                }
                            });
                            break;
                    }
                }
            }
        });
        localStorage.setItem("favouriteList", JSON.stringify(list));
        if($scope.id==6) {
            $scope.setData(6);
        }
    }

    $scope.isStarred=function(id){ //checking starred value on reloading
        var match=false;
        var list = angular.fromJson(localStorage.getItem("favouriteList"));
        angular.forEach(list, function(entry, index) {
            if(entry.id === id) {
                console.log("match found");
                match=true;
                return match;
            }
        });
        return match;
    }


    $scope.clear=function(){console.log("insideClear");
    $scope.id=1;
    $scope.tableShow=false;
    $scope.tableDetail=false;
    //$('#searchText').val(" ");
        $scope.search="";
    $scope.noShow=false;
    $scope.clearTabs();

    }
$scope.clearTabs=function()
    {
        $scope.userData="";
        $scope.pageData="";
        $scope.eventData="";
        $scope.placeData="";
        $scope.groupData="";

    }


});

