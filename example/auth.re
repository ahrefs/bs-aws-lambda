let generatePolicy =
    (~context=?, ~principalId, ~effect, ~resource, ())
    : AwsLambda.APIGatewayAuthorizer.result => {
  let context = Js.Nullable.fromOption(context);
  {
    "principalId": principalId,
    "context": context,
    "policyDocument": {
      "Statement": [|
        {
          "Action": [|"execute-api:Invoke"|],
          "Effect": effect,
          "Resource": resource,
        },
      |],
      "Version": "2012-10-17",
    },
  };
};

let handle: AwsLambda.APIGatewayAuthorizer.handler =
  (event, _context, cb) => {
    let token =
      Js.Nullable.toOption(event##queryStringParameters)
      |> Js.Option.andThen((. params) => Js.Dict.get(params, "token"));
    switch (token) {
    | Some("secrettoken") =>
      cb(
        Js.null,
        Js.Nullable.return(
          generatePolicy(
            ~principalId="myuser",
            ~effect="Allow",
            ~resource=[|event##methodArn|],
            (),
          ),
        ),
      );
      Js.Promise.resolve();
    | _ =>
      cb(
        Js.Null.return({
          "name": "Unauthorized",
          "message": "Unauthorized",
          "stack": Js.Nullable.null,
        }),
        Js.Nullable.null,
      );
      Js.Promise.resolve();
    };
  };
