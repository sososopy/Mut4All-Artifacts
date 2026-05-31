use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemType, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_For_Binder_Assoc_Type_442;

impl Mutator for Replace_For_Binder_Assoc_Type_442 {
    fn name(&self) -> &str {
        "Replace_For_Binder_Assoc_Type_442"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_types = Vec::new();
        let mut rng = thread_rng();
        
        for item in &file.items {
            match item {
                Item::Type(item_type) => {
                    concrete_types.push(item_type.ident.clone());
                }
                Item::Struct(item_struct) => {
                    concrete_types.push(item_struct.ident.clone());
                }
                Item::Enum(item_enum) => {
                    concrete_types.push(item_enum.ident.clone());
                }
                _ => {}
            }
        }
        
        let primitive_types = vec![
            Ident::new("i32", Span::call_site()),
            Ident::new("u32", Span::call_site()),
            Ident::new("bool", Span::call_site()),
            Ident::new("char", Span::call_site()),
            Ident::new("String", Span::call_site()),
        ];
        
        concrete_types.extend(primitive_types);
        
        let mut modifications = Vec::new();
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut visitor = ForBinderVisitor {
                            concrete_types: &concrete_types,
                            rng: &mut rng,
                            modified: false,
                        };
                        visitor.visit_type_impl_trait_mut(type_impl_trait);
                        
                        if visitor.modified && !concrete_types.is_empty() {
                            modifications.push(true);
                        }
                    }
                }
            }
        }
        
        if modifications.iter().any(|&m| m) {
            let new_type_alias: Item = parse_quote! {
                type Concrete = i32;
            };
            file.items.insert(0, new_type_alias);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning impl Trait with higher-ranked for<> binders. It identifies associated type projections within the impl Trait that use type parameters bound by the for<> binder and replaces them with concrete types from scope or introduces new type aliases. This transformation creates mismatches in bound variable contexts, potentially triggering ICEs in the resolve_bound_vars query by confusing binder resolution logic."
    }
}

struct ForBinderVisitor<'a> {
    concrete_types: &'a Vec<Ident>,
    rng: &'a mut rand::rngs::ThreadRng,
    modified: bool,
}

impl<'a> VisitMut for ForBinderVisitor<'a> {
    fn visit_type_path_mut(&mut self, node: &mut TypePath) {
        for segment in &mut node.path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty) = arg {
                        self.visit_type_mut(ty);
                    }
                }
            }
        }
    }
    
    fn visit_type_mut(&mut self, node: &mut Type) {
        match node {
            Type::Path(type_path) => {
                if let Some(last_segment) = type_path.path.segments.last_mut() {
                    if last_segment.ident == "Assoc" {
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Type(ty) = arg {
                                    if let Type::Path(inner_path) = ty {
                                        if inner_path.path.segments.len() == 1 {
                                            let param_name = inner_path.path.segments[0].ident.to_string();
                                            if param_name.chars().next().map_or(false, |c| c.is_uppercase()) {
                                                if !self.concrete_types.is_empty() {
                                                    let new_type = self.concrete_types.choose(self.rng).unwrap().clone();
                                                    *ty = Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath::from(new_type),
                                                    });
                                                    self.modified = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                self.visit_type_path_mut(type_path);
            }
            Type::ImplTrait(type_impl_trait) => {
                for bound in &mut type_impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        self.visit_trait_bound_mut(trait_bound);
                    }
                }
            }
            _ => {}
        }
    }
    
    fn visit_trait_bound_mut(&mut self, node: &mut TraitBound) {
        self.visit_path_mut(&mut node.path);
    }
}