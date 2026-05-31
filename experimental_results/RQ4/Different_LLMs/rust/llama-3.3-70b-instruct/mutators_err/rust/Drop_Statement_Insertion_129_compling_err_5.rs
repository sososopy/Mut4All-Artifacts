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

pub struct Drop_Statement_Insertion_129;

impl Mutator for Drop_Statement_Insertion_129 {
    fn name(&self) -> &str {
        "Drop_Statement_Insertion_129"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut drop_types = HashSet::new();

        for item in &file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    if path.is_ident("Drop") {
                        if let Type::Path(type_path) = &*impl_item.self_ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                drop_types.insert(segment.ident.clone());
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Path(path) = &**init {
                                if let Some(segment) = path.path.segments.last() {
                                    if drop_types.contains(&segment.ident) {
                                        let drop_stmt = Stmt::Expr(
                                            parse_quote!(drop(#local.pat)),
                                            None,
                                        );
                                        func.block.stmts.insert(func.block.stmts.len(), drop_stmt);
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
        "The mutation operator targets the structures where drop statements are used, particularly focusing on areas where the compiler handles drop implementations for types. It applies to code constructs that involve types implementing the Drop trait, which is commonly used for resource management and cleanup."
    }
}