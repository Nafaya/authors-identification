const yargs = require('yargs');

const argv = yargs
    .command('load', 'Load books', {
        listing: {
            description: 'uri listing which to load',
            alias: 'l',
            type: 'string',
            required:true
        },
        dir: {
            description: 'books directory',
            alias: 'd',
            type: 'string',
        },
        json: {
            description: 'path to json listing file',
            alias: 'j',
            type: 'string',
        },
        streamsNumber: {
            description: 'number of parallel streams',
            alias: 's',
            type: 'number',
        },
        patchSize: {
            description: 'save patches. Patch size.',
            alias: 'p',
            type: 'number',
        }
    })
    .help()
    .alias('help', 'h')
    .argv;

async function main(){
    if (argv._.includes('load')) {
        const path = require('path');
        var Library = require('../app/lib/library');
        const Gutenberg = require('../app/lib/gutenberg');

        var library = new Library({
            booksDirectory:path.resolve('../data/gutenberg/books'),
            catalogDirectory:path.resolve('../data/gutenberg/catalog')
        });
        const gutenberg = new Gutenberg({
            library
        });
        await gutenberg.init();
        gutenberg.processListing(argv.listing, { numberOfStreams: argv.streamsNumber || 10, jsonPatchSize: argv.patchSize || 10 }).then(function(result){
            console.log(result) ;
        })
    }
}

console.log(argv);
main();