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

pub struct Modify_Static_Definition_With_Dynamic_Trait_282;

impl Mutator for Modify_Static_Definition_With_Dynamic_Trait_282 {
    fn name(&self) -> &str {
        "Modify_Static_Definition_With_Dynamic_Trait_282"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_items = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Item(syn::Item::Static(item_static)) = stmt {
                        if let Type::TraitObject(type_trait_object) = &*item_static.ty {
                            let new_item = Item::Fn(ItemFn {
                                attrs: item_static.attrs.clone(),
                                vis: item_static.vis.clone(),
                                sig: syn::Signature {
                                    constness: None,
                                    asyncness: None,
                                    unsafety: None,
                                    abi: None,
                                    fn_token: token::Fn {
                                        span: item_static.static_token.span,
                                    },
                                    ident: item_static.ident.clone(),
                                    generics: Generics::default(),
                                    paren_token: token::Paren::default(),
                                    inputs: Punctuated::new(),
                                    variadic: None,
                                    output: ReturnType::Type(
                                        token::RArrow::default(),
                                        Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: {
                                                let mut segments = Punctuated::new();
                                                segments.push(PathSegment {
                                                    ident: Ident::new("Box", Span::call_site()),
                                                    arguments: PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: {
                                                                let mut args = Punctuated::new();
                                                                args.push(GenericArgument::Type(
                                                                    Type::TraitObject(
                                                                        type_trait_object.clone(),
                                                                    ),
                                                                ));
                                                                args
                                                            },
                                                            gt_token: token::Gt::default(),
                                                        },
                                                    ),
                                                });
                                                Path {
                                                    leading_colon: None,
                                                    segments,
                                                }
                                            },
                                        })),
                                    ),
                                },
                                block: Box::new(Block {
                                    brace_token: token::Brace::default(),
                                    stmts: vec![Stmt::Expr(
                                        Expr::Call(ExprCall {
                                            attrs: Vec::new(),
                                            func: Box::new(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: Path::from(Ident::new(
                                                    "Box::new",
                                                    Span::call_site(),
                                                )),
                                            })),
                                            paren_token: token::Paren::default(),
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push((*item_static.expr).clone());
                                                args
                                            },
                                        }),
                                        None,
                                    )],
                                }),
                            });
                            new_items.push(syn::Item::Fn(new_item));
                        } else {
                            new_items.push(syn::Item::Static(item_static.clone()));
                        }
                    } else {
                        new_items.push(syn::Item::Verbatim(stmt.to_token_stream()));
                    }
                }
                item_fn.block = Box::new(Block {
                    brace_token: item_fn.block.brace_token,
                    stmts: new_items
                        .into_iter()
                        .map(Stmt::Item)
                        .collect::<Vec<_>>(),
                });
            }
            if let Item::Mod(item_mod) = item {
                if let Some((_, items)) = &mut item_mod.content {
                    let mut new_items = Vec::new();
                    for item in items {
                        if let Item::Static(item_static) = item {
                            if let Type::TraitObject(type_trait_object) = &*item_static.ty {
                                let new_item = Item::Fn(ItemFn {
                                    attrs: item_static.attrs.clone(),
                                    vis: item_static.vis.clone(),
                                    sig: syn::Signature {
                                        constness: None,
                                        asyncness: None,
                                        unsafety: None,
                                        abi: None,
                                        fn_token: token::Fn {
                                            span: item_static.static_token.span,
                                        },
                                        ident: item_static.ident.clone(),
                                        generics: Generics::default(),
                                        paren_token: token::Paren::default(),
                                        inputs: Punctuated::new(),
                                        variadic: None,
                                        output: ReturnType::Type(
                                            token::RArrow::default(),
                                            Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: {
                                                    let mut segments = Punctuated::new();
                                                    segments.push(PathSegment {
                                                        ident: Ident::new("Box", Span::call_site()),
                                                        arguments:
                                                            PathArguments::AngleBracketed(
                                                                AngleBracketedGenericArguments {
                                                                    colon2_token: None,
                                                                    lt_token: token::Lt::default(),
                                                                    args: {
                                                                        let mut args =
                                                                            Punctuated::new();
                                                                        args.push(
                                                                            GenericArgument::Type(
                                                                                Type::TraitObject(
                                                                                    type_trait_object
                                                                                        .clone(),
                                                                                ),
                                                                            ),
                                                                        );
                                                                        args
                                                                    },
                                                                    gt_token: token::Gt::default(),
                                                                },
                                                            ),
                                                    });
                                                    Path {
                                                        leading_colon: None,
                                                        segments,
                                                    }
                                                },
                                            })),
                                        ),
                                    },
                                    block: Box::new(Block {
                                        brace_token: token::Brace::default(),
                                        stmts: vec![Stmt::Expr(
                                            Expr::Call(ExprCall {
                                                attrs: Vec::new(),
                                                func: Box::new(Expr::Path(ExprPath {
                                                    attrs: Vec::new(),
                                                    qself: None,
                                                    path: Path::from(Ident::new(
                                                        "Box::new",
                                                        Span::call_site(),
                                                    )),
                                                })),
                                                paren_token: token::Paren::default(),
                                                args: {
                                                    let mut args = Punctuated::new();
                                                    args.push((*item_static.expr).clone());
                                                    args
                                                },
                                            }),
                                            None,
                                        )],
                                    }),
                                });
                                new_items.push(syn::Item::Fn(new_item));
                            } else {
                                new_items.push(syn::Item::Static(item_static.clone()));
                            }
                        } else {
                            new_items.push(item.clone());
                        }
                    }
                    *items = new_items;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator transforms static variable declarations using dynamic traits within functions or modules into equivalent function definitions that return boxed trait objects. By converting a static context into a dynamic one, it stresses the compiler's handling of trait objects, ownership semantics, and type inference, potentially uncovering issues related to trait resolution, memory management, and code generation for dynamic dispatch."
    }
}