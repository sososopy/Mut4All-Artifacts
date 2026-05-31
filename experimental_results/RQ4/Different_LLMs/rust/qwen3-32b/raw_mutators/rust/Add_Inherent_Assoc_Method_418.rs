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

pub struct Add_Inherent_Assoc_Method_418;

impl Mutator for Add_Inherent_Assoc_Method_418 {
    fn name(&self) -> &str {
        "Add_Inherent_Assoc_Method_418"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                // Collect associated types in this impl
                let mut assoc_types = Vec::new();
                for impl_item in &impl_item.items {
                    if let syn::ImplItem::Type(ty) = impl_item {
                        assoc_types.push(ty.ident.clone());
                    }
                }
                if assoc_types.is_empty() {
                    continue;
                }
                // Check if there are any methods
                let has_methods = impl_item.items.iter().any(|item| matches!(item, syn::ImplItem::Fn(_)));
                if has_methods {
                    // Modify the first method's return type
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            if let ReturnType::Type(_, ref mut return_type) = func.sig.output {
                                let assoc_ty = &assoc_types[0];
                                *return_type = Box::new(parse_quote!(Self::#assoc_ty));
                            }
                            break;
                        }
                    }
                } else {
                    // Add a new method
                    let assoc_ty = &assoc_types[0];
                    let new_method = parse_quote! {
                        fn new_method(&self) -> Self::#assoc_ty {
                            0
                        }
                    };
                    impl_item.items.push(syn::ImplItem::Fn(new_method));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a method into an impl block that defines an inherent associated type. The method uses the associated type in its return type, forcing the compiler to evaluate trait bounds and obligations during method analysis. This increases the likelihood of exposing ICEs in the compiler's handling of associated types within method signatures."
    }
}