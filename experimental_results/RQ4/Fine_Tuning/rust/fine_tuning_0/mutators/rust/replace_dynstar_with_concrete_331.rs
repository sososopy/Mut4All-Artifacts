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

pub struct Replace_DynStar_With_Concrete_331;

impl Mutator for Replace_DynStar_With_Concrete_331 {
    fn name(&self) -> &str {
        "Replace_DynStar_With_Concrete_331"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::TraitObject(type_trait_object) = &**return_type {
                        if type_trait_object.bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.path.segments.last().unwrap().ident == "Debug"
                            } else {
                                false
                            }
                        }) {
                            *return_type = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!(f64),
                            }));

                            for stmt in &mut item_fn.block.stmts {
                                if let Stmt::Expr(Expr::Cast(expr_cast), _) = stmt {
                                    if let Type::TraitObject(type_trait_object) = &*expr_cast.ty {
                                        if type_trait_object.bounds.iter().any(|bound| {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                trait_bound.path.segments.last().unwrap().ident == "Debug"
                                            } else {
                                                false
                                            }
                                        }) {
                                            expr_cast.ty = Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: parse_quote!(f64),
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions returning `dyn* Debug` and replaces it with a concrete type `f64`. It also updates cast operations in the function body to cast to `f64` instead of `dyn* Debug`. This transformation tests the compiler's handling of dynamically sized trait objects and their conversion to concrete types."
    }
}