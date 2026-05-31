use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Avoid_Glob_Import_Cycles_664;

impl Mutator for Avoid_Glob_Import_Cycles_664 {
    fn name(&self) -> &str {
        "Avoid_Glob_Import_Cycles_664"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("m") {
                    if let Ok(block) = syn::parse2::<Block>(mac.mac.tokens.clone()) {
                        for stmt in block.stmts {
                            if let Stmt::Item(Item::Mod(item_mod)) = stmt {
                                for mod_item in &item_mod.content.as_ref().unwrap().1 {
                                    if let Item::Use(item_use) = mod_item {
                                        if let UseTree::Glob(_) = item_use.tree {
                                            let new_use_tree: UseTree = parse_quote! {
                                                {Entity1, Entity2}
                                            };
                                            let new_item_use = ItemUse {
                                                attrs: item_use.attrs.clone(),
                                                vis: item_use.vis.clone(),
                                                use_token: item_use.use_token,
                                                leading_colon: item_use.leading_colon,
                                                tree: new_use_tree,
                                            };
                                            *item_use = new_item_use;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}