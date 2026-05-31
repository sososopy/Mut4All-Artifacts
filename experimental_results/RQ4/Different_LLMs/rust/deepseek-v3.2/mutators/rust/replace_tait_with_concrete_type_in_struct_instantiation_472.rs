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

pub struct Replace_TAIT_With_Concrete_Type_In_Struct_Instantiation_472;

impl Mutator for Replace_TAIT_With_Concrete_Type_In_Struct_Instantiation_472 {
    fn name(&self) -> &str {
        "Replace_TAIT_With_Concrete_Type_In_Struct_Instantiation_472"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tait_aliases = Vec::new();
        let mut concrete_types = Vec::new();
        let mut struct_instantiations = Vec::new();
        
        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(_) = &*type_item.ty {
                    tait_aliases.push(type_item.ident.to_string());
                }
            }
            if let Item::Struct(struct_item) = item {
                for field in &struct_item.fields {
                    collect_concrete_types(&field.ty, &mut concrete_types);
                }
            }
            if let Item::Fn(fn_item) = item {
                struct_instantiations.extend(collect_struct_instantiations(&fn_item.block));
            }
        }
        
        if tait_aliases.is_empty() || concrete_types.is_empty() || struct_instantiations.is_empty() {
            return;
        }
        
        let mut rng = thread_rng();
        let target_tait = tait_aliases.choose(&mut rng).unwrap();
        let replacement_type = concrete_types.choose(&mut rng).unwrap();
        
        for item in &mut file.items {
            if let Item::Fn(fn_item) = item {
                mutate_struct_instantiations(&mut fn_item.block, target_tait, replacement_type);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies type alias impl trait (TAIT) definitions and struct instantiations that use these aliases as generic arguments. It replaces the TAIT with a concrete type from the seed program during struct instantiation, creating a mismatch between the TAIT's inferred type and the concrete type used. This transformation targets compiler unification bugs in trait projection and type inference for opaque types, particularly in scenarios involving generic structs with closure bounds."
    }
}

fn collect_concrete_types(ty: &Type, types: &mut Vec<String>) {
    match ty {
        Type::Path(type_path) => {
            if let Some(segment) = type_path.path.segments.last() {
                let type_name = segment.ident.to_string();
                if is_primitive_type(&type_name) || type_name.chars().next().map_or(false, |c| c.is_uppercase()) {
                    types.push(type_name);
                }
            }
        }
        Type::Slice(type_slice) => {
            collect_concrete_types(&type_slice.elem, types);
        }
        Type::Array(type_array) => {
            collect_concrete_types(&type_array.elem, types);
        }
        Type::Ptr(type_ptr) => {
            collect_concrete_types(&type_ptr.elem, types);
        }
        Type::Reference(type_ref) => {
            collect_concrete_types(&type_ref.elem, types);
        }
        Type::Tuple(type_tuple) => {
            for elem in &type_tuple.elems {
                collect_concrete_types(elem, types);
            }
        }
        _ => {}
    }
}

fn is_primitive_type(name: &str) -> bool {
    matches!(name, "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
                   "u8" | "u16" | "u32" | "u64" | "u128" | "usize" |
                   "f32" | "f64" | "bool" | "char" | "str" | "String")
}

fn collect_struct_instantiations(block: &Block) -> Vec<(Expr, usize)> {
    let mut collector = StructInstantiationCollector {
        instantiations: Vec::new(),
        depth: 0,
    };
    collector.visit_block(block);
    collector.instantiations
}

struct StructInstantiationCollector {
    instantiations: Vec<(Expr, usize)>,
    depth: usize,
}

impl Visit<'_> for StructInstantiationCollector {
    fn visit_expr(&mut self, expr: &Expr) {
        if let Expr::Struct(expr_struct) = expr {
            if let Some(segment) = expr_struct.path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    if !args.args.is_empty() {
                        self.instantiations.push((expr.clone(), self.depth));
                    }
                }
            }
        }
        
        let old_depth = self.depth;
        self.depth += 1;
        syn::visit::visit_expr(self, expr);
        self.depth = old_depth;
    }
}

fn mutate_struct_instantiations(block: &mut Block, target_tait: &str, replacement_type: &str) {
    let mut visitor = StructInstantiationMutator {
        target_tait: target_tait.to_string(),
        replacement_type: replacement_type.to_string(),
    };
    visitor.visit_block_mut(block);
}

struct StructInstantiationMutator {
    target_tait: String,
    replacement_type: String,
}

impl VisitMut for StructInstantiationMutator {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Struct(expr_struct) = expr {
            if let Some(segment) = expr_struct.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    if !args.args.is_empty() {
                        if let Some(GenericArgument::Type(type_arg)) = args.args.first_mut() {
                            if let Type::Path(type_path) = type_arg {
                                if let Some(type_segment) = type_path.path.segments.last() {
                                    if type_segment.ident == self.target_tait {
                                        let new_type: Type = parse_quote!(#(self.replacement_type));
                                        *type_arg = new_type;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}