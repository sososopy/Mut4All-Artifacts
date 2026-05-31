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

pub struct Replace_MaybeUninit_Uninit_With_Zeroed_For_Uninhabited_215;

impl Mutator for Replace_MaybeUninit_Uninit_With_Zeroed_For_Uninhabited_215 {
    fn name(&self) -> &str {
        "Replace_MaybeUninit_Uninit_With_Zeroed_For_Uninhabited_215"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceMaybeUninitVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unsafe calls to `MaybeUninit::<T>::uninit().assume_init()` where T is an uninhabited type or contains a never type field. It replaces `uninit()` with `zeroed()`, altering the initialization pattern to expose potential bugs in GVN related to unreachable code or invalid dominator tree nodes. This mutation specifically tests edge cases in MIR optimizations involving uninhabited types and zero-initialization."
    }
}

struct ReplaceMaybeUninitVisitor {
    type_cache: HashSet<syn::Type>,
}

impl ReplaceMaybeUninitVisitor {
    fn new() -> Self {
        Self {
            type_cache: HashSet::new(),
        }
    }

    fn is_uninhabited_type(&mut self, ty: &syn::Type, file: &syn::File) -> bool {
        if self.type_cache.contains(ty) {
            return true;
        }

        let mut uninhabited = false;
        match ty {
            syn::Type::Path(type_path) => {
                let type_name = type_path.path.segments.last().map(|seg| &seg.ident);
                for item in &file.items {
                    match item {
                        syn::Item::Struct(item_struct) => {
                            if let Some(name) = type_name {
                                if item_struct.ident == *name {
                                    for field in &item_struct.fields {
                                        if self.is_never_type(&field.ty) {
                                            uninhabited = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        syn::Item::Enum(item_enum) => {
                            if let Some(name) = type_name {
                                if item_enum.ident == *name {
                                    if item_enum.variants.is_empty() {
                                        uninhabited = true;
                                    } else {
                                        for variant in &item_enum.variants {
                                            for field in &variant.fields {
                                                if self.is_never_type(&field.ty) {
                                                    uninhabited = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
            syn::Type::Never(_) => {
                uninhabited = true;
            }
            _ => {}
        }

        if uninhabited {
            self.type_cache.insert(ty.clone());
        }
        uninhabited
    }

    fn is_never_type(&self, ty: &syn::Type) -> bool {
        matches!(ty, syn::Type::Never(_))
    }
}

impl VisitMut for ReplaceMaybeUninitVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Call(call_expr) = expr {
            if let syn::Expr::MethodCall(method_call) = &*call_expr.func {
                if method_call.method == "assume_init" {
                    if let syn::Expr::Call(inner_call) = &*method_call.receiver {
                        if let syn::Expr::Path(path_expr) = &*inner_call.func {
                            if let Some(segment) = path_expr.path.segments.last() {
                                if segment.ident == "uninit" {
                                    if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                        if let Some(GenericArgument::Type(ty)) = args.args.first() {
                                            let file = syn::File {
                                                shebang: None,
                                                attrs: vec![],
                                                items: vec![],
                                            };
                                            if self.is_uninhabited_type(ty, &file) {
                                                let new_segment = syn::PathSegment {
                                                    ident: syn::Ident::new("zeroed", segment.ident.span()),
                                                    arguments: segment.arguments.clone(),
                                                };
                                                let mut new_path = path_expr.path.clone();
                                                new_path.segments.pop();
                                                new_path.segments.push(new_segment);
                                                let new_func = syn::Expr::Path(syn::ExprPath {
                                                    attrs: path_expr.attrs.clone(),
                                                    qself: None,
                                                    path: new_path,
                                                });
                                                let mut new_inner_call = inner_call.clone();
                                                new_inner_call.func = Box::new(new_func);
                                                let mut new_method_call = method_call.clone();
                                                new_method_call.receiver = Box::new(syn::Expr::Call(new_inner_call));
                                                call_expr.func = Box::new(syn::Expr::MethodCall(new_method_call));
                                            }
                                        }
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