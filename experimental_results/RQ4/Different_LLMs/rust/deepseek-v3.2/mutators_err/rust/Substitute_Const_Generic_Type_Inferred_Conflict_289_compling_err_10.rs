use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Substitute_Const_Generic_Type_Inferred_Conflict_289;

impl Mutator for Substitute_Const_Generic_Type_Inferred_Conflict_289 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Type_Inferred_Conflict_289"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut user_types = HashSet::new();
        let mut primitive_types = HashSet::new();
        let mut const_params = Vec::new();
        let mut rng = thread_rng();

        struct TypeCollector<'a> {
            user_types: &'a mut HashSet<Ident>,
            primitive_types: &'a mut HashSet<Ident>,
        }

        impl<'ast> Visit<'ast> for TypeCollector<'ast> {
            fn visit_item(&mut self, item: &'ast Item) {
                if let Item::Struct(ItemStruct { ident, .. }) = item {
                    self.user_types.insert(ident.clone());
                }
                if let Item::Enum(item_enum) = item {
                    self.user_types.insert(item_enum.ident.clone());
                }
                syn::visit::visit_item(self, item);
            }

            fn visit_type(&mut self, ty: &'ast Type) {
                if let Type::Path(TypePath { path, .. }) = ty {
                    if let Some(segment) = path.segments.last() {
                        let ident_str = segment.ident.to_string();
                        if matches!(
                            ident_str.as_str(),
                            "bool" | "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
                            "u8" | "u16" | "u32" | "u64" | "u128" | "usize" |
                            "f32" | "f64" | "char"
                        ) {
                            self.primitive_types.insert(segment.ident.clone());
                        }
                    }
                }
                syn::visit::visit_type(self, ty);
            }
        }

        let mut collector = TypeCollector {
            user_types: &mut user_types,
            primitive_types: &mut primitive_types,
        };
        collector.visit_file(file);

        struct ConstParamCollector<'a> {
            const_params: &'a mut Vec<(GenericParam, Span, Option<Ident>)>,
        }

        impl<'ast> Visit<'ast> for ConstParamCollector<'ast> {
            fn visit_generic_param(&mut self, param: &'ast GenericParam) {
                if let GenericParam::Const(const_param) = param {
                    if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                        if let Some(segment) = path.segments.last() {
                            let ident_str = segment.ident.to_string();
                            if !matches!(
                                ident_str.as_str(),
                                "bool" | "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
                                "u8" | "u16" | "u32" | "u64" | "u128" | "usize" |
                                "f32" | "f64" | "char"
                            ) {
                                self.const_params.push((
                                    param.clone(),
                                    param.span(),
                                    Some(segment.ident.clone()),
                                ));
                            }
                        }
                    }
                }
                syn::visit::visit_generic_param(self, param);
            }
        }

        let mut param_collector = ConstParamCollector {
            const_params: &mut const_params,
        };
        param_collector.visit_file(file);

        if const_params.is_empty() {
            return;
        }

        let target_param = const_params.choose(&mut rng).unwrap();
        let original_type_ident = target_param.2.as_ref().unwrap();

        let mut candidate_types = Vec::new();
        candidate_types.extend(primitive_types.iter().cloned());
        candidate_types.extend(user_types.iter().filter(|&ident| ident != original_type_ident).cloned());

        if candidate_types.is_empty() {
            return;
        }

        let new_type_ident = candidate_types.choose(&mut rng).unwrap();

        struct ConstParamReplacer<'a> {
            original_type_ident: &'a Ident,
            new_type_ident: &'a Ident,
        }

        impl VisitMut for ConstParamReplacer<'_> {
            fn visit_generic_param_mut(&mut self, param: &mut GenericParam) {
                if let GenericParam::Const(const_param) = param {
                    if let Type::Path(TypePath { path, .. }) = &mut const_param.ty {
                        if let Some(segment) = path.segments.last_mut() {
                            if segment.ident == *self.original_type_ident {
                                segment.ident = self.new_type_ident.clone();
                            }
                        }
                    }
                }
                syn::visit_mut::visit_generic_param_mut(self, param);
            }

            fn visit_type_mut(&mut self, ty: &mut Type) {
                if let Type::Path(TypePath { path, .. }) = ty {
                    if let Some(segment) = path.segments.last_mut() {
                        if segment.ident == *self.original_type_ident {
                            segment.ident = self.new_type_ident.clone();
                        }
                    }
                }
                syn::visit_mut::visit_type_mut(self, ty);
            }
        }

        let mut replacer = ConstParamReplacer {
            original_type_ident,
            new_type_ident,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes const generic parameter types from user-defined types to other existing types (preferring primitives) in structs, impls, and functions. This creates type conflicts during const inference, particularly in trait resolution contexts, potentially triggering ICEs similar to reported bugs where mismatched const types cause compiler confusion."
    }
}