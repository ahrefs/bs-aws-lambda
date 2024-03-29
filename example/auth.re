let generatePolicy =
    (~context=?, ~principalId, ~effect, ~resource, ())
    : AwsLambda.APIGatewayAuthorizer.Result.t =>
  AwsLambda.APIGatewayAuthorizer.(
    Result.make(
      ~policyDocument=
        PolicyDocument.make(
          ~statement=[|
            Statement.make(
              ~action=[|"execute-api:Invoke"|],
              ~effect,
              ~resource,
            ),
          |],
          ~version="2012-10-17",
        ),
      ~principalId,
      ~context?,
      (),
    )
  );

let handle: AwsLambda.APIGatewayAuthorizer.handler =
  (event, _context) => {
    open AwsLambda.APIGatewayAuthorizer;
    let token =
      event
      |. Event.queryStringParameters
      |> Js.Option.andThen((. params) => Js.Dict.get(params, "token"));
    switch (token) {
    | Some("secrettoken") =>
      Js.Promise.resolve(
        Js.Nullable.return(
          generatePolicy(
            ~principalId="myuser",
            ~effect="Allow",
            ~resource=[|event |. Event.methodArn|],
            (),
          ),
        ),

      );
    | _ =>
      Js.Promise.reject(Failure("Unauthorized"))
    };
  };
