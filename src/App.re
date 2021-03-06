type route =
  | DistBuilder
  | Home
  | NotFound;

let routeToPath = route =>
  switch (route) {
  | DistBuilder => "/dist-builder"
  | Home => "/"
  | _ => "/"
  };

module Menu = {
  module Styles = {
    open Css;
    let menu =
      style([
        position(`relative),
        marginTop(em(0.25)),
        marginBottom(em(0.25)),
        selector(
          "a",
          [
            borderRadius(em(0.25)),
            display(`inlineBlock),
            backgroundColor(`hex("eee")),
            padding(em(1.)),
            cursor(`pointer),
          ],
        ),
        selector("a:hover", [backgroundColor(`hex("bfcad4"))]),
        selector("a:hover", [backgroundColor(`hex("bfcad4"))]),
        selector(
          "a:not(:first-child):not(:last-child)",
          [marginRight(em(0.25)), marginLeft(em(0.25))],
        ),
      ]);
  };

  module Item = {
    [@react.component]
    let make = (~href, ~children) => {
      <a
        href
        onClick={e => {
          e->ReactEvent.Synthetic.preventDefault;
          ReasonReactRouter.push(href);
        }}>
        children
      </a>;
    };
  };

  [@react.component]
  let make = () => {
    <div className=Styles.menu>
      <Item href={routeToPath(Home)} key="home"> {"Home" |> R.ste} </Item>
      <Item href={routeToPath(DistBuilder)} key="dist-builder">
        {"Dist Builder" |> R.ste}
      </Item>
    </div>;
  };
};

let fixedLength = r =>
  <div className="w-full max-w-screen-xl mx-auto px-6"> r </div>;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let routing =
    switch (url.path) {
    | ["dist-builder"] => DistBuilder
    | [] => Home
    | _ => NotFound
    };

  <>
    <Menu />
    {switch (routing) {
     | DistBuilder => <DistBuilder />
     | Home => <Home />
     | _ => fixedLength({"Page is not found" |> R.ste})
     }}
  </>;
};
