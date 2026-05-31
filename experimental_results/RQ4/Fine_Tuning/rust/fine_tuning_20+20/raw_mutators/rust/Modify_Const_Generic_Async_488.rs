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

pub struct Modify_Const_Generic_Async_488;

impl Mutator for Modify_Const_Generic_Async_488 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Async_488"
    }
    fn mutate(&self, file: &mut syn::File) {
        fn is_const_generic_alias(alias: &syn::TypeAlias) -> bool {
            if let syn::Type::ImplTrait(_) = &*alias.ty {
                true
            } else {
                false
            }
        }

        let mut alias_map = HashSet::new();
        for item in &file.items {
            if let syn::Item::Type(alias) = item {
                if is_const_generic_alias(alias) {
                    alias_map.insert(alias.ident.to_string());
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_none() {
                    continue;
                }
                let mut remove_indices = Vec::new();
                for (index, param) in func.sig.generics.params.iter().enumerate() {
                    if let syn::GenericParam::Const(const_param) = param {
                        if alias_map.contains(&const_param.ty.to_token_stream().to_string()) {
                            remove_indices.push(index);
                        }
                    }
                }
                for index in remove_indices.iter().rev() {
                    func.sig.generics.params.remove(*index);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async functions with const generic parameters that reference a type alias using `impl Trait`. It removes such const generic parameters from the function signature, thereby simplifying the function's generic constraints. This transformation stresses the compiler's handling of const generics and type aliasing in async contexts, potentially revealing weaknesses in type inference and trait resolution."
    }
}