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

pub struct Add_Opaque_Associated_Type_Binding_In_HRTB_439;

impl Mutator for Add_Opaque_Associated_Type_Binding_In_HRTB_439 {
    fn name(&self) -> &str {
        "Add_Opaque_Associated_Type_Binding_In_HRTB_439"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_trait = false;
        let mut trait_names = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                has_trait = true;
                trait_names.push(trait_item.ident.clone());
            }
        }
        if !has_trait {
            let dummy_trait: Item = parse_quote! {
                trait Dummy {}
            };
            file.items.push(dummy_trait);
            trait_names.push(Ident::new("Dummy", Span::call_site()));
        }
        let mut added_feature = false;
        for attr in &file.attrs {
            if let Some(seg) = attr.path.segments.last() {
                if seg.ident == "feature" {
                    if let Meta::List(list) = &attr.meta {
                        if let Ok(nested) = syn::parse::<Meta>(list.tokens.clone().into()) {
                            if let Meta::Path(path) = nested {
                                if let Some(seg) = path.segments.last() {
                                    if seg.ident == "non_lifetime_binders" {
                                        added_feature = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if !added_feature {
            let feature_attr: Attribute = parse_quote! {
                #![feature(non_lifetime_binders)]
            };
            file.attrs.insert(0, feature_attr);
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if !type_impl_trait.bounds.is_empty() {
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let trait_path = &trait_bound.path;
                                    if let Some(seg) = trait_path.segments.last() {
                                        let trait_ident = &seg.ident;
                                        if trait_names.contains(trait_ident) {
                                            let new_type: Type = parse_quote! {
                                                impl for<T> #trait_ident<(), Assoc = impl #trait_ident<T>>
                                            };
                                            *return_type = Box::new(new_type);
                                            let placeholder_expr: Expr = parse_quote! { () };
                                            let mut new_block = item_fn.block.clone();
                                            new_block.stmts.clear();
                                            new_block.stmts.push(Stmt::Expr(placeholder_expr, None));
                                            item_fn.block = new_block;
                                            break;
                                        }
                                    }
                                }
                            }
                        } else {
                            if let Some(trait_ident) = trait_names.first() {
                                let new_type: Type = parse_quote! {
                                    impl for<T> #trait_ident<(), Assoc = impl #trait_ident<T>>
                                };
                                *return_type = Box::new(new_type);
                                let placeholder_expr: Expr = parse_quote! { () };
                                let mut new_block = item_fn.block.clone();
                                new_block.stmts.clear();
                                new_block.stmts.push(Stmt::Expr(placeholder_expr, None));
                                item_fn.block = new_block;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait`. It modifies the return type to introduce a higher-ranked trait binder with an associated type binding that uses `impl Trait` referencing the binder variable, mimicking the structure that triggered an ICE related to non_lifetime_binders. It ensures the feature is enabled, adds a dummy trait if none exists, and replaces the function body with `()` to potentially trigger compiler bugs in type resolution."
    }
}