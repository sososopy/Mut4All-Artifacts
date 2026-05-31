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

pub struct Replace_TAIT_With_Mismatched_Closure_487;

impl Mutator for Replace_TAIT_With_Mismatched_Closure_487 {
    fn name(&self) -> &str {
        "Replace_TAIT_With_Mismatched_Closure_487"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TAITVisitor {
            rng: thread_rng(),
            modified: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates functions returning TAITs (type alias impl trait) or using TAITs in trait bounds. It modifies closures within those functions to have mismatched parameter or return types while superficially maintaining trait bounds. This aims to trigger bugs in subtype predicates and hidden type registration for opaque types by introducing inconsistent signatures that still implement required traits."
    }
}

struct TAITVisitor {
    rng: rand::rngs::ThreadRng,
    modified: bool,
}

impl VisitMut for TAITVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        // Check if function has TAIT in return type or parameters
        let has_tait_return = match &i.sig.output {
            ReturnType::Type(_, ty) => matches!(**ty, Type::ImplTrait(_)),
            _ => false,
        };

        let has_tait_param = i.sig.inputs.iter().any(|input| {
            if let FnArg::Typed(pat_type) = input {
                matches!(*pat_type.ty, Type::ImplTrait(_))
            } else {
                false
            }
        });

        if !has_tait_return && !has_tait_param {
            return;
        }

        // Collect all closures in the function body
        let mut closures = Vec::new();
        self.collect_closures(&i.block, &mut closures);

        if closures.is_empty() {
            return;
        }

        // Select a random closure to mutate
        let target_closure = closures.choose_mut(&mut self.rng);
        if let Some(closure) = target_closure {
            self.mutate_closure(closure);
            self.modified = true;
        }

        syn::visit_mut::visit_item_fn_mut(self, i);
    }
}

impl TAITVisitor {
    fn collect_closures(&mut self, block: &syn::Block, closures: &mut Vec<syn::ExprClosure>) {
        struct ClosureCollector(Vec<syn::ExprClosure>);
        impl VisitMut for ClosureCollector {
            fn visit_expr_closure_mut(&mut self, i: &mut syn::ExprClosure) {
                self.0.push(i.clone());
                syn::visit_mut::visit_expr_closure_mut(self, i);
            }
        }

        let mut collector = ClosureCollector(Vec::new());
        collector.visit_block_mut(&mut block.clone());
        closures.extend(collector.0);
    }

    fn mutate_closure(&mut self, closure: &mut syn::ExprClosure) {
        // Determine mutation type: change parameters or return type
        let mutation_type = self.rng.gen_range(0..2);

        match mutation_type {
            0 => {
                // Modify parameters
                self.modify_closure_parameters(closure);
            }
            1 => {
                // Modify return type
                self.modify_closure_return_type(closure);
            }
            _ => unreachable!(),
        }
    }

    fn modify_closure_parameters(&mut self, closure: &mut syn::ExprClosure) {
        match &mut closure.inputs {
            syn::Pat::None => {
                // Add a parameter
                let param_name = Ident::new("x", Span::call_site());
                closure.inputs = parse_quote!(#param_name);
            }
            syn::Pat::Ident(pat_ident) => {
                // Convert to PatType with a random type
                let param_name = pat_ident.ident.clone();
                let new_type = self.get_random_type();
                closure.inputs = parse_quote!(#param_name: #new_type);
            }
            syn::Pat::Type(pat_type) => {
                // Change the type
                let new_type = self.get_random_type();
                pat_type.ty = Box::new(new_type);
            }
            syn::Pat::Tuple(tuple) => {
                // Modify one of the tuple elements if present
                if !tuple.elems.is_empty() {
                    let idx = self.rng.gen_range(0..tuple.elems.len());
                    let elem = &mut tuple.elems[idx];
                    
                    match elem {
                        syn::Pat::Ident(pat_ident) => {
                            let param_name = pat_ident.ident.clone();
                            let new_type = self.get_random_type();
                            *elem = parse_quote!(#param_name: #new_type);
                        }
                        syn::Pat::Type(pat_type) => {
                            let new_type = self.get_random_type();
                            pat_type.ty = Box::new(new_type);
                        }
                        _ => {
                            let param_name = Ident::new("x", Span::call_site());
                            *elem = parse_quote!(#param_name);
                        }
                    }
                }
            }
            _ => {
                // Convert to simple identifier pattern
                let param_name = Ident::new("x", Span::call_site());
                closure.inputs = parse_quote!(#param_name);
            }
        }
    }

    fn modify_closure_return_type(&mut self, closure: &mut syn::ExprClosure) {
        // Add or change return type annotation
        let return_type = self.get_random_type();
        closure.output = syn::ReturnType::Type(Default::default(), Box::new(return_type));
    }

    fn get_random_type(&mut self) -> syn::Type {
        let types: [syn::Type; 9] = [
            parse_quote!(i32),
            parse_quote!(u32),
            parse_quote!(bool),
            parse_quote!(String),
            parse_quote!(&str),
            parse_quote!(usize),
            parse_quote!(isize),
            parse_quote!(f64),
            parse_quote!(()),
        ];
        
        let idx = self.rng.gen_range(0..types.len());
        types[idx].clone()
    }
}