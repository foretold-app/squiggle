open Rationale.Function.Infix;

module FloatFloatMap = {
  module Id =
    Belt.Id.MakeComparable({
      type t = float;
      let cmp: (float, float) => int = Pervasives.compare;
    });

  type t = Belt.MutableMap.t(Id.t, float, Id.identity);

  let fromArray = (ar: array((float, float))) =>
    Belt.MutableMap.fromArray(ar, ~id=(module Id));
  let toArray = (t: t) => Belt.MutableMap.toArray(t);
  let empty = () => Belt.MutableMap.make(~id=(module Id));
  let increment = (el, t: t) =>
    Belt.MutableMap.update(
      t,
      el,
      fun
      | Some(n) => Some(n +. 1.0)
      | None => Some(1.0),
    );

  let get = (el, t: t) => Belt.MutableMap.get(t, el);
  let fmap = (fn, t: t) => Belt.MutableMap.map(t, fn);
};

/* Utils */
module U = {
  let isEqual = (a, b) => a == b;
  let toA = a => [|a|];
  let id = e => e;
};

module O = {
  let dimap = (sFn, rFn, e) =>
    switch (e) {
    | Some(r) => sFn(r)
    | None => rFn()
    };
  ();
  let fmap = Rationale.Option.fmap;
  let bind = Rationale.Option.bind;
  let default = Rationale.Option.default;
  let isSome = Rationale.Option.isSome;
  let isNone = Rationale.Option.isNone;
  let toExn = Rationale.Option.toExn;
  let some = Rationale.Option.some;
  let firstSome = Rationale.Option.firstSome;
  let toExt = Rationale.Option.toExn;
  let flatApply = (fn, b) =>
    Rationale.Option.apply(fn, Some(b)) |> Rationale.Option.flatten;

  let toBool = opt =>
    switch (opt) {
    | Some(_) => true
    | _ => false
    };

  let ffmap = (fn, r) =>
    switch (r) {
    | Some(sm) => fn(sm)
    | _ => None
    };

  let toString = opt =>
    switch (opt) {
    | Some(s) => s
    | _ => ""
    };

  let toResult = (error, e) =>
    switch (e) {
    | Some(r) => Belt.Result.Ok(r)
    | None => Error(error)
    };

  module React = {
    let defaultNull = default(ReasonReact.null);
    let fmapOrNull = fn => fmap(fn) ||> default(ReasonReact.null);
    let flatten = default(ReasonReact.null);
  };
};

/* Functions */
module F = {
  let apply = (a, e) => a |> e;

  let flatten2Callbacks = (fn1, fn2, fnlast) =>
    fn1(response1 => fn2(response2 => fnlast(response1, response2)));

  let flatten3Callbacks = (fn1, fn2, fn3, fnlast) =>
    fn1(response1 =>
      fn2(response2 =>
        fn3(response3 => fnlast(response1, response2, response3))
      )
    );

  let flatten4Callbacks = (fn1, fn2, fn3, fn4, fnlast) =>
    fn1(response1 =>
      fn2(response2 =>
        fn3(response3 =>
          fn4(response4 => fnlast(response1, response2, response3, response4))
        )
      )
    );
};

module Bool = {
  type t = bool;
  let toString = (t: t) => t ? "TRUE" : "FALSE";
  let fromString = str => str == "TRUE" ? true : false;

  module O = {
    let toBool = opt =>
      switch (opt) {
      | Some(true) => true
      | _ => false
      };
  };
};

module Float = {
  let with2DigitsPrecision = Js.Float.toPrecisionWithPrecision(_, ~digits=2);
  let with3DigitsPrecision = Js.Float.toPrecisionWithPrecision(_, ~digits=3);
  let toFixed = Js.Float.toFixed;
  let toString = Js.Float.toString;
};

module I = {
  let increment = n => n + 1;
  let decrement = n => n - 1;
  let toString = Js.Int.toString;
};

