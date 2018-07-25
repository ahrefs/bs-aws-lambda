let handler: AwsLambda.APIGatewayProxy.handler =
  (event, _context, cb) => {
    open AwsLambda.APIGatewayProxy;
    let parameter =
      event
      |. Event.queryStringParameters
      |> Js.Option.andThen((. params) => Js.Dict.get(params, "userid"));
    switch (parameter) {
    | Some(userid) => Js.log2("executing lambda for", userid)
    | None => Js.log("executing lambda for anonymous user")
    };
    let result =
      switch (event |. Event.body) {
      | None =>
        Js.log("error: no body available in the request");
        result(
          ~body=`Plain({|{"status": "no body available in the request"}|}),
          ~statusCode=400,
          (),
        );
      | Some(body) =>
        Result.make(~statusCode=200, ~body, ~isBase64Encoded=event |. Event.isBase64Encoded, ())
      };
    cb(Js.null, result);
    Js.Promise.resolve();
  };
