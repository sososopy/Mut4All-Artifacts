use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Macro_Invocation_With_Function_Call_198;

impl Mutator for Replace_Macro_Invocation_With_Function_Call_198 {
    fn name(&self) -> &str {
        "Replace_Macro_Invocation_With_Function_Call_198"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(mac) = item {
                let macro_name = mac.mac.path.segments.first().unwrap().ident.clone();
                let func_name = Ident::new(&format!("{}_func", macro_name), mac.mac.path.span());
                let mut func_args = Punctuated::new();
                for arg in mac.mac.tokens.clone() {
                    if let TokenTree::Ident(ident) = arg {
                        func_args.push(Expr::Path(ExprPath {
                            attrs: Vec::new(),
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: ident.clone(),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }));
                    }
                }
                let func_call = ExprCall {
                    attrs: Vec::new(),
                    func: Box::new(Expr::Path(ExprPath {
                        attrs: Vec::new(),
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![PathSegment {
                                ident: func_name.clone(),
                                arguments: PathArguments::None,
                            }]),
                        },
                    })),
                    paren_token: Paren {
                        span: mac.mac.path.span(),
                    },
                    args: func_args,
                };
                let func_decl = ItemFn {
                    attrs: Vec::new(),
                    vis: syn::Visibility::Inherited,
                    sig: Signature {
                        constness: None,
                        asyncness: None,
                        unsafety: None,
                        abi: None,
                        ident: func_name.clone(),
                        generics: Generics {
                            params: Punctuated::new(),
                            where_clause: None,
                            lt_token: None,
                            gt_token: None,
                        },
                        paren_token: Paren {
                            span: mac.mac.path.span(),
                        },
                        inputs: Punctuated::from_iter(vec![FnArg::Typed(PatType {
                            attrs: Vec::new(),
                            pat: Box::new(Pat::Ident(Ident::new("arg", mac.mac.path.span()).into())),
                            colon_token: token::Colon {
                                spans: [mac.mac.path.span()],
                            },
                            ty: Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("str", mac.mac.path.span()).into(),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            })),
                        })]),
                        output: ReturnType::Default,
                        variadic: None,
                        fn_token: token::Fn {
                            span: mac.mac.path.span(),
                        },
                    },
                    block: Box::new(Block {
                        brace_token: token::Brace {
                            span: mac.mac.path.span(),
                        },
                        stmts: Vec::new(),
                    }),
                };
                file.items.push(Item::Fn(func_decl));
                *item = Item::Expr(Expr::Call(func_call));
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces macro invocations with function calls that have the same name as the macro and take the same arguments. This transformation simulates the behavior of the macro invocation while using a function call, which may help expose bugs related to the interaction between macros and lint checks."
    }
}