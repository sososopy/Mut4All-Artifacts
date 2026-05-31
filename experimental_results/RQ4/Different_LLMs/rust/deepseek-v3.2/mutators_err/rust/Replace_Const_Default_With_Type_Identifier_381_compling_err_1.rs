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

pub struct Replace_Const_Default_With_Type_Identifier_381;

impl Mutator for Replace_Const_Default_With_Type_Identifier_381 {
    fn name(&self) -> &str {
        "Replace_Const_Default_With_Type_Identifier_381"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut available_types = HashSet::new();
        struct TypeCollector<'a> {
            types: &'a mut HashSet<String>,
        }
        impl<'ast> Visit<'ast> for TypeCollector<'ast> {
            fn visit_type_path(&mut self, node: &'ast TypePath) {
                if let Some(segment) = node.path.segments.last() {
                    self.types.insert(segment.ident.to_string());
                }
                syn::visit::visit_type_path(self, node);
            }
            fn visit_item_struct(&mut self, node: &'ast ItemStruct) {
                self.types.insert(node.ident.to_string());
                syn::visit::visit_item_struct(self, node);
            }
        }
        let mut collector = TypeCollector {
            types: &mut available_types,
        };
        collector.visit_file(file);
        let primitive_types = vec!["usize", "u32", "bool", "i32", "u64", "i64", "char", "f32", "f64"];
        for prim in primitive_types {
            available_types.insert(prim.to_string());
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_params_with_defaults = Vec::new();
                for (idx, param) in item_struct.generics.params.iter().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            const_params_with_defaults.push(idx);
                        }
                    }
                }
                if !const_params_with_defaults.is_empty() {
                    let mut rng = thread_rng();
                    let chosen_idx = const_params_with_defaults.choose(&mut rng);
                    if let Some(&idx) = chosen_idx {
                        if let GenericParam::Const(const_param) = &mut item_struct.generics.params[idx] {
                            let candidate_types: Vec<String> = available_types.iter().cloned().collect();
                            if let Some(chosen_type_name) = candidate_types.choose(&mut rng) {
                                let type_ident = Ident::new(chosen_type_name, Span::call_site());
                                const_param.default = Some(parse_quote!(#type_ident));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with const generic parameters that have default values. It replaces the default expression with a type identifier (e.g., usize, bool, or a user-defined type) that is syntactically valid but semantically invalid as a const expression. This transformation aims to provoke const evaluation errors, type mismatches, and compiler ICEs by introducing type identifiers where const expressions are expected, testing the compiler's ability to handle malformed const generic defaults."
    }
}