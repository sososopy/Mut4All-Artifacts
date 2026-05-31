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

pub struct Replace_Type_Alias_With_Concrete_Type_484;

impl Mutator for Replace_Type_Alias_With_Concrete_Type_484 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Concrete_Type_484"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    let concrete_type = self.get_concrete_type(type_impl_trait);
                    if let Some(concrete_type) = concrete_type {
                        item_type.ty = concrete_type;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type aliases defined with `impl Trait` and replaces them with concrete types that implement the same trait. This transformation tests the compiler's handling of type aliases and their interaction with other language features, such as pattern matching and trait bounds."
    }
}

impl Replace_Type_Alias_With_Concrete_Type_484 {
    fn get_concrete_type(&self, type_impl_trait: &TypeImplTrait) -> Option<Box<Type>> {
        let bounds = type_impl_trait.bounds.iter();
        for bound in bounds {
            if let TypeParamBound::Trait(trait_bound) = bound {
                let path = &trait_bound.path;
                let ident = path.segments.last().unwrap().ident.to_string();
                match ident.as_str() {
                    "Copy" => {
                        return Some(Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("i32", Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        })));
                    }
                    "Clone" => {
                        return Some(Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("(u32, u32)", Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        })));
                    }
                    _ => {}
                }
            }
        }
        None
    }
}