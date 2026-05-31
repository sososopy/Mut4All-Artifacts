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

pub struct Replace_Const_Generic_Array_Length_With_Loop_Break_107;

impl Mutator for Replace_Const_Generic_Array_Length_With_Loop_Break_107 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Array_Length_With_Loop_Break_107"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let generics = item_fn.sig.generics.clone();
                let mut visitor = ConstGenericArrayLengthVisitor {
                    generics: &generics,
                };
                visitor.visit_item_fn_mut(item_fn);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let generics = method.sig.generics.clone();
                        let mut visitor = ConstGenericArrayLengthVisitor {
                            generics: &generics,
                        };
                        visitor.visit_impl_item_fn_mut(method);
                    }
                }
            }
            if let Item::Struct(item_struct) = item {
                let generics = item_struct.generics.clone();
                let mut visitor = ConstGenericArrayLengthVisitor {
                    generics: &generics,
                };
                visitor.visit_item_struct_mut(item_struct);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces direct usage of const generic parameters as array length specifiers with a constant expression containing a loop with a break statement. This transformation targets array type definitions in return types, parameter types, and variable declarations. It introduces complex control flow within constant expressions while preserving the same integer type, potentially exposing bugs in const evaluation, loop analysis, or array type validation."
    }
}

struct ConstGenericArrayLengthVisitor<'a> {
    generics: &'a Generics,
}

impl<'a> VisitMut for ConstGenericArrayLengthVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Array(type_array) = node {
            if let Expr::Path(expr_path) = &type_array.len {
                let path = &expr_path.path;
                if path.segments.len() == 1 {
                    let ident = &path.segments[0].ident;
                    if self.generics.params.iter().any(|param| {
                        if let GenericParam::Const(const_param) = param {
                            const_param.ident == *ident
                        } else {
                            false
                        }
                    }) {
                        let new_expr: Expr = parse_quote! { { loop { break #ident; } } };
                        type_array.len = Box::new(new_expr);
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }

    fn visit_fn_arg_mut(&mut self, node: &mut FnArg) {
        if let FnArg::Typed(pat_type) = node {
            self.visit_type_mut(&mut *pat_type.ty);
        }
        syn::visit_mut::visit_fn_arg_mut(self, node);
    }

    fn visit_return_type_mut(&mut self, node: &mut ReturnType) {
        if let ReturnType::Type(_, ty) = node {
            self.visit_type_mut(&mut **ty);
        }
        syn::visit_mut::visit_return_type_mut(self, node);
    }

    fn visit_local_mut(&mut self, node: &mut Local) {
        if let Some(init) = &mut node.init {
            self.visit_expr_mut(&mut *init.expr);
        }
        syn::visit_mut::visit_local_mut(self, node);
    }
}