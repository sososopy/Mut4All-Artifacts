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

pub struct Modify_Tuple_Access_To_Non_Existent_Fields_64;

impl Mutator for Modify_Tuple_Access_To_Non_Existent_Fields_64 {
    fn name(&self) -> &str {
        "Modify_Tuple_Access_To_Non_Existent_Fields_64"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tuple_map = std::collections::HashMap::new();
        let mut tuple_names = HashSet::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Tuple(tuple) = &*init.expr {
                                if let Pat::Ident(pat_ident) = &local.pat {
                                    tuple_map.insert(pat_ident.ident.to_string(), tuple.elems.len());
                                    tuple_names.insert(pat_ident.ident.to_string());
                                }
                            }
                        }
                    }
                }
            }
        }
        struct TupleAccessVisitor<'a> {
            tuple_map: &'a std::collections::HashMap<String, usize>,
            tuple_names: &'a HashSet<String>,
        }
        impl<'a> VisitMut for TupleAccessVisitor<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Field(expr_field) = expr {
                    if let Expr::Path(expr_path) = &*expr_field.base {
                        if let Some(ident) = expr_path.path.get_ident() {
                            if self.tuple_names.contains(&ident.to_string()) {
                                if let Member::Unnamed(index) = &expr_field.member {
                                    if let Some(&tuple_len) = self.tuple_map.get(&ident.to_string())
                                    {
                                        if index.index as usize >= tuple_len {
                                            return;
                                        }
                                    }
                                }
                                if let Some(&tuple_len) = self.tuple_map.get(&ident.to_string()) {
                                    expr_field.member = Member::Unnamed(Index {
                                        index: tuple_len as u32,
                                        span: Span::call_site(),
                                    });
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = TupleAccessVisitor {
                    tuple_map: &tuple_map,
                    tuple_names: &tuple_names,
                };
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets tuple field accesses in Rust code. It first identifies all tuple declarations and their sizes by analyzing local variable initializations. Then, it traverses the AST to find field accesses on these tuples. If a field access is within the valid range of the tuple's fields, it modifies the access to target a non-existent field index, specifically the first out-of-bounds index. This transformation is designed to provoke errors related to tuple field access, challenging the compiler's handling of tuple indexing and error reporting for invalid accesses."
    }
}