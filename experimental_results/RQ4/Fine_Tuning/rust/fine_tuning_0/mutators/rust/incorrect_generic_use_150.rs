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

pub struct Incorrect_Generic_Use_150;

impl Mutator for Incorrect_Generic_Use_150 {
    fn name(&self) -> &str {
        "Incorrect_Generic_Use_150"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    continue;
                }

                let mut rng = thread_rng();
                let new_generic = Ident::new("UndeclaredType", Span::call_site());

                // Randomly decide to mutate either a parameter type or return type
                if rng.gen_bool(0.5) {
                    // Mutate a parameter type
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            pat_type.ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath::from(new_generic.clone()),
                            }));
                            break;
                        }
                    }
                } else {
                    // Mutate the return type
                    if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                        *ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(new_generic.clone()),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic type parameters by introducing an undeclared generic type in the function signature. It randomly replaces a parameter type or the return type with an undeclared generic type, aiming to test the compiler's handling of generic type mismatches and declaration inconsistencies."
    }
}