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

pub struct Replace_Enum_Const_With_Slice_67;

impl Mutator for Replace_Enum_Const_With_Slice_67 {
    fn name(&self) -> &str {
        "Replace_Enum_Const_With_Slice_67"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut enum_constants = Vec::new();
        let mut enum_types_with_complex_variant = HashSet::new();
        
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    if let Some(last_segment) = type_path.path.segments.last() {
                        let enum_name = &last_segment.ident;
                        if let Expr::Path(expr_path) = &*item_const.expr {
                            if expr_path.path.segments.len() == 1 {
                                let first_segment = &expr_path.path.segments[0];
                                if first_segment.ident == *enum_name {
                                    enum_constants.push((item_const.ident.clone(), enum_name.clone(), item_const.expr.clone()));
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Enum(item_enum) = item {
                for variant in &item_enum.variants {
                    if let syn::Fields::Unnamed(fields) = &variant.fields {
                        if fields.unnamed.len() > 0 {
                            enum_types_with_complex_variant.insert(item_enum.ident.clone());
                        }
                    }
                }
            }
        }
        
        for (const_name, enum_name, expr) in enum_constants {
            if enum_types_with_complex_variant.contains(&enum_name) {
                let new_const_name = Ident::new(&format!("{}_SLICE", const_name), Span::call_site());
                let new_const_item: Item = parse_quote! {
                    const #new_const_name: &[#enum_name] = &[#expr];
                };
                
                for item in &mut file.items {
                    if let Item::Const(item_const) = item {
                        if item_const.ident == const_name {
                            *item = new_const_item;
                            break;
                        }
                    }
                }
                
                let mut visitor = PatternReplacer {
                    old_const: const_name.clone(),
                    new_const: new_const_name.clone(),
                };
                visitor.visit_file_mut(file);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies constants of enum types that have at least one variant with fields (non-structural-match). It replaces such constants with a slice constant containing the original enum constant. Then it updates pattern matches to use the slice constant instead of the original constant. This transformation introduces slice patterns and may trigger const-eval issues with non-structural-match enums in slice contexts."
    }
}

struct PatternReplacer {
    old_const: Ident,
    new_const: Ident,
}

impl VisitMut for PatternReplacer {
    fn visit_pat_mut(&mut self, pat: &mut Pat) {
        if let Pat::Ident(pat_ident) = pat {
            if pat_ident.ident == self.old_const {
                let new_const = self.new_const.clone();
                *pat = parse_quote!(&[#new_const]);
            }
        }
        syn::visit_mut::visit_pat_mut(self, pat);
    }
}