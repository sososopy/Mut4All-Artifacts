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

use std::collections::HashMap;

use crate::mutator::Mutator;

pub struct Modify_Const_Initialization_With_Trait_Bound_Violation_57;

impl Mutator for Modify_Const_Initialization_With_Trait_Bound_Violation_57 {
    fn name(&self) -> &str {
        "Modify_Const_Initialization_With_Trait_Bound_Violation_57"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TraitBoundCollector {
            bound_map: HashMap<String, Vec<String>>,
        }
        impl<'ast> Visit<'ast> for TraitBoundCollector {
            fn visit_item_struct(&mut self, node: &'ast ItemStruct) {
                let struct_name = node.ident.to_string();
                let mut bounds = Vec::new();
                for param in &node.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let bound_name = trait_bound
                                    .path
                                    .segments
                                    .last()
                                    .unwrap()
                                    .ident
                                    .to_string();
                                bounds.push(bound_name);
                            }
                        }
                    }
                }
                self.bound_map.insert(struct_name, bounds);
            }
            fn visit_item_type(&mut self, node: &'ast ItemType) {
                let type_name = node.ident.to_string();
                if let Type::Path(type_path) = &*node.ty {
                    let struct_name = type_path
                        .path
                        .segments
                        .last()
                        .unwrap()
                        .ident
                        .to_string();
                    let struct_bounds = self.bound_map.get(&struct_name).cloned().unwrap_or_default();
                    self.bound_map
                        .entry(type_name)
                        .or_insert_with(Vec::new)
                        .extend(struct_bounds);
                }
            }
        }
        struct ConstInitializerRewriter<'a> {
            bound_map: &'a HashMap<String, Vec<String>>,
        }
        impl VisitMut for ConstInitializerRewriter<'_> {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if let Expr::Path(expr_path) = &*node.func {
                    if let Some(last_segment) = expr_path.path.segments.last() {
                        let struct_name = last_segment.ident.to_string();
                        if let Some(bounds) = self.bound_map.get(&struct_name) {
                            if bounds.contains(&"Trait".to_string()) {
                                if let Some(last_arg) = node.args.last_mut() {
                                    *last_arg = parse_quote!(42);
                                }
                            }
                        }
                    }
                }
            }
        }
        let mut collector = TraitBoundCollector {
            bound_map: HashMap::new(),
        };
        collector.visit_file(file);
        let mut rewriter = ConstInitializerRewriter {
            bound_map: &collector.bound_map,
        };
        rewriter.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets const initializations involving structs or type aliases with trait bounds. It collects all struct and type alias definitions with their trait bounds, especially focusing on those with a `Trait` bound. Then, it rewrites the const initializer expressions to violate the trait bound, specifically by replacing the last argument with a value that does not satisfy the bound (e.g., `42` for a `Trait` bound). This transformation aims to induce type checking failures or ICEs by breaking the expected trait constraints in const contexts."
    }
}