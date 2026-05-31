use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Function_Return_Type_366;

impl Mutator for Modify_Function_Return_Type_366 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_366"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    let new_struct = syn::ItemStruct {
                        attrs: vec![],
                        vis: syn::Visibility::Inherited,
                        struct_token: token::Struct { span: Span::call_site() },
                        ident: Ident::new("Complex", Span::call_site()),
                        generics: syn::Generics {
                            lt_token: Some(token::Lt { spans: [Span::call_site()] }),
                            params: {
                                let mut params = Punctuated::new();
                                params.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                                    attrs: vec![],
                                    lifetime: syn::Lifetime::new("'a", Span::call_site()),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                }));
                                params
                            },
                            gt_token: Some(token::Gt { spans: [Span::call_site()] }),
                            where_clause: None,
                        },
                        fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                            paren_token: token::Paren(Span::call_site()),
                            unnamed: {
                                let mut fields = Punctuated::new();
                                fields.push(syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    ident: None,
                                    colon_token: None,
                                    ty: syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path::from(Ident::new("&'a str", Span::call_site())),
                                    }),
                                });
                                fields
                            },
                        }),
                        semi_token: None,
                    };

                    file.items.insert(0, syn::Item::Struct(new_struct));

                    *ty = Box::new(syn::Type::Path(syn::TypePath {
                        qself: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segments = Punctuated::new();
                                segments.push(syn::PathSegment {
                                    ident: Ident::new("Complex", Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt { spans: [Span::call_site()] },
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(syn::GenericArgument::Lifetime(
                                                    syn::Lifetime::new("'a", Span::call_site())
                                                ));
                                                args
                                            },
                                            gt_token: token::Gt { spans: [Span::call_site()] },
                                        }
                                    ),
                                });
                                segments
                            },
                        },
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}