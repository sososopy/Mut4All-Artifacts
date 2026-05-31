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

pub struct Insert_Extern_Crate_After_Proc_Macro_214;

impl Mutator for Insert_Extern_Crate_After_Proc_Macro_214 {
    fn name(&self) -> &str {
        "Insert_Extern_Crate_After_Proc_Macro_214"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut items_to_insert = Vec::new();
        let mut proc_macro_positions = Vec::new();
        let mut extern_crates = Vec::new();
        let mut used_crates = HashSet::new();

        for (i, item) in file.items.iter().enumerate() {
            match item {
                Item::ExternCrate(extern_crate) => {
                    extern_crates.push((i, extern_crate.ident.to_string()));
                }
                Item::Use(use_item) => {
                    if let UseTree::Path(use_path) = &use_item.tree {
                        used_crates.insert(use_path.ident.to_string());
                    }
                }
                _ => {}
            }
        }

        for (i, item) in file.items.iter().enumerate() {
            if let Item::Fn(item_fn) = item {
                for attr in &item_fn.attrs {
                    if attr.path().is_ident("proc_macro_attribute") || attr.path().is_ident("proc_macro") || attr.path().is_ident("proc_macro_derive") {
                        proc_macro_positions.push(i);
                        break;
                    }
                }
            }
        }

        for proc_pos in proc_macro_positions {
            let mut target_crate = None;
            for (ext_pos, crate_name) in &extern_crates {
                if *ext_pos < proc_pos {
                    target_crate = Some(crate_name.clone());
                    break;
                }
            }
            if target_crate.is_none() {
                for crate_name in &used_crates {
                    target_crate = Some(crate_name.clone());
                    break;
                }
            }
            if let Some(crate_name) = target_crate {
                let new_extern_crate: Item = parse_quote! {
                    extern crate #crate_name;
                };
                items_to_insert.push((proc_pos + 1, new_extern_crate));
            }
        }

        items_to_insert.sort_by_key(|&(pos, _)| std::cmp::Reverse(pos));
        for (pos, item) in items_to_insert {
            file.items.insert(pos, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets procedural macro attributes and extern crate declarations. It identifies functions with proc macro attributes, determines relevant crate names from existing extern crate statements or use statements, and inserts a duplicate extern crate declaration after the attributed item. This disrupts the compiler's metadata loading order during resolution, potentially causing conflicts in macro expansion or crate resolution phases."
    }
}