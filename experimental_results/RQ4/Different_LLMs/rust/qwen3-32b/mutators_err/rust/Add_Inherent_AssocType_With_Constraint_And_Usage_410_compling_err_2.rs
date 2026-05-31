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

pub struct Add_Inherent_AssocType_With_Constraint_And_Usage_410;

impl Mutator for Add_Inherent_AssocType_With_Constraint_And_Usage_410 {
    fn name(&self) -> &str {
        "Add_Inherent_AssocType_With_Constraint_And_Usage_410"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                // Check if the impl has no associated types
                let has_assoc_type = impl_item.items.iter().any(|i| matches!(i, syn::ImplItem::Type(_)));
                if !has_assoc_type {
                    // Check if the impl has at least one type parameter
                    let has_type_param = impl_item.generics.params.iter().any(|p| matches!(p, syn::GenericParam::Type(_)));
                    if has_type_param {
                        // Add the associated type
                        let assoc_type = parse_quote! {
                            type Assoc = Vec<T>;
                        };
                        impl_item.items.push(syn::ImplItem::Type(assoc_type));

                        // Find the struct name from the impl's self_ty
                        if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                            if let Some(segment) = type_path.path.segments.iter().next() {
                                let struct_name = segment.ident.clone();

                                // Create a new function with the usage
                                let new_fn = parse_quote! {
                                    fn f() {
                                        let _: #struct_name<_>::Assoc;
                                    }
                                };

                                // Add the new function to the file
                                file.items.push(syn::Item::Fn(new_fn));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an inherent associated type in an impl block with a trait-bound type (e.g., `Vec<T>`) and adds a function that references this type. This stresses the compiler's handling of associated types, type normalization, and inference during unification, aiming to expose bugs in complex type resolution scenarios."
    }
}