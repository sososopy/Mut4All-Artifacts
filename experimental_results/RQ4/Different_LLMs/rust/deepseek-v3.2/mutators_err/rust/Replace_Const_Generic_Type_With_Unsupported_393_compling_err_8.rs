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

pub struct Replace_Const_Generic_Type_With_Unsupported_393;

impl Mutator for Replace_Const_Generic_Type_With_Unsupported_393 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Type_With_Unsupported_393"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let unsupported_types = vec!["f32", "f64", "bool", "char", "&str"];
        
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut const_params_to_replace = Vec::new();
                
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                let type_name = segment.ident.to_string();
                                if type_name == "usize" || type_name == "u8" || type_name == "u16" || 
                                   type_name == "u32" || type_name == "u64" || type_name == "u128" ||
                                   type_name == "isize" || type_name == "i8" || type_name == "i16" ||
                                   type_name == "i32" || type_name == "i64" || type_name == "i128" {
                                    const_params_to_replace.push((const_param.ident.clone(), type_name));
                                }
                            }
                        }
                    }
                }
                
                if !const_params_to_replace.is_empty() {
                    let (param_ident, _) = const_params_to_replace.choose(&mut rng).unwrap();
                    let new_type = unsupported_types.choose(&mut rng).unwrap();
                    
                    for param in &mut item_impl.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.ident == *param_ident {
                                const_param.ty = parse_quote!(#new_type);
                                
                                if let Type::Path(ty_self) = &mut *item_impl.self_ty {
                                    let mut visitor = ConstExprVisitor {
                                        param_name: param_ident.clone(),
                                        new_type: new_type.to_string(),
                                        found: false,
                                    };
                                    visitor.visit_type_path_mut(ty_self);
                                    
                                    if !visitor.found {
                                        if let Some(segment) = ty_self.path.segments.last_mut() {
                                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::Const(expr) = arg {
                                                        let new_expr: Expr = parse_quote! { #param_ident as usize };
                                                        *expr = new_expr;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks with const generic parameters of integer types. It randomly selects one such parameter and changes its type to an unsupported const generic type (f32, f64, bool, char, or &str). Additionally, it ensures the parameter is used in a const expression within the impl header by adding a cast operation (e.g., 'as usize'). This creates a type mismatch between the impl's const parameter and the struct's expected integer type, potentially triggering const evaluation errors or internal compiler errors when the generic_const_exprs feature is enabled."
    }
}

struct ConstExprVisitor {
    param_name: Ident,
    new_type: String,
    found: bool,
}

impl VisitMut for ConstExprVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Cast(expr_cast) = expr {
            if let Expr::Path(expr_path) = &*expr_cast.expr {
                if expr_path.path.is_ident(&self.param_name) {
                    self.found = true;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
    
    fn visit_generic_argument_mut(&mut self, arg: &mut GenericArgument) {
        if let GenericArgument::Const(expr) = arg {
            if let Expr::Binary(expr_bin) = &mut *expr {
                self.visit_expr_mut(&mut expr_bin.left);
                self.visit_expr_mut(&mut expr_bin.right);
            } else if let Expr::Path(expr_path) = &mut *expr {
                if expr_path.path.is_ident(&self.param_name) {
                    let cast_expr: Expr = parse_quote! { #expr_path as usize };
                    *expr = cast_expr;
                    self.found = true;
                }
            }
        }
        syn::visit_mut::visit_generic_argument_mut(self, arg);
    }
}