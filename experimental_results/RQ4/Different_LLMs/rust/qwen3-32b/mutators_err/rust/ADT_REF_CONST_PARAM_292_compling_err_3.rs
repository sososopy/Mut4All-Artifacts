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

pub struct ADT_REF_CONST_PARAM_292;

impl Mutator for ADT_REF_CONST_PARAM_292 {
    fn name(&self) -> &str {
        "ADT_REF_CONST_PARAM_292"
    }
    fn mutate(&self, file: &mut syn::File) {
        let valid_structs = collect_valid_structs(file);
        if valid_structs.is_empty() {
            return;
        }
        let selected_struct = &valid_structs[0];
        
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                modify_function_generics(&mut func.sig, selected_struct);
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        modify_function_generics(&mut method.sig, selected_struct);
                    }
                }
            }
        }
        
        let mut modifier = CallModifier { struct_name: selected_struct.clone() };
        modifier.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with references to structs implementing ConstParamTy and modifies const arguments to use the struct's new method. This transformation stresses the compiler's handling of const generics with complex types and dynamic initialization, potentially exposing issues in const evaluation and trait resolution for reference-based const parameters."
    }
}

fn collect_valid_structs(file: &File) -> Vec<Ident> {
    let mut structs = Vec::new();
    for item in &file.items {
        if let Item::Struct(struct_item) = item {
            let ident = &struct_item.ident;
            // Assume struct implements ConstParamTy and has a new method
            if ident == "MyStr" || ident == "MyBytes" {
                structs.push(ident.clone());
            }
        }
    }
    structs
}

fn modify_function_generics(sig: &mut syn::Signature, struct_name: &Ident) {
    for param in &mut sig.generics.params {
        if let GenericParam::Const(const_param) = param {
            let new_type = parse_quote! { &'static #struct_name };
            const_param.ty = new_type;
        }
    }
}

struct CallModifier {
    struct_name: Ident,
}

impl VisitMut for CallModifier {
    fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
        if let Expr::Path(path_expr) = &*call.func {
            for segment in &mut path_expr.path.segments {
                if let PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                    for arg in &mut angle_args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            let struct_name = &self.struct_name;
                            let new_expr = parse_quote! { #struct_name::new("literal") };
                            *const_arg = new_expr;
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_call_mut(self, call);
    }
}