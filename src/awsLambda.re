/**
   Type definitions for AWS Lambda
   http://docs.aws.amazon.com/lambda
   https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/aws-lambda/index.d.ts
 */
module CognitoIdentity = {
  [@bs.deriving abstract]
  type t = {
    cognitoIdentityId: string,
    cognitoIdentityPoolId: string,
  };
  let make = t;
};

module ClientContextClient = {
  [@bs.deriving abstract]
  type t = {
    installationId: string,
    appTitle: string,
    appVersionName: string,
    appVersionCode: string,
    appPackageName: string,
  };
  let make = t;
};

module ClientContextEnv = {
  [@bs.deriving abstract]
  type t = {
    platformVersion: string,
    platform: string,
    make: string,
    model: string,
    locale: string,
  };
  let make = t;
};

module ClientContext = {
  [@bs.deriving abstract]
  type t = {
    client: ClientContextClient.t,
    [@bs.optional] [@bs.as "Custom"]
    custom: Js.Json.t,
    env: ClientContextEnv.t,
  };
  let make = t;
};

module Context = {
  [@bs.deriving abstract]
  type t = {
    mutable callbackWaitsForEmptyEventLoop: bool,
    functionName: string,
    functionVersion: string,
    invokedFunctionArn: string,
    memoryLimitInMB: int,
    awsRequestId: string,
    logGroupName: string,
    logStreamName: string,
    [@bs.optional]
    identity: CognitoIdentity.t,
    [@bs.optional]
    clientContext: ClientContext.t,
  };

  let make = t;
  [@bs.send] external getRemainingTimeInMillis : t => int = "";
};

type error = Js.Nullable.t(Js.Exn.t);

/**
 * Optional callback parameter.
 * http://docs.aws.amazon.com/lambda/latest/dg/nodejs-prog-model-handler.html
 *
 * @param error – an optional parameter that you can use to provide results of the failed Lambda function execution.
 * @param result – an optional parameter that you can use to provide the result of a successful function execution. The result provided must be JSON.stringify compatible.
 */
type callback_error = error => unit;

type callback('error, 'return) = (Js.Null.t('error), 'return) => unit;

/**
 * AWS Lambda handler function.
 * http://docs.aws.amazon.com/lambda/latest/dg/nodejs-prog-model-handler.html
 *
 * @param event – event data.
 * @param context – runtime information of the Lambda function that is executing.
 * @param callback – optional callback to return information to the caller, otherwise return value is null.
 */
type handler_error('event) =
  ('event, Context.t, callback_error) => Js.Promise.t(unit);

type handler('event, 'error, 'return) =
  ('event, Context.t, callback('error, 'return)) => Js.Promise.t(unit);

type handler_default('event, 'return) = handler('event, error, 'return);

/**
 * API Gateway CustomAuthorizer AuthResponse.PolicyDocument.Statement.
 * http://docs.aws.amazon.com/apigateway/latest/developerguide/use-custom-authorizer.html#api-gateway-custom-authorizer-output
 */
type authResponseContext = Js.Dict.t(Js.Json.t);

module APIGatewayProxy = {
  module Identity = {
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      accessKey: string,
      [@bs.optional]
      accountId: string,
      [@bs.optional]
      apiKey: string,
      [@bs.optional]
      caller: string,
      [@bs.optional]
      cognitoAuthenticationProvider: string,
      [@bs.optional]
      cognitoAuthenticationType: string,
      [@bs.optional]
      cognitoIdentityId: string,
      [@bs.optional]
      cognitoIdentityPoolId: string,
      [@bs.optional]
      sourceIp: string,
      [@bs.optional]
      user: string,
      [@bs.optional]
      userAgent: string,
      [@bs.optional]
      userArn: string,
    };

    let make = t;
  };

  module EventRequestContext = {
    [@bs.deriving abstract]
    type t = {
      accountId: string,
      apiId: string,
      [@bs.optional]
      authorizer: authResponseContext,
      httpMethod: string,
      stage: string,
      requestId: string,
      requestTimeEpoch: int,
      resourceId: string,
      resourcePath: string,
      identity: Identity.t,
    };

    let make = t;
  };
  /** [header: string]: boolean | number | string */
  type headers = Js.Dict.t(Js.Json.t);

  module Result = {
    [@bs.deriving abstract]
    type t = {
      statusCode: int,
      [@bs.optional]
      headers,
      body: string,
      [@bs.optional]
      isBase64Encoded: bool,
    };
    let make = t;
  };

  let result = (~headers=?, ~body, ~statusCode, ()) => {
    let (body, isBase64Encoded) =
      switch (body) {
      | `Plain(body) => (body, false)
      | `Base64(body) => (body, true)
      };

    Result.make(~body, ~statusCode, ~headers?, ~isBase64Encoded, ());
  };

  module Event = {
    [@bs.deriving abstract]
    type t = {
      body: Js.Nullable.t(string),
      headers: Js.Dict.t(string),
      httpMethod: string,
      isBase64Encoded: bool,
      path: string,
      pathParameters: Js.Nullable.t(Js.Dict.t(string)),
      queryStringParameters: Js.Nullable.t(Js.Dict.t(string)),
      stageVariables: Js.Nullable.t(Js.Dict.t(string)),
      requestContext: EventRequestContext.t,
      resource: string,
    };
    let make = t;
  };
  type handler = handler_default(Event.t, Result.t);
};

module APIGatewayAuthorizer = {
  module Event = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "type"]
      type_: string,
      methodArn: string,
      [@bs.optional]
      authorizationToken: string,
      [@bs.optional]
      headers: Js.Dict.t(string),
      [@bs.optional]
      pathParameters: Js.Dict.t(string),
      [@bs.optional]
      queryStringParameters: Js.Dict.t(string),
      [@bs.optional]
      requestContext: APIGatewayProxy.EventRequestContext.t,
    };
    let make = t;
  };

  module Statement = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Action"]
      action: array(string),
      [@bs.as "Effect"]
      effect: string,
      [@bs.as "Resource"]
      resource: array(string),
    };
    let make = t;
  };

  module PolicyDocument = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Statement"]
      statement: array(Statement.t),
      [@bs.as "Version"]
      version: string,
    };
    let make = t;
  };

  module Result = {
    [@bs.deriving abstract]
    type t = {
      policyDocument: PolicyDocument.t,
      principalId: string,
      [@bs.optional]
      context: authResponseContext,
    };
    let make = t;
  };
  type nonrec handler = handler(Event.t, string, Js.Nullable.t(Result.t));
};

