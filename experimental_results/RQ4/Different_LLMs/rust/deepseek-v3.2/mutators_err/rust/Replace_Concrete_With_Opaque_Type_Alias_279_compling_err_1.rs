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

pub struct Replace_Concrete_With_Opaque_Type_Alias_279;

impl Mutator for Replace_Concrete_With_Opaque_Type_Alias_279 {
    fn name(&self) -> &str {
        "Replace_Concrete_With_Opaque_Type_Alias_279"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_aliases = Vec::new();
        let mut constants_to_mutate = Vec::new();
        let mut has_main = false;
        
        for item in &file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(_) = &type_item.ty {
                    opaque_aliases.push((type_item.ident.clone(), type_item.ty.clone()));
                }
            }
            if let Item::Const(const_item) = item {
                if let Type::Path(type_path) = &const_item.ty {
                    if opaque_aliases.iter().any(|(ident, _)| {
                        type_path.path.segments.last().map(|seg| &seg.ident) == Some(ident)
                    }) {
                        constants_to_mutate.push(const_item.clone());
                    }
                }
            }
            if let Item::Fn(fn_item) = item {
                if fn_item.sig.ident == "main" {
                    has_main = true;
                }
            }
        }
        
        if opaque_aliases.is_empty() {
            let new_alias: Item = parse_quote! {
                type OpaqueAlias = impl std::fmt::Debug;
            };
            file.items.insert(0, new_alias);
            opaque_aliases.push((Ident::new("OpaqueAlias", Span::call_site()), 
                parse_quote!(impl std::fmt::Debug)));
        }
        
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Type::Path(type_path) = &const_item.ty {
                    if opaque_aliases.iter().any(|(ident, _)| {
                        type_path.path.segments.last().map(|seg| &seg.ident) == Some(ident)
                    }) {
                        let mut rng = thread_rng();
                        let new_expr: Expr = match rng.gen_range(0..4) {
                            0 => parse_quote!(3.14),
                            1 => parse_quote!("mismatched"),
                            2 => parse_quote!(false),
                            _ => parse_quote!([1, 2, 3]),
                        };
                        const_item.expr = Box::new(new_expr);
                    }
                }
            }
        }
        
        if !has_main {
            let main_fn: Item = parse_quote! {
                fn main() {
                    let _ = ();
                }
            };
            file.items.push(main_fn);
        }
        
        for (alias_ident, alias_ty) in &opaque_aliases {
            let mut found_defining_fn = false;
            for item in &file.items {
                if let Item::Fn(fn_item) = item {
                    if let ReturnType::Type(_, ret_ty) = &fn_item.sig.output {
                        if let Type::Path(type_path) = &**ret_ty {
                            if type_path.path.segments.last().map(|seg| &seg.ident) == Some(alias_ident) {
                                found_defining_fn = true;
                                break;
                            }
                        }
                    }
                }
            }
            
            if !found_defining_fn {
                let fn_name = Ident::new(&format!("define_{}", alias_ident), Span::call_site());
                let new_fn: Item = match &alias_ty {
                    Type::ImplTrait(impl_trait) => {
                        if impl_trait.bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.path.segments.last().map(|seg| seg.ident.to_string()) == Some("Debug".to_string())
                            } else {
                                false
                            }
                        }) {
                            parse_quote! {
                                fn #fn_name() -> #alias_ident { 42 }
                            }
                        } else {
                            parse_quote! {
                                fn #fn_name() -> #alias_ident { () }
                            }
                        }
                    }
                    _ => parse_quote! {
                        fn #fn_name() -> #alias_ident { 42 }
                    },
                };
                file.items.insert(1, new_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets opaque type aliases (impl Trait) used in constant/static definitions. It replaces initializers with values of different concrete types that still satisfy the trait bounds, creating hidden type mismatches. When no opaque alias exists, one is created. A defining function is added if missing, and a main function ensures MIR validation. This triggers ICEs in MIR validation by forcing the compiler to reconcile multiple hidden types for the same opaque alias."
    }
}