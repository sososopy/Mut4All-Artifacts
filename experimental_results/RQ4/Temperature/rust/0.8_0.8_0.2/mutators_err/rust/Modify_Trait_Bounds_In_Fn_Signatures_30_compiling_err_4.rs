use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, BoundLifetimes, Expr, ExprCall, ExprPath,
    File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime,
    LifetimeParam, Local, Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt,
    TraitBound, TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath,
    punctuated::Punctuated, token::{Comma, Paren, Plus}, visit::Visit,
};

use crate::mutator::Mutator;

pub struct ModifyTraitBoundsInFnSignatures30;

impl Mutator for ModifyTraitBoundsInFnSignatures30 {
    fn name(&self) -> &str {
        "ModifyTraitBoundsInFnSignatures30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        // Add an extra trait bound
                        type_param.bounds.push(parse_quote!(TraitC));

                        // Introduce conflicting bound
                        type_param.bounds.push(parse_quote!(TraitB));
                    }
                }

                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        let mut new_bounds = type_impl_trait.bounds.clone();

                        // Alter the associated type specification
                        new_bounds.push(parse_quote!(TraitA<AssocType = Type1>));

                        *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function signatures that include trait bounds. It adds extra trait bounds, introduces conflicting bounds, and alters associated type specifications. This aims to test the Rust compiler's handling of complex trait bounds and associated types, potentially exposing bugs in trait resolution and type inference."
    }
}