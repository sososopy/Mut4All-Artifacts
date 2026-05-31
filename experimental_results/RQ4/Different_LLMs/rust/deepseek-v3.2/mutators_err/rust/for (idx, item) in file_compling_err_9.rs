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

pub struct Insert_Static_Transmute_Extern_Type_Wrapper_74;

impl Mutator for Insert_Static_Transmute_Extern_Type_Wrapper_74 {
    fn name(&self) -> &str {
        "Insert_Static_Transmute_Extern_Type_Wrapper_74"
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator inserts a static transmute wrapper for extern types."
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut extern_type_name: Option<Ident> = None;
        let mut wrapper_exists = false;
        let mut static_insertion_index: Option<usize> = None;
        let mut extern_block_index: Option<usize> = None;
        for (idx, item) in file.items.iter().enumerate() {
            if let Item::ForeignMod(extern_block) = item {
                extern_block_index = Some(idx);
                for item in &extern_block.items {
                    if let syn::ForeignItem::Fn(extern_fn) = item {
                        extern_type_name = Some(extern_fn.sig.ident.clone());
                    }
                }
            }
            if let Item::Static(static_item) = item {
                if static_item.ident.to_string().contains("wrapper") {
                    wrapper_exists = true;
                }
            }
        }
        if let Some(extern_idx) = extern_block_index {
            if let Some(type_name) = extern_type_name {
                if !wrapper_exists {
                    let wrapper_name = Ident::new(&format!("{}_wrapper", type_name), Span::call_site());
                    let static_wrapper: Item = parse_quote! {
                        static mut #wrapper_name: *mut std::mem::MaybeUninit<#type_name> = std::mem::transmute(0usize);
                    };
                    static_insertion_index = Some(extern_idx + 1);
                    file.items.insert(static_insertion_index.unwrap(), static_wrapper);
                }
            }
        }
    }
}