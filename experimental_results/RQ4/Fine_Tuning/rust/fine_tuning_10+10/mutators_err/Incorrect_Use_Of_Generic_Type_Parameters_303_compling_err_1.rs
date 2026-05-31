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

pub struct Incorrect_Use_Of_Generic_Type_Parameters_303;

impl Mutator for Incorrect_Use_Of_Generic_Type_Parameters_303 {
    fn name(&self) -> &str {
        "Incorrect_Use_Of_Generic_Type_Parameters_303"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if !func.sig.generics.params.is_empty() {
                    let mut rng = thread_rng();
                    let new_generic = Ident::new("U", Span::call_site());
                    let mut mutated = false;

                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(PatType { ty, .. }) = input {
                            if let Type::Path(TypePath { path, .. }) = &**ty {
                                if path.segments.iter().any(|seg| seg.ident == "T") {
                                    *ty = Box::new(Type::Path(TypePath {
                                        path: parse_quote!(#new_generic),
                                    }));
                                    mutated = true;
                                    break;
                                }
                            }
                        }
                    }

                    if !mutated {
                        if let ReturnType::Type(_, ty) = &mut func.sig.output {
                            if let Type::Path(TypePath { path, .. }) = &**ty {
                                if path.segments.iter().any(|seg| seg.ident == "T") {
                                    *ty = Box::new(Type::Path(TypePath {
                                        path: parse_quote!(#new_generic),
                                    }));
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function definitions with generic type parameters. It identifies a function signature that uses generics and introduces a misuse by replacing a correctly declared generic type parameter with an undeclared one. This mutation only affects the function signature, focusing on potential discrepancies in generic usage."
    }
}