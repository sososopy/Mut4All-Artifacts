use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Modify_Box_Init_With_System_Allocator_5;

impl Mutator for Modify_Box_Init_With_System_Allocator_5 {
    fn name(&self) -> &str {
        "Modify_Box_Init_With_System_Allocator_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut uses_system = false;
        
        for item in &file.items {
            if let Item::Use(use_item) = item {
                if let UseTree::Path(use_path) = &use_item.tree {
                    if use_path.ident == "std" {
                        if let UseTree::Path(sub_path) = &*use_path.tree {
                            if sub_path.ident == "alloc" {
                                if let UseTree::Name(name) = &*sub_path.tree {
                                    if name.ident == "System" {
                                        uses_system = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if !uses_system {
            file.items.insert(0, parse_quote! { use std::alloc::System; });
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &mut local.init {
                            if let Expr::Call(call) = &mut **expr {
                                if let Expr::Path(path) = &*call.func {
                                    if path.path.segments.len() == 2 {
                                        if path.path.segments[0].ident == "Box" && path.path.segments[1].ident == "new_in" {
                                            if let Some(last_arg) = call.args.last_mut() {
                                                *last_arg = parse_quote!(&System);
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}