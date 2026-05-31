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

pub struct Replace_Static_Type_With_TAIT_477;

impl Mutator for Replace_Static_Type_With_TAIT_477 {
    fn name(&self) -> &str {
        "Replace_Static_Type_With_TAIT_477"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut static_items = Vec::new();
        let mut trait_candidates = vec![
            "Debug",
            "Clone",
            "Default",
            "Send",
            "Sync",
        ];
        
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Some(ty) = extract_concrete_type(&item_static.ty) {
                    static_items.push((item_static, ty));
                }
            }
        }
        
        for (item_static, original_type) in static_items {
            trait_candidates.shuffle(&mut rng);
            let selected_trait = trait_candidates.first().unwrap();
            
            let alias_name = generate_unique_alias_name(&item_static.ident, &mut file.items);
            let fn_name = generate_unique_fn_name(&alias_name, &mut file.items);
            
            let new_type_alias: Item = parse_quote! {
                type #alias_name = impl #selected_trait;
            };
            
            let define_fn: Item = parse_quote! {
                fn #fn_name() -> #alias_name {
                    #original_type
                }
            };
            
            item_static.ty = Box::new(parse_quote!(#alias_name));
            
            file.items.insert(0, define_fn);
            file.items.insert(0, new_type_alias);
        }
        
        if !file.items.iter().any(|item| {
            matches!(item, Item::Macro(item_macro) if item_macro.mac.path.is_ident("feature"))
        }) {
            let feature_attr: Item = parse_quote! {
                #![feature(type_alias_impl_trait)]
            };
            file.items.insert(0, feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies static items with concrete types and replaces them with type alias impl Trait (TAIT). It creates a TAIT alias for a compatible trait (Debug, Clone, Default, Send, Sync) and adds a defining function to provide the concrete type. This transformation introduces the pattern that caused the ICE: TAIT used in static context, potentially triggering debuginfo generation issues in the compiler's type system."
    }
}

fn extract_concrete_type(ty: &Type) -> Option<TokenStream> {
    match ty {
        Type::Path(type_path) => {
            let path = &type_path.path;
            Some(quote! { #path })
        }
        Type::Array(type_array) => {
            let elem = &type_array.elem;
            let len = &type_array.len;
            Some(quote! { [#elem; #len] })
        }
        Type::Slice(type_slice) => {
            let elem = &type_slice.elem;
            Some(quote! { [#elem] })
        }
        Type::Tuple(type_tuple) => {
            let elems = &type_tuple.elems;
            Some(quote! { (#(#elems,)*) })
        }
        _ => None,
    }
}

fn generate_unique_alias_name(base: &Ident, items: &[Item]) -> Ident {
    let mut name = format!("{}_alias", base);
    let mut counter = -1;
    
    while items.iter().any(|item| {
        match item {
            Item::Type(item_type) => item_type.ident == Ident::new(&name, Span::call_site()),
            _ => false,
        }
    }) {
        counter += 1;
        name = format!("{}_alias_{}", base, counter);
    }
    
    Ident::new(&name, Span::call_site())
}

fn generate_unique_fn_name(alias_name: &Ident, items: &[Item]) -> Ident {
    let mut name = format!("_define_{}", alias_name);
    let mut counter = -1;
    
    while items.iter().any(|item| {
        match item {
            Item::Fn(item_fn) => item_fn.sig.ident == Ident::new(&name, Span::call_site()),
            _ => false,
        }
    }) {
        counter += 1;
        name = format!("_define_{}_{}", alias_name, counter);
    }
    
    Ident::new(&name, Span::call_site())
}