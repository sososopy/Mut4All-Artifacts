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

pub struct Insert_Extern_C_Trait_Object_Function_201;

impl Mutator for Insert_Extern_C_Trait_Object_Function_201 {
    fn name(&self) -> &str {
        "Insert_Extern_C_Trait_Object_Function_201"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                traits.push((trait_item.ident.clone(), trait_item.vis.clone(), trait_item));
            }
        }
        for (trait_ident, trait_vis, trait_item) in traits {
            let mut has_default_method = false;
            let mut method_names = Vec::new();
            for item in &trait_item.items {
                if let syn::TraitItem::Fn(method) = item {
                    method_names.push(method.sig.ident.clone());
                    if method.default.is_some() {
                        has_default_method = true;
                    }
                }
            }
            if !has_default_method && !method_names.is_empty() {
                let new_method_name = Ident::new("extern_c_method", Span::call_site());
                let new_method: syn::TraitItemFn = parse_quote! {
                    extern "C" fn #new_method_name(&self, _: ()) -> i32 { 0 }
                };
                for item in &mut file.items {
                    if let Item::Trait(trait_item) = item {
                        if trait_item.ident == trait_ident {
                            trait_item.items.push(syn::TraitItem::Fn(new_method));
                            method_names.push(new_method_name.clone());
                            break;
                        }
                    }
                }
            }
            for item in &mut file.items {
                if let Item::Trait(trait_item) = item {
                    if trait_item.ident == trait_ident {
                        for trait_item_inner in &mut trait_item.items {
                            if let syn::TraitItem::Fn(method) = trait_item_inner {
                                if method.default.is_some() {
                                    method.sig.abi = Some(parse_quote!(extern "C"));
                                    if let ReturnType::Default = method.sig.output {
                                        method.sig.output = parse_quote!(-> i32);
                                    }
                                    let mut has_self = false;
                                    let mut has_unit = false;
                                    for input in &method.sig.inputs {
                                        if let FnArg::Receiver(_) = input {
                                            has_self = true;
                                        }
                                        if let FnArg::Typed(pat_type) = input {
                                            if let Type::Tuple(tuple) = &*pat_type.ty {
                                                if tuple.elems.is_empty() {
                                                    has_unit = true;
                                                }
                                            }
                                        }
                                    }
                                    if !has_self {
                                        method.sig.inputs.insert(0, parse_quote!(&self));
                                    }
                                    if !has_unit {
                                        method.sig.inputs.push(parse_quote!(_: ()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            let extern_fn_name = Ident::new(&format!("extern_c_call_{}", trait_ident), Span::call_site());
            let mut extern_fn: ItemFn = parse_quote! {
                extern "C" fn #extern_fn_name(p: &dyn #trait_ident) -> i32 {
                    p.#method_names[0](())
                }
            };
            if let Visibility::Public(_) = trait_vis {
                extern_fn.vis = parse_quote!(pub);
            }
            file.items.push(Item::Fn(extern_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates trait definitions in the seed program. For each trait, it modifies default methods to have an extern \"C\" ABI, adding self and unit parameters and an i32 return type if missing. If no default method exists, it adds a new extern \"C\" default method. Then it creates an extern \"C\" function at module scope that takes a dyn reference to the trait and calls the modified method. This transformation tests the compiler's handling of extern ABIs with trait objects, default method modifications, and cross-ABI trait method calls."
    }
}