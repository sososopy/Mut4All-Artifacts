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

pub struct Modify_Function_Pointer_Usage_In_Const_Generics_406;

impl Mutator for Modify_Function_Pointer_Usage_In_Const_Generics_406 {
    fn name(&self) -> &str {
        "Modify_Function_Pointer_Usage_In_Const_Generics_406"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.generics.const_params().count() == 1 {
                    let const_param = item_struct.generics.const_params().next().unwrap();
                    if let Type::Path(type_path) = &const_param.ty {
                        if type_path.path.is_ident("fn") {
                            let new_ident = Ident::new("SIZE", const_param.ident.span());
                            let new_params = item_struct
                                .generics
                                .params
                                .iter()
                                .map(|param| match param {
                                    GenericParam::Const(param) => {
                                        let mut new_param = param.clone();
                                        new_param.ty = parse_quote!(usize);
                                        GenericParam::Const(new_param)
                                    }
                                    _ => param.clone(),
                                })
                                .collect::<Vec<_>>();
                            item_struct.generics.params = Punctuated::from_iter(new_params);
                            item_struct.ident = Ident::new("Wrapper", item_struct.ident.span());
                            let mut visitor = ReplaceConstGenericVisitor {
                                old_ident: const_param.ident.clone(),
                                new_ident,
                            };
                            visitor.visit_item_struct_mut(item_struct);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with a single const generic parameter that uses a function pointer type. It replaces the function pointer with a usize constant and updates all references to the generic parameter within the struct's implementation. This transformation stresses the compiler's handling of const generics by altering the type and semantics of the generic parameter, potentially exposing issues in const evaluation, type resolution, and code generation."
    }
}

struct ReplaceConstGenericVisitor {
    old_ident: Ident,
    new_ident: Ident,
}

impl VisitMut for ReplaceConstGenericVisitor {
    fn visit_ident_mut(&mut self, ident: &mut Ident) {
        if *ident == self.old_ident {
            *ident = self.new_ident.clone();
        }
    }
    fn visit_expr_path_mut(&mut self, expr_path: &mut ExprPath) {
        if expr_path.path.is_ident(&self.old_ident) {
            expr_path.path = parse_quote!(#self.new_ident);
        }
        syn::visit_mut::visit_expr_path_mut(self, expr_path);
    }
    fn visit_impl_item_mut(&mut self, impl_item: &mut ImplItem) {
        if let ImplItem::Const(impl_const) = impl_item {
            if impl_const.ident == self.old_ident {
                impl_const.ident = self.new_ident.clone();
            }
        }
        syn::visit_mut::visit_impl_item_mut(self, impl_item);
    }
    fn visit_item_impl_mut(&mut self, item_impl: &mut ItemImpl) {
        if let Some((_, ref mut path, _)) = item_impl.trait_ {
            if path.is_ident(&self.old_ident) {
                *path = parse_quote!(#self.new_ident);
            }
        }
        syn::visit_mut::visit_item_impl_mut(self, item_impl);
    }
}