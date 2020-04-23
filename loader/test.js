var path        = require('path') ;
var async        = require('async') ;
var _        = require('underscore') ;
var fs        = require('fs') ;
const Browser = require('nafaya-browser');
const browser = new Browser({timeout:120000,gzip:true}) ;
var Library = require('./app/lib/library');
var Gutenberg = require('./app/lib/gutenberg');

var library = new Library({
    booksDirectory:path.resolve('../data/gutenberg/books'),
    catalogDirectory:path.resolve('../data/gutenberg/catalog')
});
var gutenberg = new Gutenberg({
    library
});
async.series(
    [
        function(callback){
            console.log('started') ;
            callback();
        },
        /*function(callback){
            gutenberg.loadAndParseBookTxt('534','534-0.txt').then(function(res){
                console.log(res) ;
            })
        },*/
        function(callback){
            gutenberg.init().then(function(){callback();});
        },
        function(callback){
            console.log(_.keys(library.booksHash).length+' books in library.');
            callback();
        },
        /*function(callback){

            console.log('1') ;
            let authors_hash = {};
            console.log('2') ;
            library.forEach(async (bookData, list)=>{
                //console.log(list);
                if(bookData.authors && bookData.authors.length===1) bookData.authors.forEach(function(author){
                    authors_hash[author] = (authors_hash[author]||0)+1;
                });
            }).then(function(){
                console.log('3') ;
                let arr = _.keys(authors_hash).map((author)=>{
                    return {author, amount:authors_hash[author]}
                }).sort((a,b)=>{ return b.amount - a.amount })
                console.log(arr.slice(0,100));
                console.log(arr.slice(100));
            })
        },
        function(callback){
            console.log('end') ;
        },*/
        /*function(callback){
            library.aggregate();
        },*/
        /*function(callback){
            gutenberg.processBook({id:'1789'}).then(function(res){
                console.log(res) ;
            })
        },*/
        /*function(callback){
            gutenberg.processListing('dirs/GUTINDEX.1996', { numberOfStreams:30 }).then(function(result){
                console.log(result) ;
            })
        },*/
        /*function(callback){
            gutenberg.saveList().then(function(){
                console.log('end') ;
                //callback() ;
            })
        },*/
        function(callback){
            let y = 1996;
            function func(){
                console.log(y) ;
                return gutenberg.processListing('dirs/GUTINDEX.'+y,{ numberOfStreams:30 }).then(function(listing){
                    //console.log(listing.slice(0,10)) ;
                    console.log('end') ;
                    if(y<2019){
                        y++;
                        return func();
                    }
                })
            }
            func()
        },
        /*function(callback){
            let y = 1996;
            var sum = 0 ;
            function func(){
                console.log(y) ;
                return gutenberg.loadListing('dirs/GUTINDEX.'+y).then(function(listing){
                    sum += listing.length;
                    let id = parseInt(listing[0].id);
                    console.log('id = '+id);
                    console.log('sum = '+sum);
                    console.log('listing.length = '+listing.length);
                    console.log('id-sum = '+(id-sum));
                    console.log('end') ;
                    if(y<2019){
                        y++;
                        return func();
                    }
                })
            }
            func()
        },*/
        function(callback){
            gutenberg.loadListing('dirs/GUTINDEX.2000').then(function(listing){
                for(var i=0;i<listing.length;i++){
                    console.log(i+2377,listing[listing.length-1-i].id) ;
                }
                console.log(listing.length) ;
                console.log('end') ;
                //callback();
            })
        },
        function(callback){
            browser.get('https://www.gutenberg.org/dirs/GUTINDEX.2019').then(function(response){
                var listing = response.body ;
                listing = listing.split('<==LISTINGS==>')[1];
                var date = new Date();
                console.log(date) ;
                listing = listing.split(/TITLE and AUTHOR\s+EBOOK NO\./);
                listing = listing.map(function(sublisting){
                    sublisting = sublisting.split('~ ~ ~ ~ Posting Dates')[0];
                    return sublisting.split(/\r\n/);
                });

                var sublisting = listing[2] ;
                var i = 0;
                while(i<sublisting.length){
                    if(!sublisting.length) { i++; return; }

                }
                console.log(sublisting) ;
                console.log(listing.length) ;
                console.log((new Date()-date)) ;
                console.log(date) ;
                console.log(new Date()) ;

                /*listing = listing.split('\n');
                console.log(listing[0]) ;
                console.log(listing[1]) ;
                console.log(listing[2]) ;
                console.log(listing[3]) ;
                console.log(listing[4]) ;
                console.log(listing[4].length) ;*/
                callback();
            })
        },
    ],
    function(err, results){
        console.log('end') ;
    }
);