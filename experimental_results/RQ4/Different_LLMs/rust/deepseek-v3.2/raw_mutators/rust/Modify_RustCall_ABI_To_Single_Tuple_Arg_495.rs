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

pub struct Modify_RustCall_ABI_To_Single_Tuple_Arg_495;

impl Mutator for Modify_RustCall_ABI_To_Single_Tuple_Arg_495 {
    fn name(&self) -> &str {
        "Modify_RustCall_ABI_To_Single_Tuple_Arg_495"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = RustCallMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with extern \"rust-call\" ABI and modifies their parameter list to have a single non-self tuple argument. It collects all non-self parameters, creates a tuple type from them, replaces the parameter list with a single tuple parameter, and updates the function body to destructure the tuple via field accesses. This transformation directly tests the compiler's handling of rust-call ABI requirements and may trigger ICEs during MIR transformations like inlining."
    }
}

struct RustCallMutatorVisitor;

impl VisitMut for RustCallMutatorVisitor {
    fn visit_item_fn_mut(&mut self, func: &mut ItemFn) {
        // Check if function has extern "rust-call" ABI
        if let Some(abi) = &func.sig.abi {
            if abi.name.as_ref().map(|lit| lit.value()) == Some("rust-call".to_string()) {
                self.transform_rustcall_function(func);
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, func);
    }
    
    fn visit_impl_item_fn_mut(&mut self, func: &mut syn::ImplItemFn) {
        // Check if method has extern "rust-call" ABI
        if let Some(abi) = &func.sig.abi {
            if abi.name.as_ref().map(|lit| lit.value()) == Some("rust-call".to_string()) {
                self.transform_rustcall_impl_method(func);
            }
        }
        syn::visit_mut::visit_impl_item_fn_mut(self, func);
    }
}

impl RustCallMutatorVisitor {
    fn transform_rustcall_function(&mut self, func: &mut ItemFn) {
        // Separate self parameter from other parameters
        let mut self_param = None;
        let mut other_params = Vec::new();
        let mut other_param_types = Vec::new();
        
        for input in func.sig.inputs.iter() {
            match input {
                FnArg::Receiver(receiver) => {
                    self_param = Some(FnArg::Receiver(receiver.clone()));
                }
                FnArg::Typed(pat_type) => {
                    other_params.push(pat_type.clone());
                    other_param_types.push(*pat_type.ty.clone());
                }
            }
        }
        
        // Create new parameter list
        let mut new_inputs = Punctuated::new();
        
        // Add self parameter if present
        if let Some(self_param) = self_param {
            new_inputs.push(self_param);
        }
        
        // Create tuple type from other parameters
        let tuple_type = if other_param_types.is_empty() {
            // Unit tuple for no parameters
            parse_quote! { () }
        } else {
            Type::Tuple(syn::TypeTuple {
                paren_token: token::Paren::default(),
                elems: other_param_types.into_iter().collect(),
            })
        };
        
        // Add single tuple parameter
        let tuple_param = parse_quote! { args: #tuple_type };
        new_inputs.push(tuple_param);
        
        // Replace the original parameter list
        func.sig.inputs = new_inputs;
        
        // Update function body to use tuple field accesses
        if !other_params.is_empty() {
            self.update_function_body(&mut func.block, &other_params);
        }
    }
    
    fn transform_rustcall_impl_method(&mut self, func: &mut syn::ImplItemFn) {
        // Separate self parameter from other parameters
        let mut self_param = None;
        let mut other_params = Vec::new();
        let mut other_param_types = Vec::new();
        
        for input in func.sig.inputs.iter() {
            match input {
                FnArg::Receiver(receiver) => {
                    self_param = Some(FnArg::Receiver(receiver.clone()));
                }
                FnArg::Typed(pat_type) => {
                    other_params.push(pat_type.clone());
                    other_param_types.push(*pat_type.ty.clone());
                }
            }
        }
        
        // Create new parameter list
        let mut new_inputs = Punctuated::new();
        
        // Add self parameter if present
        if let Some(self_param) = self_param {
            new_inputs.push(self_param);
        }
        
        // Create tuple type from other parameters
        let tuple_type = if other_param_types.is_empty() {
            // Unit tuple for no parameters
            parse_quote! { () }
        } else {
            Type::Tuple(syn::TypeTuple {
                paren_token: token::Paren::default(),
                elems: other_param_types.into_iter().collect(),
            })
        };
        
        // Add single tuple parameter
        let tuple_param = parse_quote! { args: #tuple_type };
        new_inputs.push(tuple_param);
        
        // Replace the original parameter list
        func.sig.inputs = new_inputs;
        
        // Update function body to use tuple field accesses
        if !other_params.is_empty() {
            self.update_function_body(&mut func.block, &other_params);
        }
    }
    
    fn update_function_body(&mut self, block: &mut syn::Block, original_params: &[PatType]) {
        let mut param_renamer = ParamRenamer {
            original_params: original_params.to_vec(),
        };
        param_renamer.visit_block_mut(block);
    }
}

struct ParamRenamer {
    original_params: Vec<PatType>,
}

impl VisitMut for ParamRenamer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        // Replace identifiers that match original parameter names with tuple field accesses
        match expr {
            Expr::Path(expr_path) => {
                if let Some(ident) = expr_path.path.get_ident() {
                    for (index, param) in self.original_params.iter().enumerate() {
                        if let Pat::Ident(pat_ident) = &*param.pat {
                            if pat_ident.ident == *ident {
                                // Replace with args.index
                                let new_path = syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(syn::PathSegment {
                                            ident: Ident::new("args", Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segments.push(syn::PathSegment {
                                            ident: Ident::new(&format!("{}", index), Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segments
                                    },
                                };
                                *expr = Expr::Path(ExprPath {
                                    attrs: expr_path.attrs.clone(),
                                    qself: None,
                                    path: new_path,
                                });
                                break;
                            }
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}