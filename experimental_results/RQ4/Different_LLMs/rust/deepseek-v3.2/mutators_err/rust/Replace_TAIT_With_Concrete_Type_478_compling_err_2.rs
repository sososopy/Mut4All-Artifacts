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

pub struct Replace_TAIT_With_Concrete_Type_478;

impl Mutator for Replace_TAIT_With_Concrete_Type_478 {
    fn name(&self) -> &str {
        "Replace_TAIT_With_Concrete_Type_478"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tait_aliases = Vec::new();
        let mut concrete_types = HashSet::new();
        
        struct TypeCollector {
            concrete_types: HashSet<syn::Type>,
        }
        
        impl<'ast> Visit<'ast> for TypeCollector {
            fn visit_type(&mut self, ty: &'ast syn::Type) {
                match ty {
                    syn::Type::Path(type_path) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Slice(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Array(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Ptr(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Reference(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Tuple(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::BareFn(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Never(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Group(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Paren(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::Macro(_) => {
                        self.concrete_types.insert(ty.clone());
                    }
                    syn::Type::ImplTrait(_) => {}
                    syn::Type::TraitObject(_) => {}
                    syn::Type::Infer(_) => {}
                    syn::Type::Verbatim(_) => {}
                    _ => {}
                }
                syn::visit::visit_type(self, ty);
            }
        }
        
        let mut collector = TypeCollector {
            concrete_types: HashSet::new(),
        };
        collector.visit_file(file);
        concrete_types = collector.concrete_types;
        
        for item in &file.items {
            if let syn::Item::Type(type_item) = item {
                if let syn::Type::ImplTrait(_) = &*type_item.ty {
                    tait_aliases.push((type_item.ident.clone(), type_item.ty.clone()));
                }
            }
        }
        
        for (tait_name, tait_type) in tait_aliases {
            let mut candidate_types = Vec::new();
            
            struct TaitUsageFinder {
                tait_name: Ident,
                candidate_types: Vec<syn::Type>,
            }
            
            impl<'ast> Visit<'ast> for TaitUsageFinder {
                fn visit_type(&mut self, ty: &'ast syn::Type) {
                    if let syn::Type::Path(type_path) = ty {
                        if let Some(segment) = type_path.path.segments.last() {
                            if segment.ident == self.tait_name {
                                if let Some(parent_type) = self.get_parent_type() {
                                    self.candidate_types.push(parent_type.clone());
                                }
                            }
                        }
                    }
                    syn::visit::visit_type(self, ty);
                }
                
                fn visit_expr(&mut self, expr: &'ast syn::Expr) {
                    if let syn::Expr::Return(expr_return) = expr {
                        if let Some(inner_expr) = &expr_return.expr {
                            if let syn::Expr::Path(expr_path) = &**inner_expr {
                                if let Some(segment) = expr_path.path.segments.last() {
                                    if segment.ident == self.tait_name {
                                        if let Some(parent_type) = self.get_parent_type() {
                                            self.candidate_types.push(parent_type.clone());
                                        }
                                    }
                                }
                            }
                        }
                    }
                    syn::visit::visit_expr(self, expr);
                }
            }
            
            impl TaitUsageFinder {
                fn get_parent_type(&self) -> Option<syn::Type> {
                    None
                }
            }
            
            let mut finder = TaitUsageFinder {
                tait_name: tait_name.clone(),
                candidate_types: Vec::new(),
            };
            finder.visit_file(file);
            
            for ty in &concrete_types {
                if let syn::Type::Path(type_path) = ty {
                    candidate_types.push(ty.clone());
                }
            }
            
            if !candidate_types.is_empty() {
                let mut rng = thread_rng();
                if let Some(selected_type) = candidate_types.choose(&mut rng) {
                    for item in &mut file.items {
                        if let syn::Item::Type(type_item) = item {
                            if type_item.ident == tait_name {
                                if let syn::Type::ImplTrait(type_impl_trait) = &*type_item.ty {
                                    type_item.ty = Box::new(selected_type.clone());
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies type alias impl trait (TAIT) definitions and replaces them with concrete types already present in the seed program. It first collects all concrete types in the program, then for each TAIT alias, it looks for usage contexts that might suggest compatible concrete types. If suitable candidates are found, it randomly selects one and replaces the TAIT definition. This transformation tests the compiler's TAIT resolution by forcing explicit type substitution, potentially revealing inconsistencies in trait resolution, type inference, or monomorphization when opaque types are made concrete."
    }
}