/* R for Result */
module R = {
  let result = Rationale.Result.result;
  let id = e => e |> result(U.id, U.id);
  let fmap = Rationale.Result.fmap;
  let bind = Rationale.Result.bind;
  let toOption = (e: Belt.Result.t('a, 'b)) =>
    switch (e) {
    | Ok(r) => Some(r)
    | Error(_) => None
    };
};

let safe_fn_of_string = (fn, s: string): option('a) =>
  try(Some(fn(s))) {
  | _ => None
  };

module S = {
  let safe_float = float_of_string->safe_fn_of_string;
  let safe_int = int_of_string->safe_fn_of_string;
  let default = (defaultStr, str) => str == "" ? defaultStr : str;
};

module J = {
  let toString = Js.Json.decodeString ||> O.default("");
  let fromString = Js.Json.string;
  let fromNumber = Js.Json.number;

  module O = {
    let fromString = (str: string) =>
      switch (str) {
      | "" => None
      | _ => Some(Js.Json.string(str))
      };

    let toString = (str: option('a)) =>
      switch (str) {
      | Some(str) => Some(str |> (Js.Json.decodeString ||> O.default("")))
      | _ => None
      };
  };
};

module M = {
  let format = MomentRe.Moment.format;
  let format_standard = "MMM DD, YYYY HH:mm";
  let format_simple = "L";
  /* TODO: Figure out better name */
  let goFormat_simple = MomentRe.Moment.format(format_simple);
  let goFormat_standard = MomentRe.Moment.format(format_standard);
  let toUtc = MomentRe.momentUtc;
  let toJSON = MomentRe.Moment.toJSON;
  let momentDefaultFormat = MomentRe.momentDefaultFormat;
};

module JsDate = {
  let fromString = Js.Date.fromString;
  let now = Js.Date.now;
  let make = Js.Date.make;
  let valueOf = Js.Date.valueOf;
};

/* List */
module L = {
  let fmap = List.map;
  let get = Belt.List.get;
  let toArray = Array.of_list;
  let fmapi = List.mapi;
  let concat = List.concat;
  let append = List.append;
  let drop = Rationale.RList.drop;
  let remove = Rationale.RList.remove;
  let find = List.find;
  let filter = List.filter;
  let for_all = List.for_all;
  let exists = List.exists;
  let sort = List.sort;
  let length = List.length;
  let filter_opt = Rationale.RList.filter_opt;
  let uniqBy = Rationale.RList.uniqBy;
  let join = Rationale.RList.join;
  let head = Rationale.RList.head;
  let uniq = Rationale.RList.uniq;
  let flatten = List.flatten;
  let last = Rationale.RList.last;
  let append = List.append;
  let getBy = Belt.List.getBy;
  let dropLast = Rationale.RList.dropLast;
  let contains = Rationale.RList.contains;
  let without = Rationale.RList.without;
  let update = Rationale.RList.update;
  let iter = List.iter;
  let findIndex = Rationale.RList.findIndex;
};

/* A for Array */
module A = {
  let fmap = Array.map;
  let fmapi = Array.mapi;
  let to_list = Array.to_list;
  let of_list = Array.of_list;
  let length = Array.length;
  let append = Array.append;
  let empty = [||];
  let unsafe_get = Array.unsafe_get;
  let get = Belt.Array.get;
  let getBy = Belt.Array.getBy;
  let last = a => get(a, length(a) - 1);
  let first = get(_, 0);
  let hasBy = (r, fn) => Belt.Array.getBy(r, fn) |> O.isSome;
  let fold_left = Array.fold_left;
  let fold_right = Array.fold_right;
  let concatMany = Belt.Array.concatMany;
  let keepMap = Belt.Array.keepMap;
  let stableSortBy = Belt.SortArray.stableSortBy;
  let toRanges = (a: array('a)) =>
    switch (a |> Belt.Array.length) {
    | 0
    | 1 => Belt.Result.Error("Must be at least 2 elements")
    | n =>
      Belt.Array.makeBy(n - 1, r => r)
      |> Belt.Array.map(_, index =>
           (
             Belt.Array.getUnsafe(a, index),
             Belt.Array.getUnsafe(a, index + 1),
           )
         )
      |> Rationale.Result.return
    };

  let asList = (f: list('a) => list('a), r: array('a)) =>
    r |> to_list |> f |> of_list;
  /* TODO: Is there a better way of doing this? */
  let uniq = r => asList(L.uniq, r);

  // @todo: Is -1 still the indicator that this is false (as is true with
  // @todo: js findIndex)? Wasn't sure.
  let findIndex = (e, i) =>
    Js.Array.findIndex(e, i)
    |> (
      r =>
        switch (r) {
        | (-1) => None
        | r => Some(r)
        }
    );
  let filter = (o, e) => Js.Array.filter(o, e);

  module O = {
    let concatSomes = (optionals: array(option('a))): array('a) =>
      optionals
      |> Js.Array.filter(Rationale.Option.isSome)
      |> Js.Array.map(
           Rationale.Option.toExn("Warning: This should not have happened"),
         );
    let defaultEmpty = (o: option(array('a))): array('a) =>
      switch (o) {
      | Some(o) => o
      | None => [||]
      };
  };

  module R = {
    let firstErrorOrOpen =
        (results: array(Belt.Result.t('a, 'b)))
        : Belt.Result.t(array('a), 'b) => {
      let bringErrorUp =
        switch (results |> Belt.Array.getBy(_, Belt.Result.isError)) {
        | Some(Belt.Result.Error(err)) => Belt.Result.Error(err)
        | Some(Belt.Result.Ok(_)) => Belt.Result.Ok(results)
        | None => Belt.Result.Ok(results)
        };
      let forceOpen = (r: array(Belt.Result.t('a, 'b))): array('a) =>
        r |> Belt.Array.map(_, r => Belt.Result.getExn(r));
      bringErrorUp |> Belt.Result.map(_, forceOpen);
    };
  };

  module Sorted = {
    let binarySearchFirstElementGreaterIndex = (ar: array('a), el: 'a) => {
      let el = Belt.SortArray.binarySearchBy(ar, el, compare);
      let el = el < 0 ? el * (-1) - 1 : el;
      switch (el) {
      | e when e >= length(ar) => `overMax
      | e when e == 0 => `underMin
      | e => `firstHigher(e)
      };
    };
  };

  module Floats = {
    let split = (sortedArray: array(float)) => {
      let continuous = [||];
      let discrete = FloatFloatMap.empty();
      Belt.Array.forEachWithIndex(
        sortedArray,
        (index, element) => {
          let maxIndex = (sortedArray |> Array.length) - 1;
          let possiblySimilarElements =
            (
              switch (index) {
              | 0 => [|index + 1|]
              | n when n == maxIndex => [|index - 1|]
              | _ => [|index - 1, index + 1|]
              }
            )
            |> Belt.Array.map(_, r => sortedArray[r]);
          let hasSimilarElement =
            Belt.Array.some(possiblySimilarElements, r => r == element);
          hasSimilarElement
            ? FloatFloatMap.increment(element, discrete)
            : {
              let _ = Js.Array.push(element, continuous);
              ();
            };
          ();
        },
      );

      (continuous, discrete);
    };
  };
};

module JsArray = {
  let concatSomes = (optionals: Js.Array.t(option('a))): Js.Array.t('a) =>
    optionals
    |> Js.Array.filter(Rationale.Option.isSome)
    |> Js.Array.map(
         Rationale.Option.toExn("Warning: This should not have happened"),
       );
  let filter = Js.Array.filter;
};

let ste = React.string;
let showIf = (cond, comp) => cond ? comp : ReasonReact.null;