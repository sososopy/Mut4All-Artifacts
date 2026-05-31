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

pub struct Alter_Function_Return_Types_220;

impl Mutator for Alter_Function_Return_Types_220 {
    fn name(&self) -> &str {
        "Alter_Function_Return_Types_220"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if last_segment.ident == "Result" {
                                if let PathArguments::AngleBracketed(angle_args) =
                                    &last_segment.arguments
                                {
                                    if angle_args.args.len() == 2 {
                                        let mut new_args = Punctuated::new();
                                        new_args.push(GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: {
                                                let mut path = type_path.path.clone();
                                                path.segments.pop();
                                                path.segments.push(PathSegment {
                                                    ident: Ident::new("Option", Span::call_site()),
                                                    arguments: PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: {
                                                                let mut inner_args =
                                                                    Punctuated::new();
                                                                inner_args.push(
                                                                    angle_args.args[0].clone(),
                                                                );
                                                                inner_args
                                                            },
                                                            gt_token: token::Gt::default(),
                                                        },
                                                    ),
                                                });
                                                path
                                            },
                                        })));
                                        new_args.push(angle_args.args[1].clone());
                                        *return_type = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: {
                                                let mut path = type_path.path.clone();
                                                path.segments.pop();
                                                path.segments.push(PathSegment {
                                                    ident: Ident::new(
                                                        "Result",
                                                        Span::call_site(),
                                                    ),
                                                    arguments: PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: new_args,
                                                            gt_token: token::Gt::default(),
                                                        },
                                                    ),
                                                });
                                                path
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
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning `Result<T, E>` and transforms their return type to `Result<Option<T>, E>`. By introducing an additional `Option` layer around the success type, it stresses the compiler's type system, particularly in MIR optimizations and type inference, potentially exposing bugs related to nested type handling and control flow analysis."
    }
}