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

pub struct Convert_IAT_To_Trait_Assoc_In_HRTB_416;

impl Mutator for Convert_IAT_To_Trait_Assoc_In_HRTB_416 {
    fn name(&self) -> &str {
        "Convert_IAT_To_Trait_Assoc_In_HRTB_416"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_name_counter = 0;
        let mut existing_traits = HashSet::new();
        let mut structs_with_iat = Vec::new();
        let mut type_aliases_to_modify = Vec::new();
        let mut fn_signatures_to_modify = Vec::new();

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                existing_traits.insert(item_trait.ident.to_string());
            }
            if let Item::Struct(item_struct) = item {
                if let Some(impl_block) = find_inherent_impl_for_struct(file, &item_struct.ident) {
                    if has_inherent_associated_type(&impl_block) {
                        structs_with_iat.push((item_struct.clone(), impl_block.clone()));
                    }
                }
            }
            if let Item::Type(item_type) = item {
                if contains_hrtb_with_iat_projection(&item_type.ty) {
                    type_aliases_to_modify.push(item_type.clone());
                }
            }
            if let Item::Fn(item_fn) = item {
                if contains_hrtb_with_iat_projection_in_signature(&item_fn.sig) {
                    fn_signatures_to_modify.push(item_fn.clone());
                }
            }
        }

        for (struct_item, impl_block) in structs_with_iat {
            let iat_name = extract_iat_name(&impl_block);
            if let Some(assoc_name) = iat_name {
                let trait_name = generate_trait_name(&assoc_name, &mut trait_name_counter, &existing_traits);
                existing_traits.insert(trait_name.clone());

                let trait_item = create_trait_definition(&trait_name, &assoc_name);
                file.items.insert(0, trait_item);

                let trait_impl = create_trait_implementation(&struct_item, &trait_name, &assoc_name, &impl_block);
                file.items.push(trait_impl);
            }
        }

        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Some(modified_ty) = modify_type_alias(&item_type.ty, &existing_traits) {
                    *item_type.ty = modified_ty;
                }
            }
            if let Item::Fn(item_fn) = item {
                if let Some(modified_sig) = modify_function_signature(&item_fn.sig, &existing_traits) {
                    item_fn.sig = modified_sig;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated type references within higher-ranked trait bounds (HRTBs) and function pointer types. It identifies type aliases and function signatures using `for<'a> fn(...) -> ...` patterns referencing inherent associated types from generic structs with lifetime parameters. The mutation changes the IAT projection to trait-associated type projections by introducing new trait definitions and implementations that mirror the inherent associated types. This alters the binding structure of lifetime parameters in HRTBs relative to the projection, potentially triggering different substitution paths in the compiler and exposing ICEs related to HRTB lifetime substitution and IAT projection interactions."
    }
}

fn find_inherent_impl_for_struct(file: &syn::File, struct_ident: &Ident) -> Option<syn::ItemImpl> {
    for item in &file.items {
        if let Item::Impl(item_impl) = item {
            if item_impl.trait_.is_none() {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    if type_path.path.segments.last().map(|s| &s.ident) == Some(struct_ident) {
                        return Some(item_impl.clone());
                    }
                }
            }
        }
    }
    None
}

fn has_inherent_associated_type(impl_block: &syn::ItemImpl) -> bool {
    for item in &impl_block.items {
        if let ImplItem::Type(_) = item {
            return true;
        }
    }
    false
}

fn extract_iat_name(impl_block: &syn::ItemImpl) -> Option<Ident> {
    for item in &impl_block.items {
        if let ImplItem::Type(type_item) = item {
            return Some(type_item.ident.clone());
        }
    }
    None
}

fn generate_trait_name(base: &Ident, counter: &mut usize, existing: &HashSet<String>) -> String {
    let mut candidate = format!("Tr_{}", base);
    while existing.contains(&candidate) {
        *counter += 1;
        candidate = format!("Tr_{}_{}", base, counter);
    }
    candidate
}

fn create_trait_definition(trait_name: &str, assoc_name: &Ident) -> syn::Item {
    let trait_ident = Ident::new(trait_name, Span::call_site());
    parse_quote! {
        trait #trait_ident {
            type #assoc_name;
        }
    }
}

fn create_trait_implementation(struct_item: &syn::ItemStruct, trait_name: &str, assoc_name: &Ident, impl_block: &syn::ItemImpl) -> syn::Item {
    let trait_ident = Ident::new(trait_name, Span::call_site());
    let struct_ident = struct_item.ident.clone();
    let struct_generics = struct_item.generics.clone();
    
    let mut assoc_type_value = None;
    for item in &impl_block.items {
        if let ImplItem::Type(type_item) = item {
            if type_item.ident == *assoc_name {
                assoc_type_value = Some(type_item.ty.clone());
                break;
            }
        }
    }
    
    let assoc_value = assoc_type_value.unwrap_or_else(|| parse_quote! { () });
    
    parse_quote! {
        impl #struct_generics #trait_ident for #struct_ident #struct_generics {
            type #assoc_name = #assoc_value;
        }
    }
}

