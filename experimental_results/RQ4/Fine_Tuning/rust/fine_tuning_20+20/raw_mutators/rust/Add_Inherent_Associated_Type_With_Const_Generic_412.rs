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

pub struct Add_Inherent_Associated_Type_With_Const_Generic_412;

impl Mutator for Add_Inherent_Associated_Type_With_Const_Generic_412 {
    fn name(&self) -> &str {
        "Add_Inherent_Associated_Type_With_Const_Generic_412"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_sample = false;
        let mut has_example = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "Sample" {
                    has_sample = true;
                }
                if item_struct.ident == "Example" {
                    has_example = true;
                }
            }
        }
        if !has_sample {
            file.items.push(Item::Struct(ItemStruct {
                attrs: vec![],
                vis: syn::Visibility::Inherited,
                struct_token: token::Struct {
                    span: Span::call_site(),
                },
                ident: Ident::new("Sample", Span::call_site()),
                generics: Generics::default(),
                fields: syn::Fields::Unit,
                semi_token: Some(token::Semi {
                    spans: [Span::call_site()],
                }),
            }));
        }
        if !has_example {
            file.items.push(Item::Struct(ItemStruct {
                attrs: vec![],
                vis: syn::Visibility::Inherited,
                struct_token: token::Struct {
                    span: Span::call_site(),
                },
                ident: Ident::new("Example", Span::call_site()),
                generics: {
                    let mut generics = Generics::default();
                    generics.params.push(GenericParam::Const(ConstParam {
                        attrs: vec![],
                        const_token: token::Const {
                            span: Span::call_site(),
                        },
                        ident: Ident::new("N", Span::call_site()),
                        colon_token: Some(token::Colon {
                            spans: [Span::call_site()],
                        }),
                        ty: Type::Path(TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("usize", Span::call_site())),
                        }),
                        default: None,
                    }));
                    generics
                },
                fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                    paren_token: token::Paren {
                        span: Span::call_site(),
                    },
                    unnamed: {
                        let mut fields = Punctuated::new();
                        fields.push(Field {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            colon_token: None,
                            ident: None,
                            ty: Type::Array(TypeArray {
                                bracket_token: token::Bracket {
                                    span: Span::call_site(),
                                },
                                elem: Box::new(Type::Tuple(TypeTuple {
                                    paren_token: token::Paren {
                                        span: Span::call_site(),
                                    },
                                    elems: Punctuated::new(),
                                })),
                                len: Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: syn::Path::from(Ident::new("N", Span::call_site())),
                                }),
                            }),
                        });
                        fields
                    },
                }),
                semi_token: Some(token::Semi {
                    spans: [Span::call_site()],
                }),
            }));
        }
        let mut has_example_impl = false;
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.is_ident("Example") {
                        has_example_impl = true;
                    }
                }
            }
        }
        if !has_example_impl {
            file.items.push(Item::Impl(ItemImpl {
                attrs: vec![],
                defaultness: None,
                unsafety: None,
                impl_token: token::Impl {
                    span: Span::call_site(),
                },
                generics: {
                    let mut generics = Generics::default();
                    generics.params.push(GenericParam::Const(ConstParam {
                        attrs: vec![],
                        const_token: token::Const {
                            span: Span::call_site(),
                        },
                        ident: Ident::new("N", Span::call_site()),
                        colon_token: Some(token::Colon {
                            spans: [Span::call_site()],
                        }),
                        ty: Type::Path(TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("usize", Span::call_site())),
                        }),
                        default: None,
                    }));
                    generics
                },
                trait_: None,
                self_ty: Box::new(Type::Path(TypePath {
                    qself: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: {
                            let mut segments = Punctuated::new();
                            segments.push(PathSegment {
                                ident: Ident::new("Example", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(
                                    AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt {
                                            spans: [Span::call_site()],
                                        },
                                        args: {
                                            let mut args = Punctuated::new();
                                            args.push(GenericArgument::Const(Expr::Path(
                                                ExprPath {
                                                    attrs: vec![],
                                                    qself: None,
                                                    path: syn::Path::from(Ident::new(
                                                        "N",
                                                        Span::call_site(),
                                                    )),
                                                },
                                            )));
                                            args
                                        },
                                        gt_token: token::Gt {
                                            spans: [Span::call_site()],
                                        },
                                    },
                                ),
                            });
                            segments
                        },
                    },
                })),
                brace_token: token::Brace {
                    span: Span::call_site(),
                },
                items: vec![ImplItem::Fn(ImplItemFn {
                    attrs: vec![],
                    vis: syn::Visibility::Inherited,
                    defaultness: None,
                    sig: syn::Signature {
                        constness: None,
                        asyncness: None,
                        unsafety: None,
                        abi: None,
                        fn_token: token::Fn {
                            span: Span::call_site(),
                        },
                        ident: Ident::new("create", Span::call_site()),
                        generics: Generics::default(),
                        paren_token: token::Paren {
                            span: Span::call_site(),
                        },
                        inputs: Punctuated::new(),
                        variadic: None,
                        output: syn::ReturnType::Type(
                            token::RArrow {
                                spans: [Span::call_site()],
                            },
                            Box::new(Type::Path(TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("Self", Span::call_site())),
                            })),
                        ),
                    },
                    block: Box::new(syn::Block {
                        brace_token: token::Brace {
                            span: Span::call_site(),
                        },
                        stmts: vec![Stmt::Expr(Expr::Call(ExprCall {
                            attrs: vec![],
                            func: Box::new(Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: syn::Path::from(Ident::new("Self", Span::call_site())),
                            })),
                            paren_token: token::Paren {
                                span: Span::call_site(),
                            },
                            args: {
                                let mut args = Punctuated::new();
                                args.push(Expr::Array(syn::ExprArray {
                                    attrs: vec![],
                                    bracket_token: token::Bracket {
                                        span: Span::call_site(),
                                    },
                                    elems: Punctuated::new(),
                                }));
                                args
                            },
                        }))],
                    }),
                })],
            }));
        }
        let mut has_sample_impl = false;
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.is_ident("Sample") {
                        has_sample_impl = true;
                    }
                }
            }
        }
        if !has_sample_impl {
            file.items.push(Item::Impl(ItemImpl {
                attrs: vec![],
                defaultness: None,
                unsafety: None,
                impl_token: token::Impl {
                    span: Span::call_site(),
                },
                generics: Generics::default(),
                trait_: None,
                self_ty: Box::new(Type::Path(TypePath {
                    qself: None,
                    path: syn::Path::from(Ident::new("Sample", Span::call_site())),
                })),
                brace_token: token::Brace {
                    span: Span::call_site(),
                },
                items: vec![ImplItem::Type(ImplItemType {
                    attrs: vec![],
                    vis: syn::Visibility::Inherited,
                    defaultness: None,
                    type_token: token::Type {
                        span: Span::call_site(),
                    },
                    ident: Ident::new("ExampleType", Span::call_site()),
                    generics: {
                        let mut generics = Generics::default();
                        generics.params.push(GenericParam::Const(ConstParam {
                            attrs: vec![],
                            const_token: token::Const {
                                span: Span::call_site(),
                            },
                            ident: Ident::new("N", Span::call_site()),
                            colon_token: Some(token::Colon {
                                spans: [Span::call_site()],
                            }),
                            ty: Type::Path(TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("usize", Span::call_site())),
                            }),
                            default: None,
                        }));
                        generics
                    },
                    eq_token: token::Eq {
                        spans: [Span::call_site()],
                    },
                    ty: Box::new(Type::Path(TypePath {
                        qself: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new("Example", Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(
                                        AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt {
                                                spans: [Span::call_site()],
                                            },
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(GenericArgument::Const(Expr::Path(
                                                    ExprPath {
                                                        attrs: vec![],
                                                        qself: None,
                                                        path: syn::Path::from(Ident::new(
                                                            "N",
                                                            Span::call_site(),
                                                        )),
                                                    },
                                                )));
                                                args
                                            },
                                            gt_token: token::Gt {
                                                spans: [Span::call_site()],
                                            },
                                        },
                                    ),
                                });
                                segments
                            },
                        },
                    })),
                    semi_token: token::Semi {
                        spans: [Span::call_site()],
                    },
                })],
            }));
        }
        let mut has_main = false;
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    has_main = true;
                }
            }
        }
        if !has_main {
            file.items.push(Item::Fn(ItemFn {
                attrs: vec![],
                vis: syn::Visibility::Inherited,
                sig: syn::Signature {
                    constness: None,
                    asyncness: None,
                    unsafety: None,
                    abi: None,
                    fn_token: token::Fn {
                        span: Span::call_site(),
                    },
                    ident: Ident::new("main", Span::call_site()),
                    generics: Generics::default(),
                    paren_token: token::Paren {
                        span: Span::call_site(),
                    },
                    inputs: Punctuated::new(),
                    variadic: None,
                    output: syn::ReturnType::Default,
                },
                block: Box::new(syn::Block {
                    brace_token: token::Brace {
                        span: Span::call_site(),
                    },
                    stmts: vec![Stmt::Local(Local {
                        attrs: vec![],
                        let_token: token::Let {
                            span: Span::call_site(),
                        },
                        pat: Pat::Ident(syn::PatIdent {
                            attrs: vec![],
                            by_ref: None,
                            mutability: None,
                            ident: Ident::new("instance", Span::call_site()),
                            subpat: None,
                        }),
                        init: Some((
                            token::Eq {
                                spans: [Span::call_site()],
                            },
                            Box::new(Expr::Call(ExprCall {
                                attrs: vec![],
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: Ident::new("Sample", Span::call_site()),
                                                arguments: syn::PathArguments::None,
                                            });
                                            segments.push(PathSegment {
                                                ident: Ident::new("ExampleType", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt {
                                                            spans: [Span::call_site()],
                                                        },
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Const(
                                                                Expr::Verbatim(
                                                                    quote! { {20 / 2} },
                                                                ),
                                                            ));
                                                            args
                                                        },
                                                        gt_token: token::Gt {
                                                            spans: [Span::call_site()],
                                                        },
                                                    },
                                                ),
                                            });
                                            segments.push(PathSegment {
                                                ident: Ident::new("create", Span::call_site()),
                                                arguments: syn::PathArguments::None,
                                            });
                                            segments
                                        },
                                    },
                                })),
                                paren_token: token::Paren {
                                    span: Span::call_site(),
                                },
                                args: Punctuated::new(),
                            })),
                        )),
                        semi_token: token::Semi {
                            spans: [Span::call_site()],
                        },
                    })],
                }),
            }));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator introduces a struct `Example` with a const generic parameter and an inherent associated type `ExampleType` within a `Sample` struct. It modifies the main function to instantiate an `Example` using a non-standard constant expression for the const generic parameter, thereby testing the compiler's handling of complex const expressions in conjunction with inherent associated types."
    }
}