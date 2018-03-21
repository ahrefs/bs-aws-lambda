/**
   Type definitions for AWS Lambda
   http://docs.aws.amazon.com/lambda
   https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/aws-lambda/index.d.ts
 */
/** This is the responsability of the user to cast this any to the good type. */
type any;

type cognitoIdentity = {
  .
  "cognitoIdentityId": string,
  "cognitoIdentityPoolId": string,
};

type clientContextClient = {
  .
  "installationId": string,
  "appTitle": string,
  "appVersionName": string,
  "appVersionCode": string,
  "appPackageName": string,
};

type clientContextEnv = {
  .
  "platformVersion": string,
  "platform": string,
  "make": string,
  "model": string,
  "locale": string,
};

type clientContext = {
  .
  "client": clientContextClient,
  "Custom": Js.Nullable.t(any),
  "env": clientContextEnv,
};

type context = {
  .
  [@bs.set] "callbackWaitsForEmptyEventLoop": Js.boolean,
  "functionName": string,
  "functionVersion": string,
  "invokedFunctionArn": string,
  "memoryLimitInMB": int,
  "awsRequestId": string,
  "logGroupName": string,
  "logStreamName": string,
  "identity": Js.Nullable.t(cognitoIdentity),
  "clientContext": Js.Nullable.t(clientContext),
  "getRemainingTimeInMillis": unit => int,
};

type error = {
  .
  "name": string,
  "message": string,
  "stack": Js.Nullable.t(string),
};

/**
 * Optional callback parameter.
 * http://docs.aws.amazon.com/lambda/latest/dg/nodejs-prog-model-handler.html
 *
 * @param error – an optional parameter that you can use to provide results of the failed Lambda function execution.
 * @param result – an optional parameter that you can use to provide the result of a successful function execution. The result provided must be JSON.stringify compatible.
 */
type callback_error = Js.Null.t(error) => unit;

type callback('return) = (Js.Null.t(error), 'return) => unit;

/**
 * AWS Lambda handler function.
 * http://docs.aws.amazon.com/lambda/latest/dg/nodejs-prog-model-handler.html
 *
 * @param event – event data.
 * @param context – runtime information of the Lambda function that is executing.
 * @param callback – optional callback to return information to the caller, otherwise return value is null.
 */
