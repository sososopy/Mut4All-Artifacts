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

pub struct Replace_Default_Const_Generic_With_Self_Referential_339;

impl Mutator for Replace_Default_Const_Generic_With_Self_Referential_339 {
    fn name(&self) -> &str {
        "Replace_Default_Const_Generic_With_Self_Referential_339"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    self.mutate_generics(&mut item_struct.generics, &item_struct.ident);
                }
                Item::Enum(item_enum) => {
                    self.mutate_generics(&mut item_enum.generics, &item_enum.ident);
                }
                Item::Fn(item_fn) => {
                    self.mutate_generics(&mut item_fn.sig.generics, &item_fn.sig.ident);
                }
                Item::Impl(item_impl) => {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            self.mutate_generics(&mut method.sig.generics, &method.sig.ident);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters with default values in structs, enums, functions, and methods. It replaces the default expression with a self-referential expression that depends on another instantiation of the same generic item, using accessible associated constants or variant discriminants. This creates complex circular dependencies that may confuse the compiler's const evaluation and substitution logic, potentially leading to ICEs, evaluation failures, or unexpected behavior when the default is used."
    }
}

impl Replace_Default_Const_Generic_With_Self_Referential_339 {
    fn mutate_generics(&self, generics: &mut Generics, ident: &Ident) {
        for param in &mut generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default) = &mut const_param.default {
                    let ty = &const_param.ty;
                    let item_name = ident.clone();
                    
                    let substitute_arg = self.generate_substitute_arg(ty);
                    let new_expr = self.create_self_referential_expr(&item_name, ty, &substitute_arg);
                    
                    *default = new_expr;
                }
            }
        }
    }
    
    fn generate_substitute_arg(&self, ty: &Type) -> Expr {
        match ty {
            Type::Path(type_path) => {
                if let Some(segment) = type_path.path.segments.last() {
                    match segment.ident.to_string().as_str() {
                        "u8" | "u16" | "u32" | "u64" | "u128" | "usize" => parse_quote! { 0 },
                        "i8" | "i16" | "i32" | "i64" | "i128" | "isize" => parse_quote! { 0 },
                        "bool" => parse_quote! { false },
                        "char" => parse_quote! { '\0' },
                        _ => parse_quote! { 0 },
                    }
                } else {
                    parse_quote! { 0 }
                }
            }
            _ => parse_quote! { 0 },
        }
    }
    
    fn create_self_referential_expr(&self, item_name: &Ident, ty: &Type, substitute_arg: &Expr) -> Expr {
        let arg_expr: Expr = parse_quote! { #substitute_arg };
        
        let left_path: Expr = parse_quote! {
            #item_name::<#arg_expr>
        };
        
        let right_path: Expr = parse_quote! {
            #item_name::<{#arg_expr as #ty + 1}>
        };
        
        parse_quote! {
            {
                #left_path as #ty + #right_path as #ty
            }
        }
    }
}