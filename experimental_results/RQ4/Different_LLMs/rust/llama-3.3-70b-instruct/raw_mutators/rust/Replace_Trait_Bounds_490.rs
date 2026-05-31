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

pub struct Replace_Trait_Bounds_490;

impl Mutator for Replace_Trait_Bounds_490 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_490"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let trait_bounds = vec![
            "Copy + Send",
            "Sync + Debug",
            "Clone + Default",
            "PartialEq + Ord",
        ];

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let new_trait_bound = trait_bounds.choose(&mut rng).unwrap();
                        let new_bounds = parse_quote!(#new_trait_bound);
                        let new_return_type = parse_quote!(impl #new_bounds);
                        *return_type = new_return_type;
                    }
                }

                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let new_trait_bound = trait_bounds.choose(&mut rng).unwrap();
                            let new_bounds = parse_quote!(#new_trait_bound);
                            let new_arg_type = parse_quote!(impl #new_bounds);
                            pat_type.ty = Box::new(new_arg_type);
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let new_trait_bound = trait_bounds.choose(&mut rng).unwrap();
                                let new_bounds = parse_quote!(#new_trait_bound);
                                let new_return_type = parse_quote!(impl #new_bounds);
                                *return_type = new_return_type;
                            }
                        }

                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                                    let new_trait_bound = trait_bounds.choose(&mut rng).unwrap();
                                    let new_bounds = parse_quote!(#new_trait_bound);
                                    let new_arg_type = parse_quote!(impl #new_bounds);
                                    pat_type.ty = Box::new(new_arg_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait bounds in function signatures and replaces them with different trait bounds. This transformation aims to test the compiler's handling of trait bounds and their implications on type inference and trait resolution."
    }
}