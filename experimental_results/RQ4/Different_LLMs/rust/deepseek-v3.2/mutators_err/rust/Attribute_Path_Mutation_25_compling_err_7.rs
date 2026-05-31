use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemTrait, ItemImpl, ItemMod, ItemForeignMod, ItemStatic, ItemConst, ItemType, ItemUnion, ItemMacro, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Attribute_Path_Mutation_25;

impl Mutator for Attribute_Path_Mutation_25 {
    fn name(&self) -> &str {
        "Attribute_Path_Mutation_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_names = HashSet::new();
        let mut visitor = CollectIdentVisitor { names: &mut existing_names };
        visitor.visit_file(file);
        
        for item in &mut file.items {
            mutate_attributes(item, &mut existing_names);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets built-in attributes (cfg, derive, allow, etc.) applied to any item. It transforms standard attributes into path-like forms by replacing them with #[builtin_attr_name::dummy], where dummy is a unique identifier. This creates malformed attribute syntax that mimics bug reports, potentially exposing ICEs in attribute handling visitors by triggering missing checks in compiler attribute validation."
    }
}

struct CollectIdentVisitor<'a> {
    names: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for CollectIdentVisitor<'a> {
    fn visit_ident(&mut self, ident: &'a Ident) {
        self.names.insert(ident.to_string());
    }
}

fn mutate_attributes(item: &mut Item, existing_names: &mut HashSet<String>) {
    let attrs = match item {
        Item::Fn(item_fn) => &mut item_fn.attrs,
        Item::Struct(item_struct) => &mut item_struct.attrs,
        Item::Enum(item_enum) => &mut item_enum.attrs,
        Item::Trait(item_trait) => &mut item_trait.attrs,
        Item::Impl(item_impl) => &mut item_impl.attrs,
        Item::Mod(item_mod) => &mut item_mod.attrs,
        Item::ForeignMod(item_foreign) => &mut item_foreign.attrs,
        Item::Static(item_static) => &mut item_static.attrs,
        Item::Const(item_const) => &mut item_const.attrs,
        Item::Type(item_type) => &mut item_type.attrs,
        Item::Union(item_union) => &mut item_union.attrs,
        Item::Macro(item_macro) => &mut item_macro.attrs,
        _ => return,
    };
    
    let mut new_attrs = Vec::new();
    for attr in attrs.iter() {
        if let Some(attr_name) = extract_builtin_attribute_name(attr) {
            let dummy_ident = generate_unique_dummy(&attr_name, existing_names);
            let mutated_path = format!("{}::{}", attr_name, dummy_ident);
            if let Ok(path) = syn::parse_str::<syn::Path>(&mutated_path) {
                let new_attr = syn::Attribute {
                    pound_token: attr.pound_token,
                    style: attr.style,
                    bracket_token: attr.bracket_token,
                    meta: syn::Meta::Path(path),
                };
                new_attrs.push(new_attr);
            } else {
                new_attrs.push(attr.clone());
            }
        } else {
            new_attrs.push(attr.clone());
        }
    }
    *attrs = new_attrs;
}

fn extract_builtin_attribute_name(attr: &syn::Attribute) -> Option<String> {
    if let syn::Meta::Path(path) = &attr.meta {
        if path.segments.len() == 1 {
            let ident = &path.segments[0].ident;
            let name = ident.to_string();
            if is_builtin_attribute(&name) {
                return Some(name);
            }
        }
    }
    None
}

fn is_builtin_attribute(name: &str) -> bool {
    let builtins = ["cfg", "derive", "allow", "deny", "warn", "forbid", "deprecated", "must_use", "inline", "no_mangle", "repr", "derive", "test", "ignore", "should_panic", "no_main", "feature", "macro_use", "macro_export"];
    builtins.contains(&name)
}

fn generate_unique_dummy(base: &str, existing_names: &mut HashSet<String>) -> String {
    let mut dummy = format!("dummy_{}", base);
    while existing_names.contains(&dummy) {
        dummy = format!("{}_mutated", dummy);
    }
    existing_names.insert(dummy.clone());
    dummy
}