module Sns = {
  module MessageAttribute = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Type"]
      type_: string,
      [@bs.as "Value"]
      value: string,
    };
    let make = t;
  };

  type messageAttributes = Js.Dict.t(MessageAttribute.t);

  module Message = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "SignatureVersion"]
      signatureVersion: string,
      [@bs.as "Timestamp"]
      timestamp: string,
      [@bs.as "Signature"]
      signature: string,
      [@bs.as "SigningCertUrl"]
      signingCertUrl: string,
      [@bs.as "MessageId"]
      messageId: string,
      [@bs.as "Message"]
      message: string,
      [@bs.as "MessageAttributes"]
      messageAttributes,
      [@bs.as "Type"]
      type_: string,
      [@bs.as "UnsubscribeUrl"]
      unsubscribeUrl: string,
      [@bs.as "TopicArn"]
      topicArn: string,
      [@bs.as "Subject"]
      subject: string,
    };
    let make = t;
  };

  module EventRecord = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "EventVersion"]
      eventVersion: string,
      [@bs.as "EventSubscriptionArn"]
      eventSubscriptionArn: string,
      [@bs.as "EventSource"]
      eventSource: string,
      [@bs.as "Sns"]
      sns: Message.t,
    };
    let make = t;
  };

  module Event = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Records"]
      records: array(EventRecord.t),
    };
    let make = t;
  };

  type handler = handler_error(Event.t);
};

