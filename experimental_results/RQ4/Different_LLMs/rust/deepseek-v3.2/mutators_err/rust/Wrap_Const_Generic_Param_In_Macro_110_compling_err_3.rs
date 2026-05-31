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

pub struct Wrap_Const_Generic_Param_In_Macro_110;

impl Mutator for Wrap_Const_Generic_Param_In_Macro_110 {
    fn name(&self) -> &str {
        "Wrap_Const_Generic_Param_In_Macro_110"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_wrapper_macro = false;
        for item in &file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("wrapper") {
                    has_wrapper_macro = true;
                    break;
                }
            }
        }
        if !has_wrapper_macro {
            let wrapper_macro: Item = parse_quote! {
                macro_rules! wrapper {
                    ($e:expr) => { $e };
                }
            };
            file.items.insert(0, wrapper_macro);
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                for param in &generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let param_name = &const_param.ident;
                        let mut visitor = ConstParamVisitor {
                            param_name,
                            changes_made: false,
                        };
                        visitor.visit_item_struct_mut(item_struct);
                        if visitor.changes_made {
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator wraps const generic parameter expressions in a trivial macro invocation when used as a const argument in a type. It first ensures a macro named 'wrapper' exists in the crate (adding it if missing). Then it scans struct definitions for const generic parameters and replaces their direct usage in type arguments with wrapper!(param). This transformation tests interactions between macro expansion and const generic HIR representation, potentially exposing edge cases in macro hygiene and const evaluation order."
    }
}

struct ConstParamVisitor<'a> {
    param_name: &'a Ident,
    changes_made: bool,
}

impl<'a> VisitMut for ConstParamVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            if let Some(last_segment) = type_path.path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            if let Expr::Path(expr_path) = &*const_arg {
                                if expr_path.path.is_ident(self.param_name) {
                                    let wrapped_expr: Expr = parse_quote! {
                                        wrapper!(#expr_path)
                                    };
                                    *const_arg = Box::new(wrapped_expr);
                                    self.changes_made = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}