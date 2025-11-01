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

pub struct Modify_Const_Generic_Lifetimes_68;

impl Mutator for Modify_Const_Generic_Lifetimes_68 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Lifetimes_68"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut lifetimes = HashSet::new();
                for generic_param in &item_fn.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = generic_param {
                        lifetimes.insert(lifetime_param.lifetime.clone());
                    }
                }

                for generic_param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = generic_param {
                        if let Type::Reference(type_ref) = &const_param.ty {
                            if let Some(lifetime) = &type_ref.lifetime {
                                let new_lifetime = lifetimes.iter().find(|&lt| lt != lifetime).cloned().unwrap_or_else(|| {
                                    let new_lt = Lifetime::new("'a", Span::call_site());
                                    lifetimes.insert(new_lt.clone());
                                    new_lt
                                });
                                const_param.ty = Type::Reference(TypeReference {
                                    and_token: type_ref.and_token,
                                    lifetime: Some(new_lifetime),
                                    mutability: type_ref.mutability,
                                    elem: type_ref.elem.clone(),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with const generic parameters that have lifetimes. It modifies the lifetime specifier of these const generic parameters by either changing it to another existing lifetime or introducing a new one. The mutation ensures that the code remains syntactically correct by adjusting related lifetimes accordingly."
    }
}