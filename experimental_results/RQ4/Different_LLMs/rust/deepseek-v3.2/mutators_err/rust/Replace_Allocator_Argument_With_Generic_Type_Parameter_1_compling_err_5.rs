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

pub struct Replace_Allocator_Argument_With_Generic_Type_Parameter_1;

impl Mutator for Replace_Allocator_Argument_With_Generic_Type_Parameter_1 {
    fn name(&self) -> &str {
        "Replace_Allocator_Argument_With_Generic_Type_Parameter_1"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut allocator_calls = Vec::new();
        let mut visitor = AllocatorCallVisitor {
            calls: &mut allocator_calls,
        };
        visitor.visit_file_mut(file);
        for (call_expr, mut parent_block, stmt_index) in allocator_calls {
            let call_path = match &*call_expr.func {
                Expr::Path(path_expr) => &path_expr.path,
                _ => continue,
            };
            let last_segment = call_path.segments.last();
            if last_segment.is_none() {
                continue;
            }
            let last_segment = last_segment.unwrap();
            let function_name = &last_segment.ident;
            let allocator_api_functions = ["new_in", "with_capacity_in", "alloc", "dealloc", "grow", "shrink"];
            if !allocator_api_functions.contains(&function_name.to_string().as_str()) {
                continue;
            }
            if call_expr.args.len() < 2 {
                continue;
            }
            let allocator_arg_index = call_expr.args.len() - 1;
            let allocator_arg = &call_expr.args[allocator_arg_index];
            let allocator_type = match allocator_arg {
                Expr::Reference(ref_expr) => {
                    if let Expr::Path(path_expr) = &*ref_expr.expr {
                        Some(path_expr.path.clone())
                    } else {
                        None
                    }
                }
                _ => None,
            };
            let allocator_type = match allocator_type {
                Some(path) => path,
                None => continue,
            };
            let generic_param_name = Ident::new("A", Span::call_site());
            let mut existing_names = HashSet::new();
            for item in &file.items {
                collect_idents(item, &mut existing_names);
            }
            let mut param_name = generic_param_name;
            while existing_names.contains(&param_name.to_string()) {