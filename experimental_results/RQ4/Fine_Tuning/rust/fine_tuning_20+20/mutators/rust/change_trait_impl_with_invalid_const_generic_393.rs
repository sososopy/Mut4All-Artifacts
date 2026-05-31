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

pub struct Change_Trait_Impl_With_Invalid_Const_Generic_393;

impl Mutator for Change_Trait_Impl_With_Invalid_Const_Generic_393 {
    fn name(&self) -> &str {
        "Change_Trait_Impl_With_Invalid_Const_Generic_393"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        if last_segment.ident == "Drop" {
                            return;
                        }
                    }
                }
                let mut target_generic = None;
                for (index, generic) in item_impl.generics.params.iter().enumerate() {
                    if let GenericParam::Const(const_param) = generic {
                        if let Type::Path(type_path) = &const_param.ty {
                            if let Some(last_segment) = type_path.path.segments.last() {
                                if last_segment.ident == "usize"
                                    || last_segment.ident == "isize"
                                    || last_segment.ident == "u8"
                                    || last_segment.ident == "i8"
                                    || last_segment.ident == "u16"
                                    || last_segment.ident == "i16"
                                    || last_segment.ident == "u32"
                                    || last_segment.ident == "i32"
                                    || last_segment.ident == "u64"
                                    || last_segment.ident == "i64"
                                    || last_segment.ident == "u128"
                                    || last_segment.ident == "i128"
                                    || last_segment.ident == "bool"
                                    || last_segment.ident == "char"
                                {
                                    target_generic = Some(index);
                                    break;
                                }
                            }
                        }
                    }
                }
                if let Some(index) = target_generic {
                    let new_ty: Type = parse_quote!(f64);
                    if let Some(GenericParam::Const(const_param)) =
                        item_impl.generics.params.get_mut(index)
                    {
                        const_param.ty = new_ty.clone();
                    }
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.segments.last().unwrap().ident == "Drop" {
                            return;
                        }
                    }
                    let mut visitor = ReplaceConstGenericUsage {
                        target_index: index,
                        new_ty,
                        current_index: 0,
                    };
                    visitor.visit_item_impl_mut(item_impl);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that use const generics, specifically those with integer, bool, or char const generic parameters. It changes one of these parameters to a floating-point type, such as f64, which is not allowed for const generics. Additionally, it attempts to replace usages of this parameter within the implementation with the new floating-point type, aiming to create type mismatches or other compilation errors. This transformation is designed to stress-test the compiler's handling of const generics and type inference, potentially exposing bugs related to const evaluation, trait resolution, and type checking."
    }
}

struct ReplaceConstGenericUsage {
    target_index: usize,
    new_ty: Type,
    current_index: usize,
}

impl VisitMut for ReplaceConstGenericUsage {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Path(expr_path) = node {
            if expr_path.qself.is_none() && expr_path.path.leading_colon.is_none() {
                if let Some(ident) = expr_path.path.get_ident() {
                    if ident == "Self" {
                        return;
                    }
                }
                if expr_path.path.segments.len() == 1 {
                    if let Some(first_segment) = expr_path.path.segments.first() {
                        if first_segment.ident == "Self" {
                            return;
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_mut(self, node);
    }

    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            if type_path.qself.is_none() && type_path.path.leading_colon.is_none() {
                if let Some(ident) = type_path.path.get_ident() {
                    if ident == "Self" {
                        return;
                    }
                }
                if type_path.path.segments.len() == 1 {
                    if let Some(first_segment) = type_path.path.segments.first() {
                        if first_segment.ident == "Self" {
                            return;
                        }
                    }
                }
            }
        }
        visit_mut::visit_type_mut(self, node);
    }

    fn visit_generic_argument_mut(&mut self, node: &mut GenericArgument) {
        if let GenericArgument::Const(expr) = node {
            if let Expr::Path(expr_path) = expr {
                if expr_path.qself.is_none() && expr_path.path.leading_colon.is_none() {
                    if let Some(ident) = expr_path.path.get_ident() {
                        if ident == "Self" {
                            return;
                        }
                    }
                    if expr_path.path.segments.len() == 1 {
                        if let Some(first_segment) = expr_path.path.segments.first() {
                            if first_segment.ident == "Self" {
                                return;
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_generic_argument_mut(self, node);
    }

    fn visit_expr_path_mut(&mut self, node: &mut ExprPath) {
        if node.qself.is_none() && node.path.leading_colon.is_none() {
            if let Some(ident) = node.path.get_ident() {
                if ident == "Self" {
                    return;
                }
            }
            if node.path.segments.len() == 1 {
                if let Some(first_segment) = node.path.segments.first() {
                    if first_segment.ident == "Self" {
                        return;
                    }
                }
            }
        }
        visit_mut::visit_expr_path_mut(self, node);
    }

    fn visit_type_path_mut(&mut self, node: &mut TypePath) {
        if node.qself.is_none() && node.path.leading_colon.is_none() {
            if let Some(ident) = node.path.get_ident() {
                if ident == "Self" {
                    return;
                }
            }
            if node.path.segments.len() == 1 {
                if let Some(first_segment) = node.path.segments.first() {
                    if first_segment.ident == "Self" {
                        return;
                    }
                }
            }
        }
        visit_mut::visit_type_path_mut(self, node);
    }

    fn visit_generic_param_mut(&mut self, node: &mut GenericParam) {
        if let GenericParam::Const(const_param) = node {
            if self.current_index == self.target_index {
                const_param.ty = self.new_ty.clone();
            }
            self.current_index += 1;
        }
        visit_mut::visit_generic_param_mut(self, node);
    }
}