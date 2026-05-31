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

pub struct Replace_Const_Generic_With_Function_Identifier_430;

impl Mutator for Replace_Const_Generic_With_Function_Identifier_430 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Function_Identifier_430"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut function_idents = Vec::new();
        let mut const_arg_positions: Vec<(usize, usize)> = Vec::new();
        
        struct FunctionCollector {
            idents: Vec<Ident>,
        }
        impl Visit<'_> for FunctionCollector {
            fn visit_item_fn(&mut self, node: &ItemFn) {
                self.idents.push(node.sig.ident.clone());
                syn::visit::visit_item_fn(self, node);
            }
        }
        
        struct ConstArgFinder {
            positions: Vec<(usize, usize)>,
        }
        impl VisitMut for ConstArgFinder {
            fn visit_type_mut(&mut self, node: &mut Type) {
                if let Type::Array(arr) = node {
                    if let Expr::Path(_) = &arr.len {
                        let elem_ptr = &mut arr.elem as *mut Box<Type>;
                        let elem_index = elem_ptr as usize;
                        let node_index = node as *mut Type as usize;
                        self.positions.push((node_index, elem_index));
                    }
                }
                syn::visit_mut::visit_type_mut(self, node);
            }
            fn visit_where_clause_mut(&mut self, node: &mut WhereClause) {
                let node_index = node as *mut WhereClause as usize;
                for predicate in &mut node.predicates {
                    if let WherePredicate::Type(pred_type) = predicate {
                        if let Type::Array(arr) = &mut pred_type.bounded_ty {
                            if let Expr::Path(_) = &arr.len {
                                let bounded_ty_ptr = &mut pred_type.bounded_ty as *mut Type;
                                let bounded_ty_index = bounded_ty_ptr as usize;
                                self.positions.push((bounded_ty_index, node_index));
                            }
                        }
                    }
                }
                syn::visit_mut::visit_where_clause_mut(self, node);
            }
        }
        
        let mut collector = FunctionCollector { idents: Vec::new() };
        collector.visit_file(file);
        function_idents = collector.idents;
        
        let mut finder = ConstArgFinder { positions: Vec::new() };
        finder.visit_file_mut(file);
        
        if !function_idents.is_empty() && !finder.positions.is_empty() {
            let target_position = finder.positions.choose(&mut rng).unwrap();
            let target_function = function_idents.choose(&mut rng).unwrap().clone();
            
            let node_ptr = target_position.0 as *mut Type;
            let where_ptr = target_position.1 as *mut WhereClause;
            
            unsafe {
                if let Type::Array(arr) = &mut *node_ptr {
                    arr.len = Expr::Path(ExprPath {
                        attrs: Vec::new(),
                        qself: None,
                        path: SynPath::from(target_function),
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a const generic argument in a where-clause bound with a function item identifier from the current scope. It collects all free function identifiers and finds const argument positions in where-clause bounds (specifically array repeat expressions). It then randomly selects one const argument position and one function identifier, replacing the const argument with the function identifier. This creates invalid const generic usage that may expose bugs in the compiler's const generics lowering or well-formedness checking, particularly under incomplete features like min_generic_const_args."
    }
}