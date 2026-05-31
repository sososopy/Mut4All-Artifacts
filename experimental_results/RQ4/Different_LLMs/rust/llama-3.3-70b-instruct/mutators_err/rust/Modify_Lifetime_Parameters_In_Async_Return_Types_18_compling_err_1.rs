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

pub struct Modify_Lifetime_Parameters_In_Async_Return_Types_18;

impl Mutator for Modify_Lifetime_Parameters_In_Async_Return_Types_18 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_In_Async_Return_Types_18"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::Reference(type_reference) = &**return_type {
                            if let Some(lifetime) = &type_reference.lifetime {
                                let mut new_lifetime: Option<Lifetime> = None;
                                for param in &func.sig.generics.params {
                                    if let GenericParam::Lifetime(lifetime_param) = param {
                                        new_lifetime = Some(lifetime_param.lifetime.clone());
                                        break;
                                    }
                                }
                                if let Some(new_lifetime) = new_lifetime {
                                    type_reference.lifetime = Some(new_lifetime);
                                } else {
                                    type_reference.lifetime = None;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with return types containing references to opaque types. It modifies the lifetime parameters of the return type, changing them to other valid lifetime parameters present in the function signature, or removing them altogether if possible. This transformation aims to test the compiler's handling of lifetime parameters in async function return types."
    }
}