type handler_error('event) =
  ('event, context, callback_error) => Js.Promise.t(unit);

type handler('event, 'cb) =
  ('event, context, callback('cb)) => Js.Promise.t(unit);

/**
 * API Gateway CustomAuthorizer AuthResponse.PolicyDocument.Statement.
 * http://docs.aws.amazon.com/apigateway/latest/developerguide/use-custom-authorizer.html#api-gateway-custom-authorizer-output
 */
type authResponseContext = Js.Dict.t(any);

module APIGatewayProxy = {
  module Authorizer = {
    type event = {
      .
      "type": string,
      "methodArn": string,
      "authorizationToken": string,
    };
    type statement = {
      .
      "Action": array(string),
      "Effect": string,
      "Resource": array(string),
      "Sid": string,
    };
    type policyDocument = {
      .
      "Statement": array(statement),
      "Version": string,
    };
    type result = {
      .
      "policyDocument": policyDocument,
      "principalId": string,
    };
    type handler =
      (event, context, (Js.Null.t(string), Js.Null.t(result)) => unit) =>
      Js.Promise.t(unit);
  };
  type identity = {
    .
    "accessKey": Js.Null.t(string),
    "accountId": Js.Null.t(string),
    "apiKey": Js.Null.t(string),
    "caller": Js.Null.t(string),
    "cognitoAuthenticationProvider": Js.Null.t(string),
    "cognitoAuthenticationType": Js.Null.t(string),
    "cognitoIdentityId": Js.Null.t(string),
    "cognitoIdentityPoolId": Js.Null.t(string),
    "sourceIp": Js.Null.t(string),
    "user": Js.Null.t(string),
    "userAgent": Js.Null.t(string),
    "userArn": Js.Null.t(string),
  };
  type eventRequestContext = {
    .
    "accountId": string,
    "apiId": string,
    "authorizer": Js.Nullable.t(authResponseContext),
    "httpMethod": string,
    "stage": string,
    "requestId": string,
    "requestTimeEpoch": int,
    "resourceId": string,
    "resourcePath": string,
    "identity": identity,
  };
  /** [header: string]: boolean | number | string */
  type headers = Js.Dict.t(any);
  type result = {
    .
    "statusCode": int,
    "headers": Js.Nullable.t(headers),
    "body": string,
    "isBase64Encoded": Js.Nullable.t(Js.boolean),
  };
  let result = (~headers=?, ~body, ~statusCode, ()) : result => {
    let (body, isBase64Encoded) =
      switch (body) {
      | `Plain(body) => (body, Js.false_)
      | `Base64(body) => (body, Js.true_)
      };
    let headers = Js.Nullable.fromOption(headers);
    {
      "body": body,
      "statusCode": statusCode,
      "headers": headers,
      "isBase64Encoded": Js.Nullable.return(isBase64Encoded),
    };
  };
  type event = {
    .
    "body": Js.Null.t(string),
    "headers": Js.Dict.t(string),
    "httpMethod": string,
    "isBase64Encoded": Js.boolean,
    "path": string,
    "pathParameters": Js.Null.t(Js.Dict.t(string)),
    "queryStringParameters": Js.Null.t(Js.Dict.t(string)),
    "stageVariables": Js.Null.t(Js.Dict.t(string)),
    "requestContext": eventRequestContext,
    "resource": string,
  };
  type nonrec handler = handler(event, result);
};

module Sns = {
  type messageAttribute = {
    .
    "Type": string,
    "Value": string,
  };
  type messageAttributes = Js.Dict.t(messageAttribute);
  type message = {
    .
    "SignatureVersion": string,
    "Timestamp": string,
    "Signature": string,
    "SigningCertUrl": string,
    "MessageId": string,
    "Message": string,
    "MessageAttributes": messageAttributes,
    "Type": string,
    "UnsubscribeUrl": string,
    "TopicArn": string,
    "Subject": string,
  };
  type eventRecord = {
    .
    "EventVersion": string,
    "EventSubscriptionArn": string,
    "EventSource": string,
    "Sns": message,
  };
  type event = {. "Records": array(eventRecord)};
  type nonrec handler = handler_error(event);
};

module Dynamodb = {
  type attributeValue = {
    .
    "B": Js.Nullable.t(string),
    "BS": Js.Nullable.t(array(string)),
    "BOOL": Js.Nullable.t(Js.boolean),
    "L": Js.Nullable.t(array(attributeValue)),
    "M": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "N": Js.Nullable.t(string),
    "NS": Js.Nullable.t(array(string)),
    "NULL": Js.Nullable.t(Js.boolean),
    "S": Js.Nullable.t(string),
    "SS": Js.Nullable.t(array(string)),
  };
  type streamRecord = {
    .
    "ApproximateCreationTime": Js.Nullable.t(int),
    "Keys": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "NewImage": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "OldImage": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "SequenceNumber": Js.Nullable.t(string),
    "SizeBytes": Js.Nullable.t(int),
    "StreamViewType": Js.Nullable.t(string) /* 'KEYS_ONLY' | 'NEW_IMAGE' | 'OLD_IMAGE' | 'NEW_AND_OLD_IMAGES' */
  };
  type record = {
    .
    "awsRegion": Js.Nullable.t(string),
    "dynamodb": Js.Nullable.t(streamRecord),
    "eventID": string,
    "eventName": Js.Nullable.t(string), /* 'INSERT' | 'MODIFY' | 'REMOVE';*/
    "eventSource": Js.Nullable.t(string),
    "eventSourceARN": Js.Nullable.t(string),
    "eventVersion": Js.Nullable.t(string),
    "userIdentity": Js.Nullable.t(string),
  };
  type streamEvent = {. "Records": array(record)};
  type streamHandler = handler_error(streamEvent);
};

module Scheduled = {
  type detail;
  type event = {
    .
    "detail-type": string,
    "account": string,
    "region": string,
    "detail": detail,
    "source": string,
    "time": string,
    "id": string,
    "resources": array(string),
  };
  type nonrec handler = handler_error(event);
};
