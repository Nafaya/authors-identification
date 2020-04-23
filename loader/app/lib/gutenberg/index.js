const Promise = require('bluebird');
const _ = require('underscore');
const path = require('path');
const url = require('url');
const fs = require('fs-extra');
const Browser = require('nafaya-browser');

function Gutenberg(config){
    //this.useMirrors = config.useMirrors || false;
    this.browser = new Browser({timeout:120000,gzip:true}) ;
    this.siteDomain = 'https://www.gutenberg.org/';
    this.library = config.library ;
}
Gutenberg.prototype.init = async function(){
    await this.library.init();
};

Gutenberg.prototype.processListing = async function(urlListingPath, { numberOfStreams=1, jsonPatchSize=100, startFromId } = {}){
    const self = this ;
    let listing = await this.loadListing(urlListingPath);
    const total_amount = listing.length;
    console.log('Total amount: '+total_amount) ;
    if(startFromId){
        let index = listing.findIndex((bookData)=>bookData.id===startFromId);
        if(index!==-1)listing = listing.slice(index);
    }
    let allowed_errors = 0 ;
    async function processWithAttempts(bookData, num_attempts_left=10){
        return await self.processBook(bookData).then(null,function(e){
            console.log(e);
            //console.log(`|${e.message}|`);
            if(e.message==='Response has status code 403'
                || e.message==='No title.'
                || e.message==='Not txt.') {
                //console.log('hi');
                allowed_errors++;
                return;
            }
            if(e.message==='Response has status code 404') {
                console.log(bookData) ;
                allowed_errors++;
                return;
            }
            if(!num_attempts_left || e.message==='Wrong format.') {
                console.log(bookData) ;
                throw e;
            }
            else return processWithAttempts(bookData, num_attempts_left-1);
        })
    }
    //return Promise.each(listing, function(bookData){
    //    return processWithAttempts(bookData, 10);
    //});
    let saved_amount = 0;
    async function func(){
        if(!listing.length) return ;
        let temp_listing = listing.splice(0,numberOfStreams);
        return Promise.all(temp_listing.map(async function(bookData){
            await processWithAttempts(bookData, numberOfStreams);
        })).then(function(){
            console.log(`Processed ${total_amount-listing.length} of ${total_amount}`) ;
            console.log(self.library.newBooksList.length+' new books in library.') ;
        }).then(func)
    }
    await func();
    await this.library.saveNewList();
    return {
        total_amount_in_list:total_amount,
        saved_amount:saved_amount,
        books_in_library:_.keys(this.library.newBooksList).length,
        allowed_errors:allowed_errors
    }
};

Gutenberg.prototype.loadListing = async function(urlListingPath){
    const regexpafterbook2  = /\s+(\d+)[a-zA-Z\s]*$/;
    const regexpafterbook  = /^[a-zA-Z\s]*$/;
    const regexpafterbooknumber  = /^\s+(\d+)$/;
    const regexpstrbook = /^\s*((?:[^ \r\n\v\f\b\0]+[ ]{0,2})+)\s+(\d+)$/;
    const longtitle = /^\s*([^\r\n\v\f\b\0]+)$/;
    const subregexpstrbook = /, by ([\-\.\w \t​\u00a0\u1680​\u180e\u2000​\u2001\u2002​\u2003\u2004​\u2005\u2006​\u2007\u2008​\u2009\u200a​\u2028\u2029​​\u202f\u205f​\u3000āÀ-ÖØ-öø-ÿÃ¥Č]+)(?:\s+\{MP3 audio\))?\s*$/;
    const regexpstroption = /^\s+\[(\w+): (.+)\]\s*$/;
    let response = await this.browser.get(url.resolve(this.siteDomain, urlListingPath));

    let listing = response.body ;
    //skip header text and get rid of duplicated text
    listing = listing.split('<==LISTINGS==>')[1];
    if(!listing) throw new Error('Wrong format.');
    listing = listing.split(/TITLE and AUTHOR\s+(?:(?:EBOOK)|(?:ETEXT)) NO\./).slice(1);

    listing = listing.map((sublisting)=>{
        let results = [];
        //get rid of footer
        sublisting = sublisting.split('~ ~ ~ ~ Posting Dates')[0];
        if(!sublisting) throw new Error('Wrong format.');
        sublisting = sublisting.split('This is the file GUTINDEX')[0];
        if(!sublisting) throw new Error('Wrong format.');

        sublisting = sublisting.split(/\r?\n/);
        let i = 0;
        while(i<sublisting.length){
            //if(i>30) throw 'stop.'
            //console.log('|'+sublisting[i]+'|') ;
            if(sublisting[i]==='Corrections made and illustrations added to the html file.') {
                i++;
                continue;
            }
            if(sublisting[i].slice(0,10)===' '.repeat(10)) {
                console.log('Early end,') ;
                return [];
            }
            if(!sublisting[i].length) { i++; continue; }
            let title = '';
            let match ;

            while(true){
                //console.log('|'+sublisting[i]+'|') ;
                /*let temp = sublisting[i].slice(78);
                match = regexpafterbook.exec(temp);
                if(!match) throw new Error('Wrong format.');
                sublisting[i] = sublisting[i].slice(0,78);
                if(match = regexpafterbooknumber.exec(sublisting[i])) {
                    match[1] = sublisting[i].slice(0,-match[1].length).trim();
                    match[2] = match[1];
                    break;
                }*/
                if(match = regexpafterbook2.exec(sublisting[i])) {
                    match[2] = match[1];
                    match[1] = sublisting[i].slice(0,-match[0].length).trim();
                    break;
                }
                if(sublisting[i]===''){
                    break;
                }
                match = longtitle.exec(sublisting[i]);
                if(!match) throw new Error('Wrong format.');
                if(match[1]) title += match[1] +'\r\n';
                i++;
            }
            if(!match) continue;
            let bookData = {
                id: match[2]
            };
            title += match[1].trim();

            i++;
            let optionsstr = '';
            while(sublisting[i] && sublisting[i].length){
                if(sublisting[i].length>=78 && (match = regexpafterbook2.exec(sublisting[i]))) break;
                if(optionsstr.length===0 && sublisting[i].trim()[0]!=='[') {
                    title+='\r\n'+sublisting[i].trim();
                } else {
                    optionsstr += sublisting[i];
                }
                i++;
            }

            match = subregexpstrbook.exec(title);
            if(match){
                let author = match[1];
                title = title.slice(0,-match[0].length);
                bookData.author = author;
            }
            bookData.title = title;

            let options = {};
            [... optionsstr.matchAll(regexpstroption)].forEach(function(match){
                options[match[1]] = match[2];
            });

            bookData.options = options;
            if(!bookData.id){
                //console.log(sublisting.slice(i-10,i))
                //console.log(bookData)
                console.log('No id.') ;
                continue;
            }
            //console.log(bookData)
            results.push(bookData);
        }
        return results;
    });

    return _.flatten(listing);
};

