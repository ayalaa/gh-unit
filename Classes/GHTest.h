//
//  GHTest.h
//  GHKit
//
//  Created by Gabriel Handford on 1/18/09.
//  Copyright 2009. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//

typedef enum {
	GHTestStatusNone,
	GHTestStatusRunning,
	GHTestStatusFinished,
} GHTestStatus;

typedef struct {
	NSInteger runCount;
	NSInteger failureCount;
	NSInteger testCount;
} GHTestStats;

static __inline__ GHTestStats GHTestStatsMake(NSInteger runCount, NSInteger failureCount, NSInteger testCount) {
	GHTestStats stats;
	stats.runCount = runCount;
	stats.failureCount = failureCount; 
	stats.testCount = testCount;
	return stats;
}

#define NSStringFromGHTestStats(stats) [NSString stringWithFormat:@"%d/%d/%d", stats.runCount, stats.testCount, stats.failureCount]

@protocol GHTest <NSObject>

- (void)run;

- (NSString *)identifier;
- (NSString *)name;

- (NSTimeInterval)interval;
- (GHTestStatus)status;
- (GHTestStats)stats;

- (NSString *)backTrace;

@end

@protocol GHTestDelegate <NSObject>
- (void)testWillStart:(id<GHTest>)test;
- (void)testUpdated:(id<GHTest>)test source:(id<GHTest>)source;
- (void)testDidFinish:(id<GHTest>)test;
@end

@interface GHTest : NSObject <GHTest> {
	
	id<GHTestDelegate> delegate_; // weak
	
	id target_;
	SEL selector_;
	
	NSString *name_;
	GHTestStatus status_;
	NSTimeInterval interval_;
	BOOL failed_;
	
	GHTestStats stats_;
	
	// If errored
	NSException *exception_; 
	NSString *backTrace_;
	
}

- (id)initWithTarget:(id)target selector:(SEL)selector interval:(NSTimeInterval)interval exception:(NSException *)exception;

+ (id)testWithTarget:(id)target selector:(SEL)selector;
+ (id)testWithTarget:(id)target selector:(SEL)selector interval:(NSTimeInterval)interval exception:(NSException *)exception;

// Loads all the tests from the specified target
+ (NSArray *)loadTestsFromTarget:(id)target;

@property (readonly) id target;
@property (readonly) SEL selector;
@property (readonly) NSString *name;
@property (readonly) NSTimeInterval interval;
@property (readonly) NSException *exception;
@property (readonly) NSString *backTrace;
@property (readonly) GHTestStatus status;
@property (readonly) BOOL failed;
@property (readonly) GHTestStats stats;

@property (assign) id<GHTestDelegate> delegate;

/*!
 Run the test.
 After running, the interval and exception properties may be set.
 @result Yes if passed, NO otherwise
 */
- (void)run;

@end
