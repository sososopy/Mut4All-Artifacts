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

pub struct Modify_Closure_Usage_429;

impl Mutator for Modify_Closure_Usage_429 {
    fn name(&self) -> &str {
        "Modify_Closure_Usage_429"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureFinder {
            closures: Vec<ExprClosure>,
        }

        impl<'ast> Visit<'ast> for ClosureFinder {
            fn visit_expr_closure(&mut self, node: &'ast ExprClosure) {
                self.closures.push(node.clone());
            }
        }

        let mut finder = ClosureFinder { closures: vec![] };
        finder.visit_file(file);

        if finder.closures.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let closure = finder.closures.choose(&mut rng).unwrap();

        struct ClosureReplacer<'a> {
            target: &'a ExprClosure,
        }

        impl<'a> VisitMut for ClosureReplacer<'a> {
            fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
                if node == self.target {
                    node.or1_token = None;
                    node.lifetimes = None;

                    for input in &mut node.inputs {
                        if let Pat::Type(pat_type) = input {
                            if let Type::Reference(type_reference) = &mut *pat_type.ty {
                                type_reference.lifetime = None;
                            }
                        }
                    }

                    if let ReturnType::Type(_, ty) = &mut node.output {
                        if let Type::Reference(type_reference) = &mut **ty {
                            type_reference.lifetime = None;
                        }
                    }
                }
            }
        }

        let mut replacer = ClosureReplacer { target: closure };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closure expressions that use lifetime annotations (e.g., `for<'a>`). It removes the lifetime annotations from the closure, including both the `for<...>` construct and any explicit lifetimes in the closure's parameters and return type. By erasing these lifetimes, the mutator stresses the compiler's lifetime resolution and borrow checking mechanisms, potentially exposing bugs related to lifetime inference and validation."
    }
}