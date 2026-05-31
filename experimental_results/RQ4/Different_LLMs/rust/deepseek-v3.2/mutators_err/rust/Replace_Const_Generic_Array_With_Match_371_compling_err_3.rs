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

pub struct Replace_Const_Generic_Array_With_Match_371;

impl Mutator for Replace_Const_Generic_Array_With_Match_371 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Array_With_Match_371"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_const_generic_array = false;
                let mut const_param_names = Vec::new();
                
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param_names.push(const_param.ident.to_string());
                    }
                }
                
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        let mut visitor = ConstGenericArrayVisitor {
                            const_params: &const_param_names,
                            found: false,
                        };
                        visitor.visit_type(&ty);
                        if visitor.found {
                            has_const_generic_array = true;
                            break;
                        }
                    }
                }
                
                if has_const_generic_array && !const_param_names.is_empty() {
                    let match_expr: Expr = parse_quote! {
                        match arr {
                            [] | _ => {}
                        }
                    };
                    
                    let mut new_block = item_fn.block.clone();
                    new_block.stmts.clear();
                    new_block.stmts.push(Stmt::Expr(match_expr, None));
                    item_fn.block = new_block;
                }
            }
            
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let mut has_const_generic_array = false;
                        let mut const_param_names = Vec::new();
                        
                        for param in &func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                const_param_names.push(const_param.ident.to_string());
                            }
                        }
                        
                        for input in &func.sig.inputs {
                            if let FnArg::Typed(PatType { ty, .. }) = input {
                                let mut visitor = ConstGenericArrayVisitor {
                                    const_params: &const_param_names,
                                    found: false,
                                };
                                visitor.visit_type(&ty);
                                if visitor.found {
                                    has_const_generic_array = true;
                                    break;
                                }
                            }
                        }
                        
                        if has_const_generic_array && !const_param_names.is_empty() {
                            let match_expr: Expr = parse_quote! {
                                match arr {
                                    [] | _ => {}
                                }
                            };
                            
                            let mut new_block = func.block.clone();
                            new_block.stmts.clear();
                            new_block.stmts.push(Stmt::Expr(match_expr, None));
                            func.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions and methods that use const generic parameters in array types. It replaces the function body with a match expression containing slice patterns that reference the const generic parameter. This transformation forces the compiler to evaluate const generics during pattern matching, potentially triggering ICEs related to const evaluation and pattern matching interactions."
    }
}

struct ConstGenericArrayVisitor<'a> {
    const_params: &'a Vec<String>,
    found: bool,
}

impl<'a> Visit<'a> for ConstGenericArrayVisitor<'a> {
    fn visit_type_array(&mut self, node: &'a syn::TypeArray) {
        if let Expr::Path(ExprPath { path, .. }) = &node.len {
            if let Some(segment) = path.segments.last() {
                if self.const_params.contains(&segment.ident.to_string()) {
                    self.found = true;
                }
            }
        }
        syn::visit::visit_type_array(self, node);
    }
}