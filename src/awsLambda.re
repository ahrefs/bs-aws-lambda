/**
   Type definitions for AWS Lambda
   http://docs.aws.amazon.com/lambda
   https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/aws-lambda/index.d.ts
 */
/** This is the responsability of the user to cast this any to the good type. */
type any;

external any : 'a => any = "%identity";

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
  [@bs.set] "callbackWaitsForEmptyEventLoop": bool,
  "functionName": string,
  "functionVersion": string,
  "invokedFunctionArn": string,
  "memoryLimitInMB": int,
  "awsRequestId": string,
  "logGroupName": string,
  "logStreamName": string,
  "identity": Js.Nullable.t(cognitoIdentity),
  "clientContext": Js.Nullable.t(clientContext),
  [@bs.meth] "getRemainingTimeInMillis": unit => int,
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
  ('event, context, callback_error) => Js.Promise.t(unit);

type handler('event, 'error, 'return) =
  ('event, context, callback('error, 'return)) => Js.Promise.t(unit);

type handler_default('event, 'return) = handler('event, error, 'return);

/**
 * API Gateway CustomAuthorizer AuthResponse.PolicyDocument.Statement.
 * http://docs.aws.amazon.com/apigateway/latest/developerguide/use-custom-authorizer.html#api-gateway-custom-authorizer-output
 */
type authResponseContext = Js.Dict.t(any);

module APIGatewayProxy = {
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
    "isBase64Encoded": Js.Nullable.t(bool),
  };
  let result = (~headers=?, ~body, ~statusCode, ()) : result => {
    let (body, isBase64Encoded) =
      switch (body) {
      | `Plain(body) => (body, false)
      | `Base64(body) => (body, true)
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
    "isBase64Encoded": bool,
    "path": string,
    "pathParameters": Js.Null.t(Js.Dict.t(string)),
    "queryStringParameters": Js.Null.t(Js.Dict.t(string)),
    "stageVariables": Js.Null.t(Js.Dict.t(string)),
    "requestContext": eventRequestContext,
    "resource": string,
  };
  type handler = handler_default(event, result);
};

module APIGatewayAuthorizer = {
  type event = {
    .
    "type": string,
    "methodArn": string,
    "authorizationToken": Js.Nullable.t(string),
    "headers": Js.Nullable.t(Js.Dict.t(string)),
    "pathParameters": Js.Nullable.t(Js.Dict.t(string)),
    "queryStringParameters": Js.Nullable.t(Js.Dict.t(string)),
    "requestContext": Js.Nullable.t(APIGatewayProxy.eventRequestContext),
  };
  type statement = {
    .
    "_Action": array(string),
    "_Effect": string,
    "_Resource": array(string),
  };
  type policyDocument = {
    .
    "_Statement": array(statement),
    "_Version": string,
  };
  type result = {
    .
    "policyDocument": policyDocument,
    "principalId": string,
    "context": Js.Nullable.t(authResponseContext),
  };
  type nonrec handler = handler(event, string, Js.Nullable.t(result));
};

module Sns = {
  type messageAttribute = {
    .
    "_Type": string,
    "_Value": string,
  };
  type messageAttributes = Js.Dict.t(messageAttribute);
  type message = {
    .
    "_SignatureVersion": string,
    "_Timestamp": string,
    "_Signature": string,
    "_SigningCertUrl": string,
    "_MessageId": string,
    "_Message": string,
    "_MessageAttributes": messageAttributes,
    "_Type": string,
    "_UnsubscribeUrl": string,
    "_TopicArn": string,
    "_Subject": string,
  };
  type eventRecord = {
    .
    "_EventVersion": string,
    "_EventSubscriptionArn": string,
    "_EventSource": string,
    "_Sns": message,
  };
  type event = {. "_Records": array(eventRecord)};
  type handler = handler_error(event);
};

module Dynamodb = {
  type attributeValue = {
    .
    "_B": Js.Nullable.t(string),
    "_BS": Js.Nullable.t(array(string)),
    "_BOOL": Js.Nullable.t(bool),
    "_L": Js.Nullable.t(array(attributeValue)),
    "_M": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "_N": Js.Nullable.t(string),
    "_NS": Js.Nullable.t(array(string)),
    "_NULL": Js.Nullable.t(bool),
    "_S": Js.Nullable.t(string),
    "_SS": Js.Nullable.t(array(string)),
  };
  type streamRecord = {
    .
    "_ApproximateCreationTime": Js.Nullable.t(int),
    "_Keys": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "_NewImage": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "_OldImage": Js.Nullable.t(Js.Dict.t(attributeValue)),
    "_SequenceNumber": Js.Nullable.t(string),
    "_SizeBytes": Js.Nullable.t(int),
    "_StreamViewType": Js.Nullable.t(string) /* 'KEYS_ONLY' | 'NEW_IMAGE' | 'OLD_IMAGE' | 'NEW_AND_OLD_IMAGES' */
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
  type streamEvent = {. "_Records": array(record)};
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
  type handler = handler_error(event);
};