Gutenberg.prototype.processBook = async function({ id }){
    if(this.library.hasBook(id)) return {};

    let page = this.browser.page();
    let response = await page.open(url.resolve(this.siteDomain, `files/${id}`, ));
    if(response.statusCode!==200) throw new Error(`Response has status code ${response.statusCode}`)
    let { $ } = response;
    let txt_el = $('tr:has(>td>img[alt=\\[TXT\\]]) td a[href]');
    if(txt_el && txt_el.length){
        let fileName = $(txt_el).attr('href');
        if('readme_warning'===fileName.split('.')[0]) throw new Error('Not txt.');
        let book = await this.loadAndParseBookTxt(id, $(txt_el).attr('href'));
        return await this.library.saveBook(book);
    }else throw new Error('Not txt.');
};

Gutenberg.prototype.loadAndParseBookTxt = async function(id, fileName){
    //console.log(id,fileName) ;
    //console.log(url.resolve(this.siteDomain, `files/${id}/${fileName}`)) ;
    let response = await this.browser.get(url.resolve(this.siteDomain, `files/${id}/${fileName}`));
    let text = response.body;
    let header = null;

    if(text.indexOf('We need your donations more than ever!')!==-1) {
        console.log('Not good case.');
        return {
            id,
            text,
            title:null, authors: ['!Unknown'], translator:null, release_date:null, last_updated:null, language:null, encoding:null
        }
    }

    let index = text.search(/\*\*\*\s?START OF/);
    header = text.slice(0,index);

    text = text.slice(index);
    index = text.indexOf('\r\n');
    text = text.slice(index);

    /*
    index = header.indexOf('\r\n\r\n');
    header = header.slice(index+4).trim();

    index = header.indexOf('\r\n\r\n');
    header = header.slice(index+4).trim();

    */
    header = header.split(/\r?\n/);
//console.log(header);
    let match ;
    let row=header.shift();
    let params = {};
    let parsed_first_line = false;
    while(true){
        //console.log('row');
        //console.log(row);
        if(!row.length){
            if(header.length) {
                row = header.shift();
                continue;
            }else break;
        }
        match = /^([\w ]+): (.+)/.exec(row);
        if(!match) if(parsed_first_line)throw new Error('Wrong format.') ; else {row = header.shift()||''; continue;}

        let value = match[2].trim();
        let offset = match[1].length+2;
        while((row = header.shift() || '') && row.length && ((new RegExp(`^[ ]{${offset}}$`)).exec(row.slice(0,offset)) || row[0]==='[')) {
            value+='\r\n'+row.trim() ;
        }
        params[match[1].trim()] = value;
    }
    let authors=null ;
    if(match = /((?:\r\n){1,}Produced by )/.exec(text)) {
        text = text.slice(match[1].length);
        index = text.indexOf(' and ');
        let anotherindex = text.search(new RegExp(`(?:\r?[\n${String.fromCharCode(13,10)}]){3,}`));
        if(index===-1 || index>anotherindex) index = anotherindex ;
        if(index===-1) throw new Error('Wrong format.') ;
        //authors=text.slice(0,index).split(', ').map(function(str){return str.trim()});
    }else if(match = /((?:\r\n){3,}Transcribed from )/.exec(text)) {
        text = text.slice(match[1].length);
    }else if(match = /((?:\r\n){3,}E-text prepared by )/.exec(text)) {
        text = text.slice(match[1].length);
    }else {
        console.log('No subtext'+id);
    }
    text = text.slice(text.indexOf('\r\n\r\n\r\n\r\n')+8).trim();
    if(params['Author']) authors = _.flatten(params['Author'].split(',').map((str) => str.trim().split(/\sand\s/).map((str) => str.trim() )));


    let title=params['Title']||null,
        translator=params['Translator']||null,
        release_date=params['Release Date']||null,
        last_updated=params['Last Updated']||null,
        language=params['Language']||null,
        encoding=params['Character set encoding']||null;

    if(!title) {
        throw new Error('No title.') ;
    }
    //if(!authors) throw new Error('Wrong format.') ;
    //if(!translator) throw new Error('Wrong format.') ;
    //if(!release_date) throw new Error('Wrong format.') ;
    //if(!last_updated) throw new Error('Wrong format.') ;
    //if(!language) throw new Error('Wrong format.') ;
    //if(!encoding) throw new Error('Wrong format.') ;
    return {
        id,
        text,
        title, authors, translator, release_date, last_updated, language, encoding, length: text.length
    }
};

module.exports = Gutenberg ;
