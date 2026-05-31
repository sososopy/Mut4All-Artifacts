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

pub struct Replace_ImplTraitAlias_Param_With_Impl_Trait_159;

impl Mutator for Replace_ImplTraitAlias_Param_With_Impl_Trait_159 {
    fn name(&self) -> &str {
        "Replace_ImplTraitAlias_Param_With_Impl_Trait_159"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut alias_map = HashMap::new();
        for item in &file.items {
            if let Item::Type(type_alias) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                    alias_map.insert(type_alias.ident.clone(), type_impl_trait.clone());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if let Some(type_impl_trait) = alias_map.get(&segment.ident) {
                                    pat_type.ty = Box::new(Type::ImplTrait(type_impl_trait.clone()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function parameters that use a type alias for `impl Trait` and replaces them with a direct `impl Trait` type. It first collects all type aliases that wrap `impl Trait` into a map, then iterates over function signatures to substitute any matching parameter types with the corresponding `impl Trait` definition. This transformation stresses the compiler's handling of opaque types and type alias resolution, potentially exposing issues in monomorphization and trait inference."
    }
}