module Dynamodb = {
  module AttributeValue = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "B"] [@bs.optional]
      b: string,
      [@bs.as "BS"] [@bs.optional]
      bs: array(string),
      [@bs.as "BOOL"] [@bs.optional]
      bool,
      [@bs.as "L"] [@bs.optional]
      l: array(t),
      [@bs.as "M"] [@bs.optional]
      m: Js.Dict.t(t),
      [@bs.as "N"] [@bs.optional]
      n: string,
      [@bs.as "NS"] [@bs.optional]
      ns: array(string),
      [@bs.as "NULL"] [@bs.optional]
      null: bool,
      [@bs.as "S"] [@bs.optional]
      s: string,
      [@bs.as "SS"] [@bs.optional]
      ss: array(string),
    };
    let make = t;
  };

  module StreamRecord = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "ApproximateCreationTime"] [@bs.optional]
      approximateCreationTime: int,
      [@bs.as "Keys"] [@bs.optional]
      keys: Js.Dict.t(AttributeValue.t),
      [@bs.as "NewImage"] [@bs.optional]
      newImage: Js.Dict.t(AttributeValue.t),
      [@bs.as "OldImage"] [@bs.optional]
      oldImage: Js.Dict.t(AttributeValue.t),
      [@bs.as "SequenceNumber"] [@bs.optional]
      sequenceNumber: string,
      [@bs.as "SizeBytes"] [@bs.optional]
      sizeBytes: int,
      [@bs.as "StreamViewType"] [@bs.optional]
      streamViewType: string /* 'KEYS_ONLY' | 'NEW_IMAGE' | 'OLD_IMAGE' | 'NEW_AND_OLD_IMAGES' */
    };
    let make = t;
  };

  module Record = {
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      awsRegion: string,
      [@bs.optional]
      dynamodb: StreamRecord.t,
      eventID: string,
      [@bs.optional]
      eventName: string, /* 'INSERT' | 'MODIFY' | 'REMOVE';*/
      [@bs.optional]
      eventSource: string,
      [@bs.optional]
      eventSourceARN: string,
      [@bs.optional]
      eventVersion: string,
      [@bs.optional]
      userIdentity: string,
    };
    let make = t;
  };

  module StreamEvent = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Records"]
      records: array(Record.t),
    };
    let make = t;
  };

  type streamHandler = handler_error(StreamEvent.t);
};

module Scheduled = {
  type detail;
  module Event = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "detail-type"]
      detailType: string,
      account: string,
      region: string,
      detail,
      source: string,
      time: string,
      id: string,
      resources: array(string),
    };
    let make = t;
  };

  type handler = handler_error(Event.t);
};

module Sqs = {
  module RecordAttributes = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "ApproximateReceiveCount"]
      approximateReceiveCount: string,
      [@bs.as "SentTimestamp"]
      sentTimestamp: string,
      [@bs.as "SenderId"]
      senderId: string,
      [@bs.as "ApproximateFirstReceiveTimestamp"]
      approximateFirstReceiveTimestamp: string,
    };

    let make = t;
  };

  module MessageAttribute = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Name"]
      name: string,
      [@bs.as "Type"]
      type_: string,
      [@bs.as "Value"]
      value: string,
    };

    let make = t;
  };

  type sqsMessageAttributes = Js.Dict.t(MessageAttribute.t);

  module Record = {
    [@bs.deriving abstract]
    type t = {
      messageId: string,
      receiptHandle: string,
      body: string,
      attributes: RecordAttributes.t,
      messageAttributes: sqsMessageAttributes,
      md5OfBody: string,
      eventSource: string,
      eventSourceARN: string,
      awsRegion: string,
    };

    let make = t;
  };

  module Event = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Records"]
      records: array(Record.t),
    };
  };

  type handler = handler_error(Event.t);
};

module S3 = {
  module UserIdentity = {
    [@bs.deriving abstract]
    type t = {principalId: string};

    let make = t;
  };

  module RequestParameters = {
    [@bs.deriving abstract]
    type t = {sourceIPAddress: string};

    let make = t;
  };

  module ResponseElements = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "x-amz-request-id"]
      xAmzRequestId: string,
      [@bs.as "x-amz-id-2"]
      xAmzId2: string,
    };

    let make = t;
  };

  module Info = {
    module Bucket = {
      [@bs.deriving abstract]
      type t = {
        name: string,
        ownerIdentity: UserIdentity.t,
        arn: string,
      };

      let make = t;
    };

    module Object = {
      [@bs.deriving abstract]
      type t = {
        key: string,
        size: int,
        eTag: string,
        versionId: string,
        sequencer: string,
      };

      let make = t;
    };

    [@bs.deriving abstract]
    type t = {
      s3SchemaVersion: string,
      configurationId: string,
      bucket: Bucket.t,
      [@bs.as "object"]
      object_: Object.t,
    };

    let make = t;
  };

  module EventRecord = {
    [@bs.deriving abstract]
    type t = {
      eventVersion: string,
      eventSource: string,
      awsRegion: string,
      eventTime: string,
      eventName: string,
      userIdentity: UserIdentity.t,
      requestParameters: RequestParameters.t,
      responseElements: ResponseElements.t,
      s3: Info.t,
    };

    let make = t;
  };

  module Event = {
    [@bs.deriving abstract]
    type t = {
      [@bs.as "Records"]
      records: array(EventRecord.t),
    };

    let make = t;
  };

  type handler = handler_error(Event.t);
};
