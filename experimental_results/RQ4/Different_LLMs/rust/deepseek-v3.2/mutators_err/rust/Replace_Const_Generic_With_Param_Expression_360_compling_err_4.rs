use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Generic_With_Param_Expression_360;

impl Mutator for Replace_Const_Generic_With_Param_Expression_360 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Param_Expression_360"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = ReplaceConstGenericVisitor {
            rng: &mut rng,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments in function calls and type paths. It identifies literal const arguments (e.g., true, false, integer literals) and replaces them with expressions referencing other const generic parameters from the same context. If insufficient parameters exist, it adds a new const generic parameter with a default value. This transformation stresses the compiler's const evaluation, particularly with generic_const_exprs, by introducing unevaluated const expressions during trait solving."
    }
}

struct ReplaceConstGenericVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    mutated: bool,
}

impl<'a> VisitMut for ReplaceConstGenericVisitor<'a> {
    fn visit_item_fn_mut(&mut self, node: &mut ItemFn) {
        if self.mutated {
            return;
        }
        if node.sig.ident == "main" {
            return;
        }
        
        let const_params: Vec<GenericParam> = node.sig.generics.params.iter()
            .filter(|p| matches!(p, GenericParam::Const(_)))
            .cloned()
            .collect();
        
        let mut const_params_refs: Vec<GenericParam> = const_params;
        
        if const_params_refs.len() < 2 {
            let new_param_name = Ident::new("NEW_CONST_PARAM", Span::call_site());
            let new_param: GenericParam = parse_quote!(const #new_param_name: bool = false);
            node.sig.generics.params.push(new_param);
            const_params_refs = node.sig.generics.params.iter()
                .filter(|p| matches!(p, GenericParam::Const(_)))
                .cloned()
                .collect();
        }
        
        if const_params_refs.len() >= 2 {
            let mut visitor = CallSiteVisitor {
                rng: self.rng,
                const_params: &const_params_refs,
                mutated: &mut self.mutated,
            };
            visitor.visit_item_fn_mut(node);
        }
        
        syn::visit_mut::visit_item_fn_mut(self, node);
    }
    
    fn visit_item_impl_mut(&mut self, node: &mut ItemImpl) {
        if self.mutated {
            return;
        }
        
        let const_params: Vec<GenericParam> = node.generics.params.iter()
            .filter(|p| matches!(p, GenericParam::Const(_)))
            .cloned()
            .collect();
        
        let mut const_params_refs: Vec<GenericParam> = const_params;
        
        if const_params_refs.len() < 2 {
            let new_param_name = Ident::new("NEW_CONST_PARAM", Span::call_site());
            let new_param: GenericParam = parse_quote!(const #new_param_name: bool = false);
            node.generics.params.push(new_param);
            const_params_refs = node.generics.params.iter()
                .filter(|p| matches!(p, GenericParam::Const(_)))
                .cloned()
                .collect();
        }
        
        if const_params_refs.len() >= 2 {
            let mut visitor = CallSiteVisitor {
                rng: self.rng,
                const_params: &const_params_refs,
                mutated: &mut self.mutated,
            };
            visitor.visit_item_impl_mut(node);
        }
        
        syn::visit_mut::visit_item_impl_mut(self, node);
    }
}

struct CallSiteVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    const_params: &'a Vec<GenericParam>,
    mutated: &'a mut bool,
}

impl<'a> VisitMut for CallSiteVisitor<'a> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if *self.mutated {
            return;
        }
        
        if let Expr::Path(ExprPath { path, .. }) = &mut *node.func {
            if let Some(last_segment) = path.segments.last_mut() {
                self.process_path_arguments(&mut last_segment.arguments);
            }
        }
        
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
    
    fn visit_type_path_mut(&mut self, node: &mut TypePath) {
        if *self.mutated {
            return;
        }
        
        if let Some(last_segment) = node.path.segments.last_mut() {
            self.process_path_arguments(&mut last_segment.arguments);
        }
        
        syn::visit_mut::visit_type_path_mut(self, node);
    }
}

impl<'a> CallSiteVisitor<'a> {
    fn process_path_arguments(&mut self, arguments: &mut PathArguments) {
        if let PathArguments::AngleBracketed(angle_args) = arguments {
            let mut literal_indices = Vec::new();
            
            for (i, arg) in angle_args.args.iter().enumerate() {
                if let GenericArgument::Const(const_expr) = arg {
                    if self.is_literal_const(const_expr) {
                        literal_indices.push(i);
                    }
                }
            }
            
            if !literal_indices.is_empty() && self.const_params.len() >= 2 {
                let target_idx = *literal_indices.choose(self.rng).unwrap();
                let param_idx = self.rng.gen_range(0..self.const_params.len());
                
                if let GenericParam::Const(const_param) = &self.const_params[param_idx] {
                    let param_ident = &const_param.ident;
                    
                    let new_expr = if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                        if path.is_ident("bool") {
                            if self.rng.gen_bool(0.5) {
                                parse_quote!(#param_ident)
                            } else {
                                parse_quote!(!#param_ident)
                            }
                        } else {
                            parse_quote!(#param_ident)
                        }
                    } else {
                        parse_quote!(#param_ident)
                    };
                    
                    if let GenericArgument::Const(old_expr) = &angle_args.args[target_idx] {
                        if self.is_literal_const(old_expr) {
                            angle_args.args[target_idx] = GenericArgument::Const(new_expr);
                            *self.mutated = true;
                        }
                    }
                }
            }
        }
    }
    
    fn is_literal_const(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Lit(_) => true,
            Expr::Path(ExprPath { path, .. }) => {
                path.segments.len() == 1 && 
                (path.segments[0].ident == "true" || path.segments[0].ident == "false")
            }
            _ => false,
        }
    }
}