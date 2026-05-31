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

pub struct Change_Const_Trait_Impl_Arg_Count_317;

impl Mutator for Change_Const_Trait_Impl_Arg_Count_317 {
    fn name(&self) -> &str {
        "Change_Const_Trait_Impl_Arg_Count_317"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(constness) = &item_impl.constness {
                    if item_impl.trait_.is_some() {
                        let trait_ref = item_impl.trait_.as_ref().unwrap();
                        let trait_path = &trait_ref.1;
                        let trait_items = find_trait_methods(file, trait_path);
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(func) = impl_item {
                                let method_name = &func.sig.ident;
                                if let Some(trait_method) = trait_items.iter().find(|tm| tm.sig.ident == *method_name) {
                                    let trait_param_count = trait_method.sig.inputs.len();
                                    let impl_param_count = func.sig.inputs.len();
                                    if trait_param_count == impl_param_count {
                                        let new_count = if trait_param_count == 0 {
                                            1
                                        } else {
                                            if rng.gen_bool(0.5) {
                                                trait_param_count + 1
                                            } else {
                                                trait_param_count - 1
                                            }
                                        };
                                        if new_count >= 0 {
                                            let mut new_inputs = Punctuated::new();
                                            if new_count > trait_param_count {
                                                for i in 0..new_count {
                                                    if i < trait_param_count {
                                                        new_inputs.push(func.sig.inputs[i].clone());
                                                    } else {
                                                        let param_type = if trait_param_count > 0 {
                                                            match &trait_method.sig.inputs[0] {
                                                                syn::FnArg::Typed(pt) => pt.ty.clone(),
                                                                _ => Box::new(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(syn::Ident::new("i32", Span::call_site()))
                                }))
                                                            }
                                                        } else {
                                                            Box::new(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(syn::Ident::new("i32", Span::call_site()))
                            }))
                                                        };
                                                        let param_name = syn::Ident::new(&format!("extra_{}", i), Span::call_site());
                                                        new_inputs.push(syn::FnArg::Typed(syn::PatType {
                                                            attrs: Vec::new(),
                                                            pat: Box::new(syn::Pat::Ident(syn::PatIdent {
                                                                attrs: Vec::new(),
                                                                by_ref: None,
                                                                mutability: None,
                                                                ident: param_name,
                                                                subpat: None,
                                                            })),
                                                            colon_token: syn::token::Colon::default(),
                                                            ty: param_type,
                                                        }));
                                                    }
                                                }
                                            } else {
                                                for i in 0..new_count {
                                                    new_inputs.push(func.sig.inputs[i].clone());
                                                }
                                            }
                                            func.sig.inputs = new_inputs;
                                            if let syn::Expr::Block(ref mut block) = &mut func.block {
                                                if new_count > trait_param_count {
                                                    if block.stmts.len() == 1 {
                                                        if let syn::Stmt::Expr(expr, _) = &block.stmts[0] {
                                                            if let syn::Expr::Binary(_) = expr {
                                                            } else {
                                                            }
                                                        }
                                                    }
                                                } else if new_count < trait_param_count {
                                                    if block.stmts.len() == 1 {
                                                        if let syn::Stmt::Expr(expr, _) = &mut block.stmts[0] {
                                                            if let syn::Expr::Binary(ref mut bin_expr) = expr {
                                                                if let syn::Expr::Path(ref mut left_path) = *bin_expr.left {
                                    if let syn::Expr::Path(ref mut right_path) = *bin_expr.right {
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
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const trait implementations, altering the number of parameters in the impl method signature to mismatch the trait signature. It either adds an extra parameter using the type of the first trait parameter (or i32 if none) or removes the last parameter. This creates ABI mismatches in const contexts, testing the compiler's const evaluation logic and trait method resolution under inconsistent signatures."
    }
}

fn find_trait_methods(file: &syn::File, trait_path: &syn::Path) -> Vec<syn::TraitItemFn> {
    let mut methods = Vec::new();
    for item in &file.items {
        if let syn::Item::Trait(item_trait) = item {
            if item_trait.ident == trait_path.segments.last().unwrap().ident {
                for trait_item in &item_trait.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        methods.push(method.clone());
                    }
                }
            }
        }
    }
    methods
}