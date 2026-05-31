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

pub struct Swap_Const_Generic_Default_Reference_342;

impl Mutator for Swap_Const_Generic_Default_Reference_342 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Default_Reference_342"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    self.mutate_generics(&mut item_struct.generics, &mut rng);
                }
                Item::Enum(item_enum) => {
                    self.mutate_generics(&mut item_enum.generics, &mut rng);
                }
                Item::Fn(item_fn) => {
                    self.mutate_generics(&mut item_fn.sig.generics, &mut rng);
                }
                Item::Impl(item_impl) => {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            self.mutate_generics(&mut method.sig.generics, &mut rng);
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    for trait_item in &mut item_trait.items {
                        if let TraitItem::Fn(method) = trait_item {
                            self.mutate_generics(&mut method.sig.generics, &mut rng);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic parameters in structs, enums, functions, and methods. It identifies const generic parameters with default values and swaps their default expression to reference a different generic parameter from the same item. This can create dependency cycles, type mismatches, or out-of-range references, testing the compiler's handling of complex generic_const_exprs interactions and edge cases in generic parameter resolution."
    }
}

impl Swap_Const_Generic_Default_Reference_342 {
    fn mutate_generics<R: Rng>(&self, generics: &mut Generics, rng: &mut R) {
        if generics.params.len() < 2 {
            return;
        }

        let mut const_params_with_defaults_indices = Vec::new();
        for (idx, param) in generics.params.iter().enumerate() {
            if let GenericParam::Const(const_param) = param {
                if const_param.default.is_some() {
                    const_params_with_defaults_indices.push(idx);
                }
            }
        }

        if const_params_with_defaults_indices.is_empty() {
            return;
        }

        let target_idx = *const_params_with_defaults_indices.choose(rng).unwrap();
        let other_param_indices: Vec<usize> = generics.params
            .iter()
            .enumerate()
            .filter(|(idx, _)| *idx != target_idx)
            .map(|(idx, _)| idx)
            .collect();

        if other_param_indices.is_empty() {
            return;
        }

        let other_idx = *other_param_indices.choose(rng).unwrap();
        let other_param = &generics.params[other_idx];

        let new_default = match other_param {
            GenericParam::Const(other_const_param) => {
                let ident = other_const_param.ident.clone();
                parse_quote! { #ident }
            }
            GenericParam::Type(other_type_param) => {
                let ident = other_type_param.ident.clone();
                parse_quote! { std::mem::size_of::<#ident>() }
            }
            GenericParam::Lifetime(other_lifetime_param) => {
                let lifetime = other_lifetime_param.lifetime.clone();
                parse_quote! { 0usize }
            }
        };

        if let GenericParam::Const(target_const_param) = &mut generics.params[target_idx] {
            target_const_param.default = Some(new_default);
        }
    }
}