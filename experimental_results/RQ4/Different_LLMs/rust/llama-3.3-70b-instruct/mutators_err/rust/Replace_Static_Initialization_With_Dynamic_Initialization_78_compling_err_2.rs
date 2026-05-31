use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStatic, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Static_Initialization_With_Dynamic_Initialization_78;

impl Mutator for Replace_Static_Initialization_With_Dynamic_Initialization_78 {
    fn name(&self) -> &str {
        "Replace_Static_Initialization_With_Dynamic_Initialization_78"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                let static_ident = &static_item.ident;
                let static_ty = &static_item.ty;
                let static_expr = &static_item.expr;

                let init_fn_ident = Ident::new(&format!("init_{}", static_ident), Span::call_site());
                let init_fn = ItemFn {
                    attrs: vec![],
                    vis: Visibility::Inherited,
                    sig: Signature {
                        ident: init_fn_ident,
                        generics: Generics::default(),
                        inputs: Punctuated::new(),
                        output: ReturnType::Type(Box::new(static_ty.clone()), None),
                        variadic: None,
                        abi: Abi::Rust,
                        asyncness: Async::NotAsync,
                        constness: Const::NotConst,
                        unsafety: Unsafety::Safe,
                    },
                    block: Box::new(Block {
                        stmts: vec![Stmt::Expr(Expr::Return(ReturnExpr {
                            expr: static_expr.clone(),
                            semi_token: Some(token::Semi::default()),
                        })),],
                        ..Default::default()
                    }),
                };

                let new_static_item = Item::Static(ItemStatic {
                    attrs: static_item.attrs.clone(),
                    vis: static_item.vis.clone(),
                    ident: static_item.ident.clone(),
                    expr: Box::new(Expr::Call(ExprCall {
                        func: Box::new(Expr::Path(ExprPath {
                            attrs: vec![],
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: init_fn_ident.clone(),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        })),
                        args: Punctuated::new(),
                        paren_token: Paren::default(),
                    })),
                    ty: Some(static_ty.clone()),
                    mutability: static_item.mutability.clone(),
                    colon_token: token::Colon::default(),
                    eq_token: token::Eq::default(),
                    semi_token: Some(token::Semi::default()),
                });

                file.items.push(Item::Fn(init_fn));
                *item = new_static_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces static initialization with dynamic initialization using a function. It targets static variables with complex initialization expressions and changes the evaluation context from compile-time to runtime, potentially triggering bugs related to constant evaluation."
    }
}