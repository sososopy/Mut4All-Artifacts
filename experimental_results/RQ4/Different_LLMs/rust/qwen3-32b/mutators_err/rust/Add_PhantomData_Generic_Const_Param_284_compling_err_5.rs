use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path as StdPath, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Add_PhantomData_Generic_Const_Param_284;

impl Mutator for Add_PhantomData_Generic_Const_Param_284 {
    fn name(&self) -> &str {
        "Add_PhantomData_Generic_Const_Param_284"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                if !s.generics.params.is_empty() {
                    let first_param = s.generics.params.first().unwrap();
                    let inner_type = match first_param {
                        GenericParam::Lifetime(lt) => {
                            let lifetime = lt.lifetime.clone();
                            let elem = Type::Tuple(TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            });
                            let ref_type = TypeReference {
                                and_token: token::And::default(),
                                lifetime: Some(lifetime),
                                mutability: None,
                                elem: Box::new(elem),
                            };
                            Box::new(Type::Reference(ref_type))
                        },
                        GenericParam::Type(tp) => {
                            let ty = Type::Path(TypePath {
                                qself: None,
                                path: {
                                    let mut path: Path = parse_quote! { #tp };
                                    path
                                },
                            });
                            Box::new(ty)
                        },
                        _ => continue,
                    };
                    let phantom_type = Type::Path(TypePath {
                        qself: None,
                        path: {
                            let mut path: Path = parse_quote! { std };
                            path.leading_colon = None;
                            let mut segs = Punctuated::new();
                            segs.push(PathSegment {
                                ident: Ident::new("marker", Span::call_site()),
                                arguments: PathArguments::None,
                            });
                            segs.push(PathSegment {
                                ident: Ident::new("PhantomData", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(GenericArgument::Type(*inner_type));
                                        args
                                    },
                                    gt_token: token::Gt::default(),
                                }),
                            });
                            path.segments = segs;
                            path
                        },
                    });
                    let const_param = GenericParam::Const(ConstParam {
                        attrs: Vec::new(),
                        const_token: token::Const::default(),
                        ident: Ident::new("C", Span::call_site()),
                        colon_token: token::Colon::default(),
                        ty: phantom_type,
                        eq_token: None,
                        default: None,
                    });
                    s.generics.params.push(const_param);
                }
            } else if let syn::Item::Enum(e) = item {
                if !e.generics.params.is_empty() {
                    let first_param = e.generics.params.first().unwrap();
                    let inner_type = match first_param {
                        GenericParam::Lifetime(lt) => {
                            let lifetime = lt.lifetime.clone();
                            let elem = Type::Tuple(TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            });
                            let ref_type = TypeReference {
                                and_token: token::And::default(),
                                lifetime: Some(lifetime),
                                mutability: None,
                                elem: Box::new(elem),
                            };
                            Box::new(Type::Reference(ref_type))
                        },
                        GenericParam::Type(tp) => {
                            let ty = Type::Path(TypePath {
                                qself: None,
                                path: {
                                    let mut path: Path = parse_quote! { #tp };
                                    path
                                },
                            });
                            Box::new(ty)
                        },
                        _ => continue,
                    };
                    let phantom_type = Type::Path(TypePath {
                        qself: None,
                        path: {
                            let mut path: Path = parse_quote! { std };
                            path.leading_colon = None;
                            let mut segs = Punctuated::new();
                            segs.push(PathSegment {
                                ident: Ident::new("marker", Span::call_site()),
                                arguments: PathArguments::None,
                            });
                            segs.push(PathSegment {
                                ident: Ident::new("PhantomData", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(GenericArgument::Type(*inner_type));
                                        args
                                    },
                                    gt_token: token::Gt::default(),
                                }),
                            });
                            path.segments = segs;
                            path
                        },
                    });
                    let const_param = GenericParam::Const(ConstParam {
                        attrs: Vec::new(),
                        const_token: token::Const::default(),
                        ident: Ident::new("C", Span::call_site()),
                        colon_token: token::Colon::default(),
                        ty: phantom_type,
                        eq_token: None,
                        default: None,
                    });
                    e.generics.params.push(const_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}