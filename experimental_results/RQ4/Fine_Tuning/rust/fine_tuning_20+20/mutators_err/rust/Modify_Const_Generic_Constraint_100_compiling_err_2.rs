use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Constraint_100;

impl Mutator for Modify_Const_Generic_Constraint_100 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Constraint_100"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_helper = false;
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "some_other_condition" {
                    has_helper = true;
                }
            }
        }
        let mut target_items = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                }) {
                    target_items.push(i);
                }
            }
        }
        for i in target_items {
            if let Item::Fn(item_fn) = &mut file.items[i] {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                if type_path.path.segments.iter().any(|seg| {
                                    seg.ident == "BikeshedIntrinsicFrom"
                                }) {
                                    if let Some(last) =
                                        type_path.path.segments.last()
                                    {
                                        if let PathArguments::AngleBracketed(
                                            args,
                                        ) = &mut last.arguments
                                        {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(
                                                    expr,
                                                ) = arg
                                                {
                                                    if let Expr::Block(
                                                        expr_block,
                                                    ) = expr
                                                    {
                                                        if expr_block
                                                            .block
                                                            .stmts
                                                            .len()
                                                            == 1
                                                        {
                                                            if let Stmt::Expr(
                                                                inner_expr,
                                                                _,
                                                            ) = &mut expr_block
                                                                .block
                                                                .stmts[0]
                                                            {
                                                                if let Expr::Call(
                                                                    inner_call,
                                                                ) = inner_expr
                                                                {
                                                                    if let Expr::Path(
                                                                        inner_path,
                                                                    ) = &*
                                                                        inner_call
                                                                            .func
                                                                    {
                                                                        if inner_path.path.segments.iter().any(|seg| {
                                                                            seg.ident == "from_options"
                                                                        }) {
                                                                            if let Some(inner_last) = inner_path.path.segments.last() {
                                                                                if let PathArguments::None = &inner_last.arguments {
                                                                                    let mut new_args = Punctuated::new();
                                                                                    new_args.push(GenericArgument::Type(Type::Infer(TypeInfer {
                                                                                        underscore_token: Default::default(),
                                                                                    })));
                                                                                    let new_inner_last = PathSegment {
                                                                                        ident: inner_last.ident.clone(),
                                                                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                                                            colon2_token: None,
                                                                                            lt_token: Default::default(),
                                                                                            args: new_args,
                                                                                            gt_token: Default::default(),
                                                                                        }),
                                                                                    };
                                                                                    let mut new_inner_segments = Punctuated::new();
                                                                                    for seg in &inner_path.path.segments {
                                                                                        new_inner_segments.push(seg.clone());
                                                                                    }
                                                                                    new_inner_segments.pop();
                                                                                    new_inner_segments.push(new_inner_last);
                                                                                    inner_call.func = Box::new(Expr::Path(ExprPath {
                                                                                        attrs: inner_path.attrs.clone(),
                                                                                        qself: inner_path.qself.clone(),
                                                                                        path: Path {
                                                                                            leading_colon: inner_path.path.leading_colon,
                                                                                            segments: new_inner_segments,
                                                                                        },
                                                                                    }));
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_helper {
            let helper_fn: ItemFn = parse_quote! {
                fn some_other_condition() -> bool {
                    use std::time::{SystemTime, UNIX_EPOCH};
                    let now = SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
                    now.as_secs() % 2 == 0
                }
            };
            file.items.push(Item::Fn(helper_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with const generics that use complex constraints in their where clause, specifically those involving the BikeshedIntrinsicFrom trait. It modifies the constant expression within the constraint to introduce a non-constant element, such as a runtime-dependent function call, thereby breaking the compile-time const evaluation. If a suitable helper function is not present, it adds one to ensure the mutation is valid. This transformation aims to stress-test the compiler's handling of const generic constraints and unevaluated constants, potentially exposing ICEs or other bugs related to const evaluation and trait resolution."
    }
}