//
//  Document.m
//  get_ad2
//
//  Created by wuheshun on 5/23/13.
//  Copyright (c) 2013 wuheshun. All rights reserved.
//

#import "Document.h"
#include "pugixml.hpp"
#include <iostream>
#include "parse_data.hpp"
#include <iostream>



@implementation Document

- (id)init
{
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
    }
    NSData *data = [NSData dataWithContentsOfURL:[NSURL URLWithString:@"http://partner.applift.com/offers/offers.xml?api_key=AFFX1FHk7x39ewkWdJGcaAP3PxwROI"]];
    //    NSLog(@"%s", [data bytes]);
    
    pugi::xml_document doc;
    
    
    
    pugi::xml_parse_result result = doc.load_buffer([data bytes], [data length]);
    std::cout << "Load result: " << result.description() << std::endl;
    std::cout << "First node value: [" << doc.first_child().value() << "], node child value: [" << doc.child_value("node") << "]\n";
    
    std::cout << "First node value: [" << doc.first_child().name() << "], node child value: [" << doc.child_value("node") << "]\n";
    
    pugi::xml_node tools = doc.first_child();
    load_data2(tools);
//    tools = tools.first_child();
//    
//    offer_t offer;
//    for (; tools; tools = tools.next_sibling()) {
//        parse_data(offer, tools);
//    }

    return self;
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"Document";
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController
{
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
}

+ (BOOL)autosavesInPlace
{
    return YES;
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to write your document to data of the specified type. If outError != NULL, ensure that you create and set an appropriate error when returning nil.
    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
    NSException *exception = [NSException exceptionWithName:@"UnimplementedMethod" reason:[NSString stringWithFormat:@"%@ is unimplemented", NSStringFromSelector(_cmd)] userInfo:nil];
    @throw exception;
    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to read your document from the given data of the specified type. If outError != NULL, ensure that you create and set an appropriate error when returning NO.
    // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead.
    // If you override either of these, you should also override -isEntireFileLoaded to return NO if the contents are lazily loaded.
    NSException *exception = [NSException exceptionWithName:@"UnimplementedMethod" reason:[NSString stringWithFormat:@"%@ is unimplemented", NSStringFromSelector(_cmd)] userInfo:nil];
    @throw exception;
    return YES;
}

@end