fn contains_hrtb_with_iat_projection(ty: &syn::Type) -> bool {
    struct HRTBIATVisitor {
        found: bool,
    }
    impl Visit<'_> for HRTBIATVisitor {
        fn visit_type(&mut self, ty: &syn::Type) {
            if let Type::BareFn(bare_fn) = ty {
                if bare_fn.lifetimes.is_some() {
                    if let ReturnType::Type(_, return_ty) = &bare_fn.output {
                        if let Type::Path(type_path) = &**return_ty {
                            if is_iat_projection(&type_path.path) {
                                self.found = true;
                            }
                        }
                    }
                }
            }
            syn::visit::visit_type(self, ty);
        }
    }
    let mut visitor = HRTBIATVisitor { found: false };
    visitor.visit_type(ty);
    visitor.found
}

fn contains_hrtb_with_iat_projection_in_signature(sig: &syn::Signature) -> bool {
    for input in &sig.inputs {
        if let FnArg::Typed(pat_type) = input {
            if contains_hrtb_with_iat_projection(&pat_type.ty) {
                return true;
            }
        }
    }
    if let ReturnType::Type(_, return_ty) = &sig.output {
        if contains_hrtb_with_iat_projection(return_ty) {
            return true;
        }
    }
    false
}

fn is_iat_projection(path: &syn::Path) -> bool {
    if path.segments.len() == 2 {
        if let PathArguments::AngleBracketed(args) = &path.segments.last().unwrap().arguments {
            if !args.args.is_empty() {
                return true;
            }
        }
    }
    false
}

fn modify_type_alias(ty: &syn::Type, existing_traits: &HashSet<String>) -> Option<syn::Type> {
    struct TypeModifier<'a> {
        existing_traits: &'a HashSet<String>,
        modified: bool,
        result: Option<syn::Type>,
    }
    impl VisitMut for TypeModifier<'_> {
        fn visit_type_mut(&mut self, ty: &mut syn::Type) {
            if let Type::BareFn(bare_fn) = ty {
                if bare_fn.lifetimes.is_some() {
                    if let ReturnType::Type(_, return_ty) = &mut bare_fn.output {
                        if let Type::Path(type_path) = &mut **return_ty {
                            if is_iat_projection(&type_path.path) {
                                if let Some(trait_name) = find_matching_trait(&type_path.path, self.existing_traits) {
                                    let new_path = create_trait_projection_path(&type_path.path, &trait_name);
                                    **return_ty = Type::Path(new_path);
                                    self.modified = true;
                                }
                            }
                        }
                    }
                }
            }
            syn::visit_mut::visit_type_mut(self, ty);
        }
    }
    let mut modifier = TypeModifier {
        existing_traits,
        modified: false,
        result: None,
    };
    let mut ty_clone = ty.clone();
    modifier.visit_type_mut(&mut ty_clone);
    if modifier.modified {
        Some(ty_clone)
    } else {
        None
    }
}

fn modify_function_signature(sig: &syn::Signature, existing_traits: &HashSet<String>) -> Option<syn::Signature> {
    let mut sig_clone = sig.clone();
    let mut modified = false;
    
    for input in &mut sig_clone.inputs {
        if let FnArg::Typed(pat_type) = input {
            if let Some(new_ty) = modify_type_alias(&pat_type.ty, existing_traits) {
                pat_type.ty = Box::new(new_ty);
                modified = true;
            }
        }
    }
    
    if let ReturnType::Type(_, return_ty) = &mut sig_clone.output {
        if let Some(new_ty) = modify_type_alias(return_ty, existing_traits) {
            *return_ty = Box::new(new_ty);
            modified = true;
        }
    }
    
    if modified {
        Some(sig_clone)
    } else {
        None
    }
}

fn find_matching_trait(path: &syn::Path, existing_traits: &HashSet<String>) -> Option<String> {
    let struct_name = path.segments.first().map(|seg| seg.ident.to_string())?;
    for trait_name in existing_traits {
        if trait_name.starts_with(&format!("Tr_{}", struct_name)) {
            return Some(trait_name.clone());
        }
    }
    None
}

fn create_trait_projection_path(path: &syn::Path, trait_name: &str) -> syn::TypePath {
    let trait_ident = Ident::new(trait_name, Span::call_site());
    let struct_segment = path.segments.first().unwrap().clone();
    let assoc_segment = path.segments.last().unwrap().clone();
    
    let trait_path: syn::Path = parse_quote! { #trait_ident };
    let projection = parse_quote! { <#struct_segment as #trait_path>::#assoc_segment };
    
    if let Type::Path(type_path) = projection {
        type_path
    } else {
        panic!("Failed to create trait projection path");
    }
}