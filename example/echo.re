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
      switch (event |. Event.body, event |. Event.isBase64Encoded) {
      | (None, false) =>
        Js.log("error: no body available in the request");
        result(
          ~body=`Plain({|{"status": "no body available in the request"}|}),
          ~statusCode=400,
          (),
        );
      | (None, true) =>
        Js.log("error: no body available in the request");
        result(
          ~body=
            `Base64(
              "eyJzdGF0dXMiOiAibm8gYm9keSBhdmFpbGFibGUgaW4gdGhlIHJlcXVlc3QifQ==",
            ),
          ~statusCode=400,
          (),
        );
      | (Some(body), false) =>
        result(~body=`Plain(body), ~statusCode=200, ())
      | (Some(body), true) =>
        result(~body=`Base64(body), ~statusCode=200, ())
      };
    cb(Js.null, result);
    Js.Promise.resolve();
  };
