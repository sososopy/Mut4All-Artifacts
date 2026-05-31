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

pub struct Modify_Trait_And_Impl_Definition_With_Invalid_Lifetime_Resolution_147;

impl Mutator for Modify_Trait_And_Impl_Definition_With_Invalid_Lifetime_Resolution_147 {
    fn name(&self) -> &str {
        "Modify_Trait_And_Impl_Definition_With_Invalid_Lifetime_Resolution_147"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_idents = Vec::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let trait_ident = &item_trait.ident;
                let mut has_lifetime = false;
                let mut has_type = false;
                for param in &item_trait.generics.params {
                    match param {
                        GenericParam::Lifetime(_) => {
                            has_lifetime = true;
                        }
                        GenericParam::Type(_) => {
                            has_type = true;
                        }
                        _ => {}
                    }
                }
                if !has_lifetime {
                    continue;
                }
                if !has_type {
                    continue;
                }
                trait_idents.push(trait_ident.clone());
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let path = &trait_bound.path;
                                    for trait_ident in &trait_idents {
                                        if path.is_ident(trait_ident) {
                                            let new_bound: TypeParamBound = parse_quote! {
                                                impl for<'b> #trait_ident<'b, T = impl #trait_ident<'c> + 'd>
                                            };
                                            *pat_type.ty = Type::Verbatim(
                                                new_bound.to_token_stream(),
                                            );
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
        "This mutator targets trait definitions with both lifetime and type parameters, specifically those used in function signatures with complex `impl Trait` bounds. It replaces such bounds with a version that introduces conflicting or unresolved lifetimes, aiming to exploit weaknesses in the compiler's lifetime resolution logic and potentially trigger an ICE."
    }
}