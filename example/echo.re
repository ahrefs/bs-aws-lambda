let handler: AwsLambda.APIGatewayProxy.handler =
  (event, _context, cb) => {
    let parameter =
      Js.Null.toOption(event##queryStringParameters)
      |> Js.Option.andThen((. params) => Js.Dict.get(params, "userid"));
    switch parameter {
    | Some(userid) => Js.log2("executing lambda for", userid)
    | None => Js.log("executing lambda for anonymous user")
    };
    let result =
      switch (Js.Null.toOption(event##body)) {
      | None =>
        Js.log("error: no body available in the request");
        AwsLambda.APIGatewayProxy.result(
          ~body=`Plain({|{"status": "no body available in the request"}|}),
          ~statusCode=400,
          ()
        );
      | Some(body) => {
          "body": body,
          "statusCode": 200,
          "headers": Js.Nullable.null,
          "isBase64Encoded": Js.Nullable.return(event##isBase64Encoded)
        }
      };
    cb(Js.null, result);
    Js.Promise.resolve();